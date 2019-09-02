/**
 * Name: afmClientSocket.cpp
 * 
 * Copyright: 2019 AFM Software
 */

#include <memory.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "clientSocket.h"

namespace afm {
    namespace communications {

        void ClientSocket::setClientSocket(int32_t socketHandle)
        {
            setSocketHandle(socketHandle); // already have a socket
        }

        std::thread ClientSocket::createProcessingThread()
        {
            return std::thread(&ClientSocket::read_processing, shared_from_base<ClientSocket>());
        }
    }
}
