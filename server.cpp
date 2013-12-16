#include "server.h"
#include "translate.h"

Server::Server()
{
    tr.open("server.txt");
    clientCount = 0;
    port = Config::getInstance().CFG_PORT;
    success = true;

    socketSet = SDLNet_AllocSocketSet(MAX_SOCKETS);
    if (socketSet == NULL)
    {
        tr << "Failed to allocate the socket set: " << SDLNet_GetError() << std::endl;
        success = false;
    }
    else
    {
        tr << "Allocated socket set with size:  " <<
                MAX_SOCKETS << ", of which " << MAX_CLIENTS << " are availble for use by clients."
                <<  std::endl;
    }

    for (int i = 0; i < MAX_CLIENTS; ++i)
    {
        clientSocket[i] = NULL;
        socketIsFree[i] = true;
    }

    int hostResolved = SDLNet_ResolveHost(&serverIP, NULL, port);

    if (hostResolved == -1)
    {
        tr << "Failed to resolve the server host: " << SDLNet_GetError() << std::endl;
        success = false;
    }
    else
    {
        Uint8 * dotQuad = (Uint8*)&serverIP.host;

        tr << "Successfully resolved server host to IP: " <<
                (unsigned short)dotQuad[0] << "." << (unsigned short)dotQuad[1]
                << "." << (unsigned short)dotQuad[2] << "." << (unsigned short)dotQuad[3];
        tr << " port " << SDLNet_Read16(&serverIP.port) << std::endl << std::endl;
    }

    serverSocket = SDLNet_TCP_Open(&serverIP);

    if (!serverSocket)
    {
        tr << "Failed to open the server socket: " << SDLNet_GetError() << std::endl;
        success = false;
    }
    else
    {
        tr << "Sucessfully created server socket." << std::endl;
        SDLNet_TCP_AddSocket(socketSet, serverSocket);
        tr << "Awaiting clients..." << std::endl;
    }
}

Server::~Server()
{
    tr.close();
    SDLNet_FreeSocketSet(socketSet);
    SDLNet_TCP_Close(serverSocket);
}

void Server::mainLoop()
{
    /* Handle server socket */
    SDLNet_CheckSockets(socketSet, 0);
    
    int serverSocketActivity = SDLNet_SocketReady(serverSocket);

    if (serverSocketActivity != 0)
    {
        if (clientCount < MAX_CLIENTS)
        {
            int freeSpot = -1;
            for (int loop = 0; loop < MAX_CLIENTS; loop++)
            {
                if (socketIsFree[loop] == true)
                {
                    socketIsFree[loop] = false;
                    freeSpot = loop;
                    break;
                }
            }

            if (freeSpot != -1)
            {
                clientSocket[freeSpot] = SDLNet_TCP_Accept(serverSocket);

                SDLNet_TCP_AddSocket(socketSet, clientSocket[freeSpot]);

                clientCount++;
                
                greetPlayer(freeSpot);

                tr << "Client connected to " << freeSpot << ". There are now " << clientCount
                        << " client(s) connected." << std::endl << std::endl;
            }
        }
        else
        {
            tr << "*** Maximum client count reached - rejecting client connection ***" << std::endl;

            TCPsocket tempSock = SDLNet_TCP_Accept(serverSocket);

            strcpy( buffer, (char*)"FULL" );
            int msgLength = strlen(buffer) + 1;
            SDLNet_TCP_Send(tempSock, (void *)buffer, msgLength);

            SDLNet_TCP_Close(tempSock);
        }
    }

    /* Handle client sockets */
    
    for (int clientNumber = 0; clientNumber < MAX_CLIENTS; clientNumber++)
    {
        int active = 1;
        while (active)
        {
            SDLNet_CheckSockets(socketSet, 0);

            active = SDLNet_SocketReady(clientSocket[clientNumber]);
            if (active != 0)
            {
                int bytes = SDLNet_TCP_Recv(clientSocket[clientNumber], buffer, BUFFER_SIZE);

                if (bytes <= 0)
                {
                    tr << "Client " << clientNumber << " disconnected." << std::endl << std::endl;

                    SDLNet_TCP_DelSocket(socketSet, clientSocket[clientNumber]);
                    SDLNet_TCP_Close(clientSocket[clientNumber]);
                    clientSocket[clientNumber] = NULL;

                    socketIsFree[clientNumber] = true;

                    clientCount--;

                    tr << "Server is now connected to: " << clientCount << " client(s)." << std::endl << std::endl;

                    sendNPlayers();
                }
                else
                {
                    tr << "Received: " << bytes << " bytes >>>> " << Translate::packetToString(buffer) << " from client number: " << clientNumber << std::endl;
                    Translate::translate(packets, buffer, bytes);
                }
            }
        }
    }
}

std::vector<NetworkEvent>& Server::getPackets()
{
    return packets;
}

int Server::getNPlayers() const
{
    return clientCount + 1;
}

void Server::greetPlayer(char id)
{
    /* Player's ID is incremented by one, 0 is the server! */
    Translate::pack(NetworkEvent(SET_NETWORK_ID, id + 1, 0, 0), buffer);
    SDLNet_TCP_Send(clientSocket[(int)id], buffer, NetworkEvent::packetSize);
    sendNPlayers();
}

void Server::sendNPlayers()
{
    Translate::pack(NetworkEvent(SET_NPLAYERS, clientCount + 1, 0, 0), buffer);
    for (int loop = 0; loop < MAX_CLIENTS; loop++)
        if (socketIsFree[loop] == false)
            SDLNet_TCP_Send(clientSocket[loop], buffer, NetworkEvent::packetSize);
}

bool Server::sendPackets(std::vector<NetworkEvent>& packetsOut)
{
    char data[BUFFER_SIZE*4];
    
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (socketIsFree[i] == false)
        {
            for (std::vector<NetworkEvent>::const_iterator it = packetsOut.begin(); it != packetsOut.end(); ++it)
            {
                bool noSelfTransmit = (it->e == MOVEMENT || it->e == PUT_BOMB || it->e == SET_TEAM);
                if (!noSelfTransmit || (noSelfTransmit && it->id - 1 != i))
                {
                    Translate::pack(*it, data);
                    tr << "Retransmitting message: " <<
                            Translate::packetToString(data) << " to client number: " << i << std::endl;
                    SDLNet_TCP_Send(clientSocket[i], data, NetworkEvent::packetSize);
                } else tr << "no transmit to " << i << "\n";
            }
        }
    }
    //packetsOut.clear();
    return true;
}

bool Server::getSuccess() const
{
    return success;
}
