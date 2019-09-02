/**
 * Name: afmServerSocket.h
 * 
 * Copyright: 2019 AFM Software
 */

#ifndef _H_AFM_SERVER_SOCKET
#define _H_AFM_SERVER_SOCKET

#include <atomic>
#include <list>
#include <map>
#include <string>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "socket.h"
#include "clientSocket.h"

namespace afm {
  namespace communications {

    using ClientSockets = std::map<std::string, ClientSocketSPtr>;

    class ServerSocket : public Socket
    {
    public:
        ServerSocket();

        virtual bool initialize(const SocketOptions &options) override;
        virtual void shutdown() override;

        virtual bool write(const std::string &socketClientId, const SocketBuffer &data) override;
        virtual bool transfer(const std::string &socketClientId, const SocketBuffer &dataOut, SocketBuffer &dataIn) override;
        virtual bool transferWait(const std::string &socketClientId, const SocketBuffer &dataOut, SocketBuffer &dataIn, uint32_t milliseconds) override;

    protected:
        void processing();
        virtual void socketFailure() override;

    private:
        std::atomic<bool>   m_threadRunning;
        std::thread         m_processingThread;
        struct sockaddr_in  m_serverAddress;
        ClientSockets       m_clients;
    };

    using ServerSocketSPtr = std::shared_ptr<ServerSocket>; 
  }
}
#endif