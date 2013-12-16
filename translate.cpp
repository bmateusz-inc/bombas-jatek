#include "translate.h"

char Translate::slice(char chr, bool first)
{
    if (first)
    {
        return (chr & 0x0F);
    }
    else
    {
        return ((chr & 0xF0) >> 4);
    }
}

NetworkEvent Translate::translate(const char* buffer)
{
    NetworkEvent e;

    e.e = buffer[0];

    e.id = buffer[1];

    Uint16 temp;

    temp = SDLNet_Read16(const_cast<char*>(buffer)+2);
    e.x = (float)temp / factor;

    temp = SDLNet_Read16(const_cast<char*>(buffer)+4);
    e.y = (float)temp / factor;

    return e;
}

void Translate::translate(std::vector<NetworkEvent>& packets,
                          const char* buffer,
                          int bytes)
{
    int i = 0;
    const int packetSize = NetworkEvent::packetSize;
    
    /* Handle partial packet */
    /*if (partialPacket.size() != 0)
    {
        int nextSize = packetSize - partialPacket.size();
        if (nextSize > bytes)
        {
            partialPacket.append(buffer);
            i += bytes;
            ptr += bytes;
        }
        else
        {
            partialPacket.append(buffer, nextSize);
            packets.push_back(translate(partialPacket.c_str()));
            i += nextSize;
            ptr += nextSize;
            packets.clear();
        }
    }*/
    while (i + packetSize <= bytes)
    {
        packets.push_back(translate(buffer + i));
        i += packetSize;
    }
}

void Translate::pack(const NetworkEvent &event, char* buffer)
{
    buffer[0] = event.e;
    buffer[1] = event.id;
    Uint16 x = event.x * factor;
    Uint16 y = event.y * factor;
    SDLNet_Write16(x, &buffer[2]);
    SDLNet_Write16(y, &buffer[4]);
}

std::string Translate::packetToString(const char* buffer) 
{
    NetworkEvent event = translate(buffer);
    std::stringstream ss;
    ss << (int)event.id << ": " << NETWORK_TRACE[(int)(event.e)] << " [" << event.x << ", " << event.y << "]";
    return ss.str();
}
