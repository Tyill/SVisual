//
// SVisual Project
// Copyright (C) 2018 by Contributors <https://github.com/Tyill/SVisual>
//
// This code is licensed under the MIT License.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include <map>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstring>

#include "libuv/uv.h"
#include "SVMisc/tcp_server.h"

#ifdef __linux__
  #define ULONG size_t 
#endif

namespace SV_Misc {
  namespace TCPServer {
     
    struct Server_m {
      uv_tcp_t u_server;
      uv_loop_t* u_loop;
      DataCBack dataCBack;
      ErrorCBack errCBack;
      std::string error;
      std::thread thr;
      
      Server_m():
        dataCBack(nullptr), 
        errCBack(nullptr),
        u_loop(nullptr) {};      
    }srv;   

    struct Client_m {
      uv_tcp_t u_client;
      uv_write_t u_writeReq;
      uv_buf_t u_buf;
      std::string inMess;
      std::string outMess;
      
      Client_m() {
        u_buf.base = nullptr;
        u_buf.len = 0;
      }

      ~Client_m() {
        if (u_buf.base)
          free(u_buf.base);
      }
    };

#define CHECK(func, mess){                                             \
    int fsts = func;                                                   \
    if (fsts != 0){                                                    \
      srv.error = std::string(mess) + " code " + std::to_string(fsts); \
      if (srv.errCBack)                                                \
        srv.errCBack(srv.error);                                       \
      return;                                                          \
    }                                                                  \
  }        
    void on_close(uv_handle_t* u_client) {
      Client_m* client = static_cast<Client_m*>(u_client->data);
      delete client;
    }

    void alloc_cb(uv_handle_t* u_client, size_t suggested_size, uv_buf_t* buf) {

      Client_m* client = static_cast<Client_m*>(u_client->data);

      if (client->u_buf.len < (ULONG)suggested_size) {
        client->u_buf.base = (char*)realloc(client->u_buf.base, suggested_size);
        client->u_buf.len = (ULONG)suggested_size;
      }

      *buf = client->u_buf;
    }

    void on_read(uv_stream_t* u_client, ssize_t nread, const uv_buf_t* buf) {
                  
      if (nread > 0) {

        Client_m* client = static_cast<Client_m*>(u_client->data);
        
        // копим данные          
        auto csz = client->inMess.size();
        client->inMess.resize(csz + nread);
        memcpy((char*)client->inMess.data() + csz, buf->base, nread);

        // передача пользователю
        if (srv.dataCBack)
          srv.dataCBack(client->inMess, client->outMess);

        // ответ
        if (!client->outMess.empty()) {
          uv_buf_t resbuf;
          resbuf.base = (char*)client->outMess.c_str();
          resbuf.len = (ULONG)client->outMess.size() + 1;

          uv_write(&client->u_writeReq, u_client, &resbuf, 1, nullptr);
        }
      }
      else if (nread < 0) {
        if (!uv_is_closing((uv_handle_t*)u_client))
          uv_close((uv_handle_t*)u_client, on_close);
      }
    }

    void on_connect(uv_stream_t* u_server, int sts) {
            
      CHECK(sts, "on_connect::sts error");
           
      Client_m* client = new Client_m();
     
      CHECK(uv_tcp_init(u_server->loop, &client->u_client),
        "on_connect::uv_tcp_init error");

      client->u_client.data = client;

      CHECK(uv_accept(u_server, (uv_stream_t*)&client->u_client),
        "on_connect::uv_accept error");

      CHECK(uv_read_start((uv_stream_t *)&client->u_client, alloc_cb, on_read),
        "on_connect::uv_read_start error");
    }
                   
    bool start(const std::string& addr, uint16_t port, bool intoOtherThread) {
                 
      if (srv.u_loop) return true;

      std::condition_variable cval;
      auto startImpl = [&cval](std::string addr, uint16_t port){
          srv.u_loop = uv_default_loop();

          CHECK(uv_tcp_init(srv.u_loop, &srv.u_server),
            "initConnection::uv_tcp_init error");

          CHECK(uv_tcp_keepalive(&srv.u_server, true, 60),
            "initConnection::uv_tcp_keepalive error");

          struct sockaddr_in address;
          CHECK(uv_ip4_addr(addr.c_str(), port, &address),
            "initConnection::uv_ip4_addr error");

          CHECK(uv_tcp_bind(&srv.u_server, (const struct sockaddr*)&address, 0),
            "initConnection::uv_tcp_bind error");

          CHECK(uv_listen((uv_stream_t*)&srv.u_server, 1000, on_connect),
            "initConnection::uv_listen error");

          cval.notify_one();

          CHECK(uv_run(srv.u_loop, UV_RUN_DEFAULT),
            "initConnection::uv_run error");
      };
      if (intoOtherThread){
          srv.thr = std::thread(startImpl, addr, port);
          std::mutex mtx;
          std::unique_lock<std::mutex> lck(mtx);
          cval.wait_for(lck, std::chrono::milliseconds(100));
      }else{
          startImpl(addr, port);
      }
      return srv.error.empty();
    }

    void stop() {      
      if (srv.u_loop){
        uv_async_t async;
        uv_async_init(srv.u_loop, &async, [](uv_async_t* handle){
          uv_stop(srv.u_loop);
          uv_loop_close(srv.u_loop);
        });
        uv_async_send(&async);

        if (srv.thr.joinable()) srv.thr.join();
      }      
    }   

    bool setDataCBack(DataCBack dcb) {
      srv.dataCBack = dcb;
      return true;
    }

    bool setErrorCBack(ErrorCBack ecb) {
      srv.errCBack = ecb;
      return true;
    }

    std::string errorStr() {
      return srv.error;
    }
  }
}
