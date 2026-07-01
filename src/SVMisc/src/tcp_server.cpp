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
#include <chrono>
#include <condition_variable>
#include <cstring>
#include <mutex>
#include <thread>
#include <unordered_set>
#include <vector>

#include "libuv/uv.h"
#include "SVMisc/tcp_server.h"

#ifdef __linux__
  #define ULONG size_t
#endif

namespace SV_Misc {
  namespace TCPServer {

    struct Client_m;

    struct WriteReq {
      uv_write_t req{};
      std::string payload;
    };

    struct Server_m {
      uv_loop_t u_loop_storage{};
      uv_loop_t* u_loop = nullptr;
      uv_tcp_t u_server{};
      uv_async_t stop_async{};

      std::atomic<bool> running{false};
      std::atomic<bool> stopping{false};
      std::atomic<bool> stop_async_inited{false};
      std::atomic<bool> server_inited{false};

      DataCBack dataCBack;
      ErrorCBack errCBack;
      std::string error;
      std::thread thr;

      std::mutex lifecycle_mtx;
      std::mutex clients_mtx;
      std::unordered_set<Client_m*> clients;

      std::mutex start_mtx;
      std::condition_variable start_cval;
      bool listen_ready = false;
    } srv;

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

    void reportError(const std::string& msg) {
      {
        std::lock_guard<std::mutex> lck(srv.lifecycle_mtx);
        srv.error = msg;
      }
      if (srv.errCBack)
        srv.errCBack(msg);
    }

    void signalListenReady() {
      {
        std::lock_guard<std::mutex> lck(srv.start_mtx);
        srv.listen_ready = true;
      }
      srv.start_cval.notify_one();
    }

    void on_client_close(uv_handle_t* handle);

    void closeAllClientsSnapshot() {
      std::vector<Client_m*> snapshot;
      {
        std::lock_guard<std::mutex> lck(srv.clients_mtx);
        snapshot.assign(srv.clients.begin(), srv.clients.end());
      }
      for (Client_m* client : snapshot) {
        if (!uv_is_closing(reinterpret_cast<uv_handle_t*>(&client->u_client)))
          uv_close(reinterpret_cast<uv_handle_t*>(&client->u_client), on_client_close);
      }
    }

    void drainAndCloseLoop() {
      uv_loop_t* loop = nullptr;
      {
        std::lock_guard<std::mutex> lck(srv.lifecycle_mtx);
        if (!srv.u_loop)
          return;
        loop = srv.u_loop;
      }

      while (uv_loop_alive(loop))
        uv_run(loop, UV_RUN_ONCE);

      {
        std::lock_guard<std::mutex> lck(srv.lifecycle_mtx);
        if (srv.u_loop != loop)
          return;

        uv_loop_close(srv.u_loop);
        srv.u_loop = nullptr;
        srv.server_inited.store(false, std::memory_order_release);
        srv.stop_async_inited.store(false, std::memory_order_release);
        srv.running.store(false, std::memory_order_release);
        srv.stopping.store(false, std::memory_order_release);
      }
    }

    void on_client_close(uv_handle_t* handle) {
      Client_m* client = static_cast<Client_m*>(handle->data);
      {
        std::lock_guard<std::mutex> lck(srv.clients_mtx);
        srv.clients.erase(client);
      }
      delete client;
    }

    void on_write(uv_write_t* req, int /*status*/) {
      delete static_cast<WriteReq*>(req->data);
    }

    void alloc_cb(uv_handle_t* u_client, size_t suggested_size, uv_buf_t* buf) {
      Client_m* client = static_cast<Client_m*>(u_client->data);

      if (client->u_buf.len < suggested_size) {
        char* base = static_cast<char*>(realloc(client->u_buf.base, suggested_size));
        if (!base) {
          buf->base = nullptr;
          buf->len = 0;
          if (!uv_is_closing(u_client))
            uv_close(u_client, on_client_close);
          return;
        }
        client->u_buf.base = base;
        client->u_buf.len = suggested_size;
      }

      *buf = client->u_buf;
    }

    void on_read(uv_stream_t* u_client, ssize_t nread, const uv_buf_t* /*buf*/) {
      if (nread > 0) {
        Client_m* client = static_cast<Client_m*>(u_client->data);

        const auto csz = client->inMess.size();
        client->inMess.resize(csz + static_cast<size_t>(nread));
        memcpy(client->inMess.data() + csz, client->u_buf.base, static_cast<size_t>(nread));

        if (srv.dataCBack)
          srv.dataCBack(client->inMess, client->outMess);

        if (!client->outMess.empty()) {
          auto* wr = new WriteReq();
          wr->payload = std::move(client->outMess);
          wr->req.data = wr;

          uv_buf_t resbuf;
          resbuf.base = wr->payload.data();
          resbuf.len = static_cast<ULONG>(wr->payload.size());

          if (uv_write(&wr->req, u_client, &resbuf, 1, on_write) != 0) {
            delete wr;
          }
        }
      } else if (nread < 0) {
        if (!uv_is_closing(reinterpret_cast<uv_handle_t*>(u_client)))
          uv_close(reinterpret_cast<uv_handle_t*>(u_client), on_client_close);
      }
    }

    void on_connect(uv_stream_t* u_server, int sts) {
      if (sts != 0) {
        reportError("on_connect::sts error code " + std::to_string(sts));
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
        return;
      }

      {
        std::lock_guard<std::mutex> lck(srv.clients_mtx);
        srv.clients.insert(client);
      }
    }

    void on_server_close(uv_handle_t* /*handle*/);

    void closeStartupHandles() {
      if (srv.server_inited.load(std::memory_order_acquire) &&
          !uv_is_closing(reinterpret_cast<uv_handle_t*>(&srv.u_server))) {
        uv_close(reinterpret_cast<uv_handle_t*>(&srv.u_server), on_server_close);
      }
      if (srv.stop_async_inited.load(std::memory_order_acquire) &&
          !uv_is_closing(reinterpret_cast<uv_handle_t*>(&srv.stop_async))) {
        uv_close(reinterpret_cast<uv_handle_t*>(&srv.stop_async), nullptr);
      }
    }

    void finishStartup(bool signalReady) {
      drainAndCloseLoop();
      if (signalReady)
        signalListenReady();
    }

    void failStartup(const std::string& msg) {
      reportError(msg);
      closeStartupHandles();
      finishStartup(true);
    }

    bool abortStartupIfRequested() {
      if (!srv.stopping.load())
        return false;

      closeStartupHandles();
      finishStartup(true);
      return true;
    }

    void on_server_close(uv_handle_t* /*handle*/) {}

    void on_stop_async(uv_async_t* /*handle*/) {
      closeAllClientsSnapshot();
      closeStartupHandles();
    }

    void startImpl(std::string addr, uint16_t port) {
      {
        std::lock_guard<std::mutex> lck(srv.lifecycle_mtx);
        srv.u_loop = &srv.u_loop_storage;
      }

      if (uv_loop_init(srv.u_loop) != 0) {
        {
          std::lock_guard<std::mutex> lck(srv.lifecycle_mtx);
          srv.u_loop = nullptr;
        }
        reportError("initConnection::uv_loop_init error");
        signalListenReady();
        return;
      }

      if (uv_async_init(srv.u_loop, &srv.stop_async, on_stop_async) != 0) {
        failStartup("initConnection::uv_async_init error");
        return;
      }
      srv.stop_async_inited.store(true, std::memory_order_release);

      if (uv_tcp_init(srv.u_loop, &srv.u_server) != 0) {
        failStartup("initConnection::uv_tcp_init error");
        return;
      }
      srv.server_inited.store(true, std::memory_order_release);

      if (uv_tcp_keepalive(&srv.u_server, 1, 60) != 0) {
        failStartup("initConnection::uv_tcp_keepalive error");
        return;
      }

      sockaddr_in address{};
      if (uv_ip4_addr(addr.c_str(), port, &address) != 0) {
        failStartup("initConnection::uv_ip4_addr error");
        return;
      }

      if (uv_tcp_bind(&srv.u_server, reinterpret_cast<const sockaddr*>(&address), 0) != 0) {
        failStartup("initConnection::uv_tcp_bind error");
        return;
      }

      if (abortStartupIfRequested())
        return;

      if (uv_listen(reinterpret_cast<uv_stream_t*>(&srv.u_server), 1000, on_connect) != 0) {
        failStartup("initConnection::uv_listen error");
        return;
      }

      if (abortStartupIfRequested())
        return;

      srv.running.store(true, std::memory_order_release);
      signalListenReady();

      if (abortStartupIfRequested())
        return;

      uv_run(srv.u_loop, UV_RUN_DEFAULT);

      drainAndCloseLoop();
    }

    // join from external thread; detach when stop/abort is invoked on worker thread
    void joinWorkerThreadIfExternal() {
      if (!srv.thr.joinable())
        return;
      if (std::this_thread::get_id() == srv.thr.get_id()) {
        srv.thr.detach();
        return;
      }
      srv.thr.join();
    }

    void abortWorker(bool sendAsyncIfPossible) {
      bool sendAsync = false;
      bool needJoin = false;

      {
        std::lock_guard<std::mutex> lck(srv.lifecycle_mtx);
        srv.stopping.store(true, std::memory_order_release);
        sendAsync = sendAsyncIfPossible &&
                    srv.stop_async_inited.load(std::memory_order_acquire) &&
                    srv.u_loop != nullptr;
        needJoin = srv.thr.joinable();
      }

      if (sendAsync)
        uv_async_send(&srv.stop_async);

      if (needJoin)
        joinWorkerThreadIfExternal();
    }

    std::string start(const std::string& addr, uint16_t port,
                      DataCBack dataCb, ErrorCBack errorCb, bool intoOtherThread) {
      {
        std::lock_guard<std::mutex> lck(srv.lifecycle_mtx);
        srv.error.clear();
        if (srv.running.load(std::memory_order_acquire) && srv.u_loop != nullptr)
          return "already running";
        if (dataCb)
          srv.dataCBack = std::move(dataCb);
        if (errorCb)
          srv.errCBack = std::move(errorCb);
      }

      if (srv.thr.joinable() && !srv.running.load(std::memory_order_acquire))
        srv.thr.join();

      {
        std::lock_guard<std::mutex> lck(srv.lifecycle_mtx);
        if (srv.running.load(std::memory_order_acquire) && srv.u_loop != nullptr)
          return "already running";
        srv.stopping.store(false, std::memory_order_release);
      }

      {
        std::lock_guard<std::mutex> lck(srv.start_mtx);
        srv.listen_ready = false;
      }

      if (intoOtherThread) {
        srv.thr = std::thread(startImpl, addr, port);
        std::unique_lock<std::mutex> lck(srv.start_mtx);
        const bool ready = srv.start_cval.wait_for(lck, std::chrono::seconds(5), [] {
          return srv.listen_ready;
        });
        lck.unlock();

        if (!ready) {
          reportError("initConnection::listen timeout");
          abortWorker(true);
          std::lock_guard<std::mutex> lifeLck(srv.lifecycle_mtx);
          return srv.error;
        }

        std::lock_guard<std::mutex> lifeLck(srv.lifecycle_mtx);
        if (!srv.error.empty())
          return srv.error;
        if (!srv.running.load(std::memory_order_acquire))
          return "failed to start";
        return {};
      }

      startImpl(addr, port);
      std::lock_guard<std::mutex> lck(srv.lifecycle_mtx);
      return srv.error;
    }

    void stop() {
      bool sendAsync = false;
      bool needJoin = false;

      {
        std::lock_guard<std::mutex> lck(srv.lifecycle_mtx);
        if (srv.u_loop == nullptr && !srv.thr.joinable())
          return;

        if (srv.stopping.load()) {
          needJoin = srv.thr.joinable();
        } else if (srv.u_loop != nullptr && srv.running.load(std::memory_order_acquire)) {
          srv.stopping.store(true, std::memory_order_release);
          sendAsync = srv.stop_async_inited.load(std::memory_order_acquire);
          needJoin = srv.thr.joinable();
        } else if (srv.thr.joinable()) {
          srv.stopping.store(true, std::memory_order_release);
          sendAsync = srv.stop_async_inited.load(std::memory_order_acquire) && srv.u_loop != nullptr;
          needJoin = true;
        }
      }

      if (sendAsync)
        uv_async_send(&srv.stop_async);

      if (needJoin)
        joinWorkerThreadIfExternal();
    }
  }
}
