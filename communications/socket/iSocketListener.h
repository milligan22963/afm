/**
 * Name: ISocketListener.h
 * 
 * Copyright: 2019 AFM Software
 */

#ifndef _H_ISOCKET_LISTENER
#define _H_ISOCKET_LISTENER

#include <memory>
#include <string>

#include "socketDefines.h"

namespace afm {
    namespace communications {

        class iSocketListener
        {
        public:
            virtual ~iSocketListener()
            {
            }

            virtual void onConnected(const std::string &clientSocketId) = 0;
            virtual void onDataReceived(const std::string &clientSocketId, const SocketBuffer &socketBuffer) = 0;
            virtual void onDataWritten(const std::string &clientSocketId, const SocketBuffer &socketBuffer) = 0;
            virtual void onError(const std::string &clientSocketId, int socketError) = 0;
            virtual void onDisconnected(const std::string &clientSocketId) = 0;
        };

        using iSocketListenerSPtr = std::shared_ptr<iSocketListener>; 
    }
}
#endif