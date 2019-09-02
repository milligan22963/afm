/**
 * @brief afmSocketFactory
 * 
 * Class for creating socket classes
 * 
 */

#include "socketFactory.h"
#include "clientSocket.h"
#include "serverSocket.h"

namespace afm {
    namespace communications {
        SocketFactory::SocketFactory()
        {

        }

        SocketFactory::~SocketFactory()
        {

        }
        
        SocketFactorySPtr SocketFactory::getInstance()
        {
            static SocketFactorySPtr pFactory = nullptr;

            if (pFactory == nullptr) {
                pFactory = std::make_shared<SocketFactory>();
            }
            return pFactory;
        }

        iSocketSPtr SocketFactory::createSocket(eSocketType socketType)
        {
            iSocketSPtr pSocket = nullptr;

            switch (socketType) {
                case eSocketType::eClientSocket: 
                {
                    pSocket = std::make_shared<ClientSocket>();
                }
                break;
                case eSocketType::eServerSocket: 
                {
                    pSocket = std::make_shared<ServerSocket>();
                }
                default:
                {
                    pSocket = nullptr;
                }
            }
            return pSocket;
        }
    }
}
