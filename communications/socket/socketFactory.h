/**
 * @brief afmSocketFactory
 * 
 * Class for creating socket classes
 * 
 */

#ifndef _H_AFM_SOCKET_FACTORY
#define _H_AFM_SOCKET_FACTORY

#include <memory>
#include "iSocket.h"

namespace afm {
    namespace communications {
        
        enum eSocketType {
            eClientSocket,
            eServerSocket,
            eEndSocketTypes
        };

        class SocketFactory;

        using SocketFactorySPtr = std::shared_ptr<SocketFactory>;

        class SocketFactory
        {
            public:
                SocketFactory();
                virtual ~SocketFactory();
                
                static SocketFactorySPtr getInstance();

                iSocketSPtr createSocket(eSocketType socketType);
        };
    }
}
#endif