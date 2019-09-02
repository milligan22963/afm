/**
 * Name: afmSocket.cpp
 * 
 * Copyright: 2019 AFM Software
 */

#include <iostream>
#include <errno.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "JSONUtil.h"

#include "socket.h"

namespace afm {
    namespace communications {
        Socket::Socket()
            : m_threadRunning(false)
            , m_socketConnected(false)
            , m_reconnect(true)
        {
        }

        Socket::~Socket()
        {
            shutdown();
        }

        bool Socket::initialize(const SocketOptions &options)
        {
            bool success = false;
            
            if (extractValue(options, sc_socketURL, m_url) == true) {
                if (extractValue(options, sc_socketPort, m_port) == true) {
                    m_socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
                    if (m_socketHandle != sc_closedSocket) {
                        success = true;
                    }
                }
            }

            return success;
        }

        void Socket::shutdown()
        {
            if (m_threadRunning == true) {
                m_threadRunning = false;
                m_processingThread.join();
            }

            for (auto listener : m_socketListeners) {
                listener->onDisconnected(m_socketClientId);
            }
            m_socketListeners.clear();

            if (m_socketHandle != sc_closedSocket) {
                ::close(m_socketHandle);
                m_socketHandle = sc_closedSocket;
            }
        }

        void Socket::addListener(iSocketListenerSPtr pSocketListener)
        {
            m_socketListeners.push_back(pSocketListener);
            if (m_threadRunning == false) {
                m_threadRunning = true;
                m_processingThread = createProcessingThread();
            }
        }

        void Socket::removeListener(iSocketListenerSPtr pSocketListener)
        {
            m_socketListeners.remove(pSocketListener);
            if (m_socketListeners.size() == 0) {
                if (m_threadRunning == true) {
                    m_threadRunning = false;
                    m_processingThread.join();
                }
            }
        }

        bool Socket::write(const SocketBuffer &data)
        {
            bool success = true;
            if (::write(m_socketHandle, data.data(), data.size()) > 0) {
                for (auto listener : m_socketListeners) {
                    listener->onDataWritten(m_socketClientId, data);
                }
            } else {
                socketFailure();
                success = false;
            }

            return success;
        }

        bool Socket::write(const std::string &socketClientId, const SocketBuffer &data)
        {
            bool success = false;

            if (socketClientId == m_socketClientId) {
                success = write(data);
            }
            return success;
        }

        bool Socket::read(SocketBuffer &buffer)
        {
            ssize_t bytesRead = 0;

            buffer.clear();
            
            uint8_t data_buffer[1024];
            bytesRead = ::read(m_socketHandle, data_buffer, 1023);
            if (bytesRead > 0) {
                buffer.reserve(bytesRead);
                std::copy(data_buffer, data_buffer + bytesRead, std::back_inserter(buffer));
            } else if (bytesRead == -1) {
                socketFailure();
            }
            return bytesRead != 0;
        }

        bool Socket::readWait(SocketBuffer &buffer, uint32_t milliseconds)
        {
            ssize_t bytesRead = 0;
            time_t seconds = milliseconds / 1000;
            suseconds_t microseconds = (milliseconds - (seconds * 1000)) * 1000;

            struct timeval tv = {seconds, microseconds};
            fd_set readDescriptorSet;
            fd_set exceptionDescriptorSet;
            
            FD_ZERO(&readDescriptorSet);
            FD_ZERO(&exceptionDescriptorSet);

            FD_SET(m_socketHandle, &readDescriptorSet);
            FD_SET(m_socketHandle, &exceptionDescriptorSet);

            if (select(m_socketHandle + 1, &readDescriptorSet, nullptr, &exceptionDescriptorSet, &tv) != -1) {
                if (FD_ISSET(m_socketHandle, &readDescriptorSet)) {
                    bytesRead = read(buffer);
                } else if (FD_ISSET(m_socketHandle, &exceptionDescriptorSet)) {
                    socketFailure();
                }
            } else {
                socketFailure();
            }
            return bytesRead != 0;
        }

        bool Socket::transfer(const SocketBuffer &dataOut, SocketBuffer &dataIn)
        {
            write(dataOut);

            return read(dataIn);
        }

        bool Socket::transfer(const std::string &socketClientId, const SocketBuffer &dataOut, SocketBuffer &dataIn)
        {
            bool success = false;

            if (socketClientId == m_socketClientId) {
                success = transfer(dataOut, dataIn);
            }
            return success;
        }

        bool Socket::transferWait(const SocketBuffer &dataOut, SocketBuffer &dataIn, uint32_t milliseconds)
        {
            bool success = false;

            if (write(dataOut) == true) {
                success = readWait(dataIn, milliseconds);
            }
            return success;
        }

        bool Socket::transferWait(const std::string &socketClientId, const SocketBuffer &dataOut, SocketBuffer &dataIn, uint32_t milliseconds)
        {
            bool success = false;

            if (socketClientId == m_socketClientId) {
                success = transferWait(dataOut, dataIn, milliseconds);
            }
            return success;;
        }

        bool Socket::connect()
        {
            bool success = false;
            struct sockaddr_in hostAddress;

            hostAddress.sin_family = AF_INET;
            hostAddress.sin_port = htons(m_port);

            if (inet_pton(AF_INET, getUrlAddress().c_str(), &hostAddress.sin_addr) > 0) {
                if (::connect(m_socketHandle, (struct sockaddr *)&hostAddress, sizeof(hostAddress)) >= 0) {
                    socklen_t addrLength = sizeof(m_socketAddress);
                    ::getsockname(m_socketHandle, (struct sockaddr *)&m_socketAddress, &addrLength);

                    char buffer[128] = {0};
                    if (inet_ntop(AF_INET, &m_socketAddress, buffer, sizeof(m_socketAddress)) != nullptr) {
                        int bufferLength = strlen(buffer);

                        snprintf(&buffer[bufferLength], 127 - bufferLength, ":%u", m_socketAddress.sin_port);
                    }
                    m_socketClientId = buffer;
                    success = true;
                    for (auto listener : m_socketListeners) {
                        listener->onConnected(m_socketClientId);
                    }
                }
            }

            return success;
        }

        std::string Socket::getUrlAddress()
        {
            std::string hostAddress;
            struct hostent *pHostEntry = nullptr;
            struct in_addr **ppAddressList = nullptr;
                
            if ((pHostEntry = gethostbyname(m_url.c_str())) != nullptr) {
                ppAddressList = (struct in_addr **)pHostEntry->h_addr_list;
                for (int addressIndex = 0; ppAddressList[addressIndex] != nullptr; addressIndex++) 
                {
                    char buffer[32];
                    strncpy(buffer, inet_ntoa(*ppAddressList[addressIndex]), 31);
                    hostAddress = buffer;
                    break;
                }
            }
            return hostAddress;
        }

        std::thread Socket::createProcessingThread()
        {
            return std::thread(&Socket::read_processing, shared_from_this());
        }

        void Socket::socketFailure()
        {
            m_socketConnected = false;

            // may need to do this prior to this call
            m_lastError = errno;

            for (auto listener : m_socketListeners) {
                listener->onError(m_socketClientId, m_lastError);
            }

            // close it - assumed it was open but just in case
            if (m_socketHandle != sc_closedSocket) {
                ::close(m_socketHandle);
            }
            // grab a new one since the other had issue(s)
            m_socketHandle = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
        }

        void Socket::setSocketHandle(int32_t socketHandle)
        {
            m_socketHandle = socketHandle;
            m_socketConnected = true;
            m_reconnect = false;

            socklen_t addrLength = sizeof(m_socketAddress);

            ::getsockname(m_socketHandle, (struct sockaddr *)&m_socketAddress, &addrLength);
            char buffer[128] = {0};
            if (inet_ntop(AF_INET, &m_socketAddress, buffer, sizeof(m_socketAddress)) != nullptr) {
                int bufferLength = strlen(buffer);

                snprintf(&buffer[bufferLength], 127 - bufferLength, ":%u", m_socketAddress.sin_port);
            }
            m_socketClientId = buffer;
            for (auto listener : m_socketListeners) {
                listener->onConnected(m_socketClientId);
            }
        }

        void Socket::read_processing()
        {
            SocketBuffer incomingData;

            while (m_threadRunning == true) {
                if (m_socketConnected == true) {
                    if (readWait(incomingData, 1000) > 0) {
                        for (auto listener : m_socketListeners) {
                            listener->onDataReceived(m_socketClientId, incomingData);
                        }
                    }
                } else if (m_reconnect == true) {
                    m_socketConnected = connect();

                    // if it fails let it wait for a bit
                    if (m_socketConnected == false) {
                        for (int count = 0; count < sm_connectionDelay; count++) {
                            sleep(1); // so we don't hammer the connection
                        }
                    }
                } else {
                    // done
                    socketFailure();
                }
            }
        }
    }
}
