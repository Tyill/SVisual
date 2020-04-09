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

#include "stdafx.h"
#include "Lib/libuv/uv.h"
#include "SVAuxFunc/serverTCP.h"

namespace SV_TcpSrv {
    	
#define SRVCheck(func, mess){ int fsts = func; \
 if (fsts != 0){ if (server.errCBack) server.errCBack(std::string(mess) + " " + std::to_string(fsts)); return; }}

    struct client_t {
		uv_tcp_t handle;
		uv_write_t write_req;
        uv_buf_t buf;
		std::string inMess;     
		std::string outMess;    

        client_t(){
            buf.base = nullptr;
            buf.len = 0;
        }

        ~client_t(){
            if (buf.base)
              free(buf.base);
        }
	};
       
	struct server_t {
		uv_tcp_t u_server;
		uv_loop_t *uv_loop;
		dataCBack dataCBack_;   
		errorCBack errCBack;    

		std::string addr;       
		int port;               
		int tout;               // ждать связи, мс
		bool keepAlive;         // оставлять подключение активным
		bool isRun;             

		server_t() {
			uv_loop = nullptr;
			dataCBack_ = nullptr;
			errCBack = nullptr;
			isRun = false;
		}
	};

    std::thread thr;

   server_t server{};

    void on_close(uv_handle_t *handle) {
		client_t *client = (client_t *) handle->data;
		delete client;
	}

    
    void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
         
        client_t* client = (client_t*)handle->data;

        if (client->buf.len < suggested_size){
            client->buf.base = (char*)realloc(client->buf.base, suggested_size);
            client->buf.len = suggested_size;
        }

        *buf = client->buf;
	}

    
	void on_read(uv_stream_t *tcp, ssize_t nread, const uv_buf_t* buf) {

		client_t* client = (client_t*)tcp->data;

		if (nread > 0) {
            
            // копим данные          
            auto clSz = client->inMess.size();
            client->inMess.resize(clSz + nread);
            memcpy((char *)client->inMess.data() + clSz, buf->base, nread);
            
			// передача пользователю
			if (server.dataCBack_)
				server.dataCBack_(client->inMess, client->outMess);

			// ответ
			if (!client->outMess.empty()) {

				uv_buf_t resbuf;
				resbuf.base = (char *) client->outMess.c_str();
				resbuf.len = client->outMess.size() + 1;

				uv_write(&client->write_req,
								 (uv_stream_t *) &client->handle,
								 &resbuf,
								 1,
								 [](uv_write_t *req, int status) {
									 SRVCheck(status, "on_read::uv_write error");
								 });
			}
		} else if (nread < 0) {

			if (!uv_is_closing((uv_handle_t *) client))
				uv_close((uv_handle_t *) client, on_close);
		}
	}
        
	void on_connect(uv_stream_t *server_handle, int sts) {
        		
		SRVCheck(sts, "on_connect:: error");

		client_t *client = new client_t();

		SRVCheck(uv_tcp_init(server.uv_loop, &client->handle),
			 "on_connect::uv_tcp_init error");

		client->handle.data = client;

		SRVCheck(uv_accept(server_handle, (uv_stream_t *) &client->handle),
			 "on_connect::uv_accept error");

		SRVCheck(uv_read_start((uv_stream_t *) &client->handle, alloc_cb, on_read),
			 "on_connect::uv_read_start error");
	}
       
	void initConnection() {

		server.uv_loop = uv_default_loop();

		SRVCheck(uv_tcp_init(server.uv_loop, &server.u_server),
			 "initConnection::uv_tcp_init error");

		SRVCheck(uv_tcp_keepalive(&server.u_server, server.keepAlive, server.tout),
			 "initConnection::uv_tcp_keepalive error");

		struct sockaddr_in address;
		SRVCheck(uv_ip4_addr(server.addr.c_str(), server.port, &address),
			 "initConnection::uv_ip4_addr error");

		SRVCheck(uv_tcp_bind(&server.u_server, (const struct sockaddr *) &address, 0),
			 "initConnection::uv_tcp_bind error");

		SRVCheck(uv_listen((uv_stream_t *) &server.u_server, 1000, on_connect),
			 "initConnection::uv_listen error");

		server.isRun = true;

		SRVCheck(uv_run(server.uv_loop, UV_RUN_DEFAULT),
			 "initConnection::uv_run error");
	}
       
	bool runServer(std::string addr, int port, bool keepAlive, int tout) {

		if (server.isRun) return true;

		server.addr = addr;
		server.port = port;
		server.keepAlive = keepAlive;
		server.tout = tout;

		std::condition_variable cval;
		thr = std::thread([&cval]() {
			initConnection();
			server.isRun = false;
			cval.notify_one();
		});
		thr.detach();

		std::mutex mtx;
		std::unique_lock<std::mutex> lck(mtx);
		cval.wait_for(lck, std::chrono::milliseconds(100));

		return server.isRun;
	}

    void stopServer() {

		if (!server.isRun) return;

		uv_stop(server.uv_loop);

		uv_loop_close(server.uv_loop);

		uv_loop_delete(server.uv_loop);
	}
        
	void setDataCBack(dataCBack uf) {

		server.dataCBack_ = uf;
	}
    
	void setErrorCBack(errorCBack uf) {

		server.errCBack = uf;
	}

#undef SRVCheck
}