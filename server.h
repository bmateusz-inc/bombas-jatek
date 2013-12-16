#ifndef SERVER_H
#define SERVER_H

#include <SDL/SDL_net.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "networkevent.h"
#include "config.h"

/** Server connection and packet handling */
class Server
{
    private:
        std::ofstream tr;

        IPaddress serverIP;
        TCPsocket serverSocket;
        TCPsocket clientSocket[MAX_CLIENTS];
        SDLNet_SocketSet socketSet;
        bool socketIsFree[MAX_CLIENTS];
        int clientCount;
        bool success;
        
        char buffer[BUFFER_SIZE];
        
        std::vector<NetworkEvent> packets;

        unsigned int port;
        
        void greetPlayer(char id);
        
        void sendNPlayers();

    public:
        /** Constructor */
        Server();
        
        /** Destructor */
        ~Server();
        
        /** Main loop of server, receives packets and accepts new clients */
        void mainLoop();
        
        /** Get packets from clients */
        std::vector<NetworkEvent>& getPackets();
        
        /** Send and retransmit packets to clients */
        bool sendPackets(std::vector<NetworkEvent>& packetsOut);
        
        /** Number of players */
        int getNPlayers() const;
        
        /** False if error occured */
        bool getSuccess() const;
};

#endif