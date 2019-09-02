/**
 * Name: afmClientSocket.h
 * 
 * Copyright: 2019 AFM Software
 */

#ifndef _H_AFM_CLIENT_SOCKET
#define _H_AFM_CLIENT_SOCKET

#include "socket.h"

namespace afm {
  namespace communications {

    class ClientSocket : public Socket
    {
    public:

        // allows a server to create client sockets
        // should be called after listeners added
        void setClientSocket(int32_t socketHandle);

        protected:
          /**
           * @copydoc AfmSocket::createProcessingThread
           */
          virtual std::thread createProcessingThread() override;

        private:
    };

    using ClientSocketSPtr = std::shared_ptr<ClientSocket>; 
  }
}
#endif