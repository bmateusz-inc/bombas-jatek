#include "client.h"

Client::Client(std::string iServerName) : serverName(iServerName), success(true)
{
    tr.open("client.txt");
    socketSet = SDLNet_AllocSocketSet(1);
    port = Config::getInstance().CFG_PORT;
    
    if (socketSet == NULL)
    {
        tr << "Failed to allocate the socket set: " << SDLNet_GetError() << "\n";
        success = false;
    }
    else
    {
        tr << "Successfully allocated socket set." << std::endl;
    }

    int hostResolved = SDLNet_ResolveHost(&serverIP, serverName.c_str(), port);

    if (hostResolved == -1)
    {
        tr << "Failed to resolve the server hostname: " << SDLNet_GetError() << "\nContinuing...\n";
        success = false;
    }
    else
    {
        Uint8 * dotQuad = (Uint8*)&serverIP.host;
        tr << "Successfully resolved host to IP: " << (unsigned short)dotQuad[0] << "." << (unsigned short)dotQuad[1] <<
                "." << (unsigned short)dotQuad[2] << "." << (unsigned short)dotQuad[3]
                << " port " << SDLNet_Read16(&serverIP.port) << std::endl << std::endl;
    }

    clientSocket = SDLNet_TCP_Open(&serverIP);
    if (!clientSocket)
    {
        tr << "Failed to open socket to server: " << SDLNet_GetError() << "\n";
        success = false;
    }
    else
    {
        tr << "Connection okay, about to read connection status from the server..." << std::endl;

        SDLNet_TCP_AddSocket(socketSet, clientSocket);
    }
}

Client::~Client()
{
    tr.close();
    SDLNet_FreeSocketSet(socketSet);
    SDLNet_TCP_Close(clientSocket);
}

void Client::mainLoop()
{
    // int activeSockets = 

    // tr << "There are " << activeSockets << " socket(s) with data on them at the moment." << std::endl;

    int active = 1;
    while (active)
    {
        SDLNet_CheckSockets(socketSet, 0);

        active = SDLNet_SocketReady(clientSocket);
        if (active != 0)
        {
            int bytes = SDLNet_TCP_Recv(clientSocket, buffer, BUFFER_SIZE);

            tr << "Got the following from server: " << Translate::packetToString(buffer) << "(" << bytes << " bytes)" << std::endl;
            Translate::translate(packets, buffer, bytes);
            
            if (bytes == -1)
            {
                success = false;
                active = 0;
            }
        }
    }
}

bool Client::sendPackets(std::vector<NetworkEvent>& packetsOut)
{
    char data[BUFFER_SIZE*4];
    for (std::vector<NetworkEvent>::const_iterator it = packetsOut.begin(); it != packetsOut.end(); ++it)
    {
        Translate::pack(*it, data);
        tr << "Packed " << Translate::packetToString(data) << std::endl;
        SDLNet_TCP_Send(clientSocket, data, NetworkEvent::packetSize);
    }
    packetsOut.clear();
    return true;
}

std::vector<NetworkEvent>& Client::getPackets()
{
    return packets;
}

bool Client::getSuccess() const
{
    return success;
}
