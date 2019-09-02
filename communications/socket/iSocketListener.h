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

            virtual void onConnected(const std::string &clientId) = 0;
            virtual void onDataReceived(const SocketBuffer &socketBuffer) = 0;
            virtual void onDataWritten(const SocketBuffer &socketBuffer) = 0;
            virtual void onError(int socketError) = 0;
            virtual void onDisconnected() = 0;
        };

        using iSocketListenerSPtr = std::shared_ptr<iSocketListener>; 
    }
}
#endif