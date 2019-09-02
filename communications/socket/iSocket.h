/**
 * Name: IAfmSocket.h
 * 
 * Copyright: 2019 AFM Software
 */

#ifndef _H_IAFM_SOCKET
#define _H_IAFM_SOCKET

#include <json.hpp>

#include <atomic>
#include <cstdint>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "socketDefines.h"
#include "iSocketListener.h"

namespace afm {
    namespace communications {
        class iSocket
        {
        public:
            virtual ~iSocket()
            {

            }

            virtual bool initialize(const SocketOptions &options) = 0;
            virtual void shutdown() = 0;

            virtual void addListener(iSocketListenerSPtr pSocketListener) = 0;
            virtual void removeListener(iSocketListenerSPtr pSocketListener) = 0;

            virtual const std::string &getSocketClientId() const = 0;

            virtual bool write(const SocketBuffer &data) = 0;
            virtual bool write(const std::string &socketClientId, const SocketBuffer &data) = 0;

            virtual bool read(SocketBuffer &data) = 0;
            virtual bool readWait(SocketBuffer &data, uint32_t milliseconds) = 0;

            virtual bool transfer(const SocketBuffer &dataOut, SocketBuffer &dataIn) = 0;
            virtual bool transfer(const std::string &socketClientId, const SocketBuffer &dataOut, SocketBuffer &dataIn) = 0;
            virtual bool transferWait(const SocketBuffer &dataOut, SocketBuffer &dataIn, uint32_t milliseconds) = 0;
            virtual bool transferWait(const std::string &socketClientId, const SocketBuffer &dataOut, SocketBuffer &dataIn, uint32_t milliseconds) = 0;
        };

        using iSocketListeners = std::list<iSocketListenerSPtr>;
        using iSocketSPtr = std::shared_ptr<iSocket>;
    }
}
#endif