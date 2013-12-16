#ifndef CLIENT_H
#define	CLIENT_H

#include <SDL/SDL_net.h>

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "config.h"
#include "networkevent.h"
#include "translate.h"

/** Client connection and packet handling */
class Client
{
    private:
        std::ofstream tr;

        IPaddress serverIP;
        TCPsocket clientSocket;
        SDLNet_SocketSet socketSet;
        std::string serverName;
        unsigned int port;
        char buffer[BUFFER_SIZE];
        char playerId;
        bool success;
        
        std::vector<NetworkEvent> packets;

    public:
        /**
         * Constructor
         * @param iServerName address to connect (can be IP or host name)
         */
        Client(std::string iServerName);
        
        /** Desctructor */
        ~Client();
        
        /** Main loop, receive packets */
        void mainLoop();
        
        /** Send packets to server */
        bool sendPackets(std::vector<NetworkEvent>& packetsOut);
        
        /** Get packets (after mainLoop run) */
        std::vector<NetworkEvent>& getPackets();
        
        /** False if error occured. */
        bool getSuccess() const;
        
        /** Get the address we connected to */
        std::string getHost() const { return serverName; }
};

#endif
