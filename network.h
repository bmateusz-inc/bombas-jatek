#ifndef NETWORK_H
#define NETWORK_H

#include <SDL/SDL_net.h>
#include <vector>

#include "config.h"

#include "server.h"
#include "client.h"
#include "gametable.h"
#include "networkevent.h"

/** Helper class to handle Server and Client together */
class Network
{
    private:
        /** Server instance */
        Server* server;
        
        /** Client instance*/
        Client* client;
        
        /** Link to GameTable */
        GameTable* table;
        
        /** Just for safety reasons */
        std::vector<NetworkEvent> empty;

    public:
        /** Constructor */
        Network();
        
        /** Destructor */
        ~Network();
        
        /** Get list of Internet servers */
        void getInternetList();
        
        /** Get lost of servers on local network */
        void getLocalList();
        
        /** Start server */
        void startListen();
        
        /** Stop server */
        void stopListen();
        
        /** Connect to server */
        void connect(std::string iServerName);
        
        /** Disconnect from server */
        void disconnect();
        
        /** Send packets where it is necessary */
        bool sendPackets(std::vector<NetworkEvent> &packets);
        
        /** Send packets where it is necessary */
        std::vector<NetworkEvent>& getPackets();
        
        /** Main loop, for doing the work */
        void mainLoop();
        
        /** Get server class */
        Server& getServer();
        
        /** Get client class */
        Client& getClient();
        
        /** False if error occured */
        bool getSuccess() const;
};

#endif
