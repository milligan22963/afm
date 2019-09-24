/**
 * Name: iServerSocketListener.h
 * 
 * Copyright: 2019 AFM Software
 */

#ifndef _H_ISERVER_SOCKET_LISTENER
#define _H_ISERVER_SOCKET_LISTENER

#include <memory>

#include "iSocket.h"
#include "socketDefines.h"

namespace afm {
    namespace communications {
        class iServerSocketListener
        {
        public:
            virtual ~iServerSocketListener()
            {
            }

            virtual void onClientConnected(iSocketSPtr pClient) = 0;
        };

        using iServerSocketListenerSPtr = std::shared_ptr<iServerSocketListener>; 

    }
}
#endif