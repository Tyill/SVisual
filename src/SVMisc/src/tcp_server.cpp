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
#include <atomic>
#include <cstdlib>
#include <cstring>
#include <mutex>
#include <thread>
#include <condition_variable>

#include "libuv/uv.h"
#include "SVMisc/tcp_server.h"

#ifdef __linux__
  #define ULONG size_t
#endif

namespace SV_Misc {
namespace TCPServer {

    struct WriteReq {
      uv_write_t req{};
      std::string payload;
    };

    struct Client_m {
      uv_tcp_t u_client{};
      uv_buf_t u_buf{};
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

    struct Server_m {
      uv_loop_t u_loop_storage{};
      std::atomic<uv_loop_t*> u_loop{nullptr};
      uv_tcp_t u_server{};
      uv_async_t u_stop{};
      DataCBack dataCBack;
      ErrorCBack errCBack;
      std::string error;
      std::thread thr;
      std::atomic<bool> stop_inited{false};
      std::atomic<bool> stop_requested{false};
      std::atomic<bool> loop_dead{false};

      std::mutex start_mtx;
      std::condition_variable start_cval;
      bool listen_ready = false;
    } srv;

    void reportError(const std::string& mess, int code = 0) {
      srv.error = code ? mess + " code " + std::to_string(code) : mess;
      if (srv.errCBack)
        srv.errCBack(srv.error);
    }

    void on_client_close(uv_handle_t* handle) {
      delete static_cast<Client_m*>(handle->data);
    }

    void closeClient(Client_m* client) {
      uv_handle_t* h = reinterpret_cast<uv_handle_t*>(&client->u_client);
      if (!uv_is_closing(h))
        uv_close(h, on_client_close);
    }

    void on_write(uv_write_t* req, int /*status*/) {
      delete static_cast<WriteReq*>(req->data);
    }

    void alloc_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
      Client_m* client = static_cast<Client_m*>(handle->data);

      if (client->u_buf.len < (ULONG)suggested_size) {
        char* base = static_cast<char*>(realloc(client->u_buf.base, suggested_size));
        if (!base) {
          buf->base = nullptr;
          buf->len = 0;
          return;
        }
        client->u_buf.base = base;
        client->u_buf.len = (ULONG)suggested_size;
      }

      *buf = client->u_buf;
    }

    void on_read(uv_stream_t* u_client, ssize_t nread, const uv_buf_t* buf) {
      if (nread > 0) {
        Client_m* client = static_cast<Client_m*>(u_client->data);

        const auto csz = client->inMess.size();
        client->inMess.resize(csz + static_cast<size_t>(nread));
        memcpy(client->inMess.data() + csz, buf->base, static_cast<size_t>(nread));

        if (srv.dataCBack)
          srv.dataCBack(client->inMess, client->outMess);

        if (!client->outMess.empty()) {
          auto* wr = new WriteReq();
          wr->payload = std::move(client->outMess);
          wr->payload.push_back('\0');
          wr->req.data = wr;

          uv_buf_t resbuf;
          resbuf.base = wr->payload.data();
          resbuf.len = (ULONG)wr->payload.size();

          if (uv_write(&wr->req, u_client, &resbuf, 1, on_write) != 0)
            delete wr;
        }
      } else if (nread < 0) {
        closeClient(static_cast<Client_m*>(u_client->data));
      }
    }

    void on_connect(uv_stream_t* u_server, int sts) {
      if (sts != 0) {
        reportError("on_connect::sts error", sts);
        return;
      }

      Client_m* client = new Client_m();
      if (uv_tcp_init(u_server->loop, &client->u_client) != 0) {
        reportError("on_connect::uv_tcp_init error");
        delete client;
        return;
      }
      client->u_client.data = client;

      if (uv_accept(u_server, reinterpret_cast<uv_stream_t*>(&client->u_client)) != 0) {
        reportError("on_connect::uv_accept error");
        uv_close(reinterpret_cast<uv_handle_t*>(&client->u_client), on_client_close);
        return;
      }

      if (uv_read_start(reinterpret_cast<uv_stream_t*>(&client->u_client), alloc_cb, on_read) != 0) {
        reportError("on_connect::uv_read_start error");
        uv_close(reinterpret_cast<uv_handle_t*>(&client->u_client), on_client_close);
      }
    }

    void walk_close(uv_handle_t* h, void* /*arg*/) {
      if (uv_is_closing(h))
        return;

      if (h == reinterpret_cast<uv_handle_t*>(&srv.u_server)) {
        uv_close(h, nullptr);
      } else if (h == reinterpret_cast<uv_handle_t*>(&srv.u_stop)) {
        uv_close(h, [](uv_handle_t*) { srv.stop_inited.store(false); });
      } else {
        uv_close(h, on_client_close);
      }
    }

    void closeOpenHandles(uv_loop_t* loop) {
      uv_walk(loop, walk_close, nullptr);
    }

    void on_stop_async(uv_async_t* /*handle*/) {
      uv_loop_t* loop = srv.u_loop.load();
      if (loop)
        closeOpenHandles(loop);
    }

    bool closeLoop(uv_loop_t* loop) {
      if (!loop)
        return true;

      int rc = UV_EBUSY;
      for (int n = 0; rc == UV_EBUSY && n < 64; ++n) {
        closeOpenHandles(loop);
        for (int k = 0; uv_loop_alive(loop) && k < 64; ++k)
          uv_run(loop, UV_RUN_NOWAIT);
        rc = uv_loop_close(loop);
      }

      srv.u_loop.store(nullptr);
      if (rc != 0)
        srv.loop_dead.store(true);
      return rc == 0;
    }

    void signalListenReady() {
      {
        std::lock_guard<std::mutex> lck(srv.start_mtx);
        srv.listen_ready = true;
      }
      srv.start_cval.notify_one();
    }

    // Loop exists, async not inited: stop() could not send. Close locally.
    void failAndClose(uv_loop_t* loop) {
      closeLoop(loop);
      signalListenReady();
    }

    // Async is live: never close it here — queue stop and let uv_run drain send
    // (ours and/or concurrent stop()).
    void requestStopAndRun(uv_loop_t* loop) {
      uv_async_send(&srv.u_stop);
      uv_run(loop, UV_RUN_DEFAULT);
      closeLoop(loop);
    }

    void startImpl(std::string addr, uint16_t port) {
      if (srv.stop_requested.load()) {
        reportError("initConnection::stopped");
        signalListenReady();
        return;
      }

      if (uv_loop_init(&srv.u_loop_storage) != 0) {
        reportError("initConnection::uv_loop_init error");
        signalListenReady();
        return;
      }
      uv_loop_t* loop = &srv.u_loop_storage;
      srv.u_loop.store(loop);

      if (srv.stop_requested.load()) {
        reportError("initConnection::stopped");
        failAndClose(loop);
        return;
      }

      if (uv_async_init(loop, &srv.u_stop, on_stop_async) != 0) {
        reportError("initConnection::uv_async_init error");
        failAndClose(loop);
        return;
      }
      srv.stop_inited.store(true);

      if (srv.stop_requested.load()) {
        reportError("initConnection::stopped");
        signalListenReady();
        requestStopAndRun(loop);
        return;
      }

      int sts = uv_tcp_init(loop, &srv.u_server);
      if (sts != 0) {
        reportError("initConnection::uv_tcp_init error", sts);
        signalListenReady();
        requestStopAndRun(loop);
        return;
      }
      srv.u_server.data = nullptr;

      sts = uv_tcp_keepalive(&srv.u_server, 1, 60);

      sockaddr_in address{};
      if (sts == 0)
        sts = uv_ip4_addr(addr.c_str(), port, &address);
      if (sts == 0)
        sts = uv_tcp_bind(&srv.u_server, reinterpret_cast<const sockaddr*>(&address), 0);
      if (sts == 0)
        sts = uv_listen(reinterpret_cast<uv_stream_t*>(&srv.u_server), 1000, on_connect);

      if (sts != 0 || srv.stop_requested.load()) {
        reportError(sts != 0 ? "initConnection error" : "initConnection::stopped", sts);
        signalListenReady();
        requestStopAndRun(loop);
        return;
      }

      signalListenReady();
      uv_run(loop, UV_RUN_DEFAULT);
      closeLoop(loop);
    }

    void joinWorkerIfExternal() {
      if (!srv.thr.joinable())
        return;
      if (std::this_thread::get_id() == srv.thr.get_id())
        srv.thr.detach();
      else
        srv.thr.join();
    }

    bool start(const std::string& addr, uint16_t port, bool intoOtherThread) {
      if (srv.loop_dead.load())
        return false;
      if (srv.u_loop.load())
        return true;

      if (srv.thr.joinable())
        joinWorkerIfExternal();

      srv.error.clear();
      srv.stop_requested.store(false);
      {
        std::lock_guard<std::mutex> lck(srv.start_mtx);
        srv.listen_ready = false;
      }

      if (intoOtherThread) {
        srv.thr = std::thread(startImpl, addr, port);
        std::unique_lock<std::mutex> lck(srv.start_mtx);
        srv.start_cval.wait(lck, [] { return srv.listen_ready; });
        if (!srv.error.empty())
          joinWorkerIfExternal();
      } else {
        startImpl(addr, port);
      }
      return srv.error.empty();
    }

    void stop() {
      srv.stop_requested.store(true);

      if (!srv.u_loop.load() && !srv.thr.joinable())
        return;

      if (srv.stop_inited.load())
        uv_async_send(&srv.u_stop);

      joinWorkerIfExternal();
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
