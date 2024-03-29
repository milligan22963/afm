/**
 * Name: afmServerSocket.cpp
 * 
 * Copyright: 2019 AFM Software
 */

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "serverSocket.h"

namespace afm {
    namespace communications {
        ServerSocket::ServerSocket()
            : Socket()
        {

        }

        bool ServerSocket::initialize(const SocketOptions &options)
        {
            bool success = false;

            if (Socket::initialize(options) == true) {
                int socketOption = 1;

                m_serverAddress.sin_family = AF_INET;
                m_serverAddress.sin_addr.s_addr = INADDR_ANY;
                m_serverAddress.sin_port = htons(getPort());

                if (setsockopt(getSocketHandle(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socketOption, sizeof(socketOption)) == 0) {
                    if (bind(getSocketHandle(), (struct sockaddr *)&m_serverAddress, sizeof(m_serverAddress)) == 0) {

                        if (listen(getSocketHandle(), 20) == 0) {
                            m_threadRunning = true;
                            m_processingThread = std::thread(&ServerSocket::processing, this);
                            success = true;
                        }
                    }
                }
            }
            return success;
        }

        void ServerSocket::shutdown()
        {
            m_clients.clear();

            if (m_threadRunning == true) {
                m_threadRunning = false;
                m_processingThread.join();
            }

            Socket::shutdown();
        }

        bool ServerSocket::write(const std::string &socketClientId, const SocketBuffer &data)
        {
            bool success = false;

            ClientSockets::iterator iter = m_clients.find(socketClientId);
            if (iter != m_clients.end()) {
                success = iter->second->write(data);
            }
            return success;
        }

        bool ServerSocket::transfer(const std::string &socketClientId, const SocketBuffer &dataOut, SocketBuffer &dataIn)
        {
            bool success = false;
            
            ClientSockets::iterator iter = m_clients.find(socketClientId);
            if (iter != m_clients.end()) {
                success = iter->second->transfer(dataOut, dataIn);
            }
            return success;
        }

        bool ServerSocket::transferWait(const std::string &socketClientId, const SocketBuffer &dataOut, SocketBuffer &dataIn, uint32_t milliseconds)
        {
            bool success = false;
            
            ClientSockets::iterator iter = m_clients.find(socketClientId);
            if (iter != m_clients.end()) {
                success = iter->second->transferWait(dataOut, dataIn, milliseconds);
            }
            return success;
        }

        void ServerSocket::processing()
        {
            struct sockaddr_in  clientAddress;
            socklen_t           addressSize = sizeof(clientAddress);

            while (m_threadRunning == true) {
                uint16_t client = accept(getSocketHandle(), (struct sockaddr *)&clientAddress, &addressSize);
                if (client != sc_closedSocket) {
                    // spawn it
                    ClientSocketSPtr pSocket = std::make_shared<ClientSocket>();

                    for (auto listener : getSocketListeners()) {
                        pSocket->addListener(listener);
                    }
                    pSocket->setClientSocket(client);
                    m_clients[pSocket->getSocketClientId()] = pSocket;
                }
                sleep(1); // pause a bit
            }
        }

        void ServerSocket::socketFailure()
        {
            Socket::socketFailure();

            if (m_threadRunning == true) {
                m_threadRunning = false;
                m_processingThread.join();
            }

            int socketOption = 1;

            if (setsockopt(getSocketHandle(), SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &socketOption, sizeof(socketOption)) == 0) {
                if (bind(getSocketHandle(), (struct sockaddr *)&m_serverAddress, sizeof(m_serverAddress)) == 0) {

                    if (listen(getSocketHandle(), 20) == 0) {
                        m_threadRunning = true;
                        m_processingThread = std::thread(&ServerSocket::processing, this);
                    }
                }
            }
        }
    }
}
