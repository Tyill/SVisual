
#include "stdafx.h"
#include "SVAuxFunc/auxFunc.h"
#include "SVAuxFunc/clientTCP.h"

namespace SV_TcpClnt {

#ifdef WIN32

#include <winsock2.h>
#include <ws2tcpip.h>

/// Клиент TCP Win32
	class clientTCP {

	private:
		SOCKET socket_;        ///< сокет

	public:

		clientTCP() {

			socket_ = INVALID_SOCKET;
		}

		~clientTCP() {

		    if (socket_ != INVALID_SOCKET)
			   mdisconnect();
		}

		/// соединение
		/// \param addr IP адресс
		/// \param port порт
		/// \param noBlock не ждать ответа
		/// \return true - ok
		bool mconnect(std::string addr, int port, bool noBlock) {

			WSADATA wsaData;
			socket_ = INVALID_SOCKET;
			struct addrinfo *adInfo = NULL, hints;

			// Initialize Winsock
			int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (iResult != 0) {
				return false;
			}

			ZeroMemory(&hints, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			hints.ai_protocol = IPPROTO_TCP;

			// Resolve the server address and port
			char cport[11];
			sprintf(cport, "%d", port);
			iResult = getaddrinfo(addr.c_str(), cport, &hints, &adInfo);
			if (iResult != 0) {
				WSACleanup();
				return false;
			}

			socket_ = socket(AF_INET, SOCK_STREAM, 0);
			if (socket_ == INVALID_SOCKET) {
				WSACleanup();
				return false;
			}

			// Connect to server.
			iResult = connect(socket_, adInfo->ai_addr, (int) adInfo->ai_addrlen);
			if (iResult == SOCKET_ERROR) {
				closesocket(socket_);
				return false;
			}

			if (noBlock) {
				BOOL l = TRUE;
				int er = ioctlsocket(socket_, FIONBIO, (unsigned long *) &l);
			}

			return true;
		}

		/// отключение
		/// \return true - ok
		bool mdisconnect() {

			// cleanup
			shutdown(socket_, SD_BOTH);
			closesocket(socket_);
			WSACleanup();

			return true;
		}

		int sendall(const char *buf, int len, int flags)
        {
            int total = 0;
            while(total < len)
            {
                int n = send(socket_, buf+total, len-total, flags);
                if(n == SOCKET_ERROR) { total = SOCKET_ERROR; break; }
                total += n;
            }

            return total;
        }

		/// отправка данных
		/// \param in данные для передачи
		/// \param out ответ
		/// \param disconn отключать после отправки
		/// \param onlySend не ждать ответа
		/// \return true - ok
		bool sendMess(std::string& in, std::string &out, bool disconn, bool onlySend) {

			// Send an initial buffer
			int iResult = sendall(in.c_str(), in.size(), 0);
			if (iResult == SOCKET_ERROR) {
				mdisconnect();
				return false;
			}

			if (!onlySend) {

				// Receive until the peer closes the connection
				static const int recvbuflen = 2048;
				char recvbuf[recvbuflen];
				out = "";
				while (true) {
					iResult = recv(socket_, recvbuf, recvbuflen, 0);
					if (iResult > 0) {

						int sz = out.size();
						out.resize(sz + iResult);
						memcpy((char *) out.data() + sz, recvbuf, iResult);

						if (recvbuf[iResult - 1] == '\0') break;
					} else if (WSAGetLastError() == WSAEWOULDBLOCK) {
						Sleep(10);
					} else break;
				}
			}

			if (disconn) mdisconnect();

			return true;
	    }
	};

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>

    /// Клиент TCP Linux
    class clientTCP {

    private:
        int socket_;        ///< сокет

    public:

        clientTCP() {

            socket_ = -1;
        }

        ~clientTCP() {

           mdisconnect();
        }

        /// соединение
        /// \param addr IP адресс
        /// \param port порт
        /// \param noBlock не ждать ответа
        /// \return true - ok
        bool mconnect(std::string addr, int port, bool noBlock) {

            sockaddr_in saddr;

            socket_ = socket(AF_INET, SOCK_STREAM, 0);
            if(socket_ < 0)
                return false;

            saddr.sin_family = AF_INET;
            saddr.sin_port = htons(port);
            if (inet_aton(addr.c_str(), &saddr.sin_addr) == 0)
                return false;

            if(connect(socket_, (struct sockaddr *)&saddr, sizeof(saddr)) < 0)
                return false;

            if (noBlock)
                fcntl(socket_, F_SETFL, O_NONBLOCK);

            return true;
        }

        /// отключение
        /// \return true - ok
        bool mdisconnect() {

            if (socket_ > 0) close(socket_);
            return true;
        }

        int sendall(const char *buf, int len, int flags){

            int total = 0;

            while (total < len) {
                int n = send(socket_, buf + total, len - total, flags);
                if (n == -1) {
                    total = -1;
                    break;
                }
                total += n;
            }

            return total;
        }

        /// отправка данных
        /// \param in данные для передачи
        /// \param out ответ
        /// \param disconn отключать после отправки
        /// \param onlySend не ждать ответа
        /// \return true - ok
        bool sendMess(std::string& in, std::string &out, bool disconn, bool onlySend) {

            // Send an initial buffer
            int iResult = sendall(in.c_str(), in.size(), 0);
            if (iResult == -1) {
                mdisconnect();
                return false;
            }

            if (!onlySend) {

                // Receive until the peer closes the connection
                static const int recvbuflen = 2048;
                char recvbuf[recvbuflen];
                out = "";
                while (true) {
                    iResult = recv(socket_, recvbuf, recvbuflen, 0);
                    if (iResult > 0) {

                        int sz = out.size();
                        out.resize(sz + iResult);
                        memcpy((char *) out.data() + sz, recvbuf, iResult);

                        if (recvbuf[iResult - 1] == '\0') break;
                    } else if (iResult == -1) {
                       SV_Aux::SleepMs(10);
                    } else break;
                }
            }

            if (disconn) mdisconnect();

            return true;
        }
    };

#endif

/// Клиент TCP
	clientTCP client;

/// клиент TCP подключение
/// \param addr IP адресс
/// \param port порт
/// \param noBlock Не ждать ответа
/// \return true - ok
	bool connect(std::string addr, int port, bool noBlock) {

		return client.mconnect(addr, port, noBlock);
	}

/// клиент TCP отключение
/// \return true - ok
	bool disconnect() {

		return client.mdisconnect();
	}

/// клиент Отправить данные
/// \param in данные
/// \param out ответ
/// \param disconn отключать после отправки
/// \param onlySend только отправка
/// \return true - ok
	bool sendData(std::string in, std::string &out, bool disconn, bool onlySend) {

		return client.sendMess(in, out, disconn, onlySend);
	}
}
