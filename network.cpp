#include "network.h"

Network::Network() {
    if(SDLNet_Init()==-1) {
        printf("SDLNet_Init: %s\n", SDLNet_GetError());
        exit(2);
    }
    
    server = 0;
    client = 0;
}

Network::~Network()
{
    SDLNet_Quit();
}

void Network::getInternetList()
{
    IPaddress ip;
    TCPsocket tcpsock;

    printf("Get serverlist\n");
    const char *host = Config::getInstance().CFG_MASTER_SERVER;

    if(SDLNet_ResolveHost(&ip,host,80)==-1)
    {
        printf("SDLNet_ResolveHost: %s\n", SDLNet_GetError());
        exit(1);
    }

    tcpsock = SDLNet_TCP_Open(&ip);
    if(!tcpsock)
    {
        printf("SDLNet_TCP_Open: %s\n", SDLNet_GetError());
        exit(2);
    }

    int len,result;
    #define MAXLEN 1024
    char msg[MAXLEN];
    sprintf(msg,"GET / HTTP/1.1\r\nHost: %s\r\n\r\n",host);

    printf("Send header\n%s",msg);

    len = strlen(msg)+1; // add one for the terminating NULL
    result = SDLNet_TCP_Send(tcpsock,msg,len);
    if(result < len) {
        printf("SDLNet_TCP_Send: %s\n", SDLNet_GetError());
    }

    printf("Recieve header\n");

    result = SDLNet_TCP_Recv(tcpsock,msg,MAXLEN);
    if(result <= 0) {
        printf("SDLNet_TCP_Recv: %d",result);
    }
    printf("Received: \"%s\"\n",msg);

    SDLNet_TCP_Close(tcpsock);
}

void Network::startListen()
{
    /*listen=SDLNet_UDP_Open(Config::getInstance().CFG_PORT);
    if(!listen)
    {
        printf("SDLNet_UDP_Open: %s\n", SDLNet_GetError());
        exit(2);
    }*/
    if (server == 0) server = new Server();
    else printf("Van már server\n");
}

void Network::stopListen()
{
    /*SDLNet_UDP_Close(listen);
    listen=0;*/
    
    if (server != 0)
    {
        delete server;
        server = 0;
    }
}

void Network::connect(std::string iServerName)
{
    if (client == 0) client = new Client(iServerName);
    else printf("Van már client\n");
}

void Network::disconnect()
{
    /*SDLNet_UDP_Close(listen);
    listen=0;*/
    
    if (client != 0)
    {
        delete client;
        client = 0;
    }
}

void Network::mainLoop()
{
    if (client != 0) client->mainLoop();
    if (server != 0) server->mainLoop();
}

bool Network::sendPackets(std::vector<NetworkEvent> &packets)
{
    if (client != 0)
    {
        client->sendPackets(packets);
    }
    if (server != 0)
    {
        server->sendPackets(packets);
    }
    return true;
}

std::vector<NetworkEvent>& Network::getPackets()
{
    if (server != 0)
    {
        return server->getPackets();
    }
    if (client != 0)
    {
        return client->getPackets();
    }
    return empty;
}

Server& Network::getServer()
{
    return *server;
}

Client& Network::getClient()
{
    return *client;
}

bool Network::getSuccess() const
{
    if (client != 0)
    {
        return client->getSuccess();
    }
    if (server != 0)
    {
        return server->getSuccess();
    }
    return false;
}
