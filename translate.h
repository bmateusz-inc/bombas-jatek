#ifndef TRANSLATE_H
#define	TRANSLATE_H

#include <SDL/SDL_net.h>
#include <sstream>
#include <string>

#include "config.h"
#include "networkevent.h"

#include <vector>

/** Static class, helper for network packet handling */
class Translate
{
    private:
        static const float factor = 100;
    public:
        /** Make a single event from packet */
        static NetworkEvent translate(const char* buffer);
        
        /**
         * Make network events from packet received
         * @param packets output
         * @param buffer input
         * @param bytes input, number of bytes received
         */
        static void translate(std::vector<NetworkEvent>& packets,
                              const char* buffer,
                              int bytes);
        
        /**
         * Make sendable packet from network event
         * @param event input
         * @param buffer output
         */
        static void pack(const NetworkEvent &event, char* buffer);
        
        /** Packet to string, for trace */
        static std::string packetToString(const char* buffer);
        
        /**
         * Split a char into two chars
         * @param chr input
         * @param first lower of higher bytes
         * @return 
         */
        static char slice(char chr, bool first);
};

#endif
