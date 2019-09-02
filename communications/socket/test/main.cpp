
#include <iostream>
#include "clientSocket.h"

using namespace afm;
using namespace communications;

int main(int argc, char *argv[])
{
    SocketOptions socketOptions = {
        {sc_socketURL, "ec2-18-191-138-192.us-east-2.compute.amazonaws.com"},
        {sc_socketPort, 1883}
    };

    ClientSocketSPtr clientSPtr = std::make_shared<ClientSocket>();

    if (clientSPtr->initialize(socketOptions) == true) {
        std::cout << "We have been initialized\n";
    } else {
        std::cout << "Failed to initialize.\n";
    }

    clientSPtr->shutdown();

    clientSPtr = nullptr;
    
    return 0;
}