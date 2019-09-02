/**
 * Name: afmServerSocket.h
 * 
 * Copyright: 2019 AFM Software
 */

#ifndef _H_AFM_SERVER_SOCKET
#define _H_AFM_SERVER_SOCKET

#include <atomic>
#include <list>
#include <thread>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "socket.h"
#include "clientSocket.h"

namespace afm {
  namespace communications {

    using ClientSockets = std::list<ClientSocketSPtr>;

    class ServerSocket : public Socket
    {
    public:
        ServerSocket();

        virtual bool initialize(const SocketOptions &options) override;
        virtual void shutdown() override;

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