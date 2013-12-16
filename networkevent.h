#ifndef NETWORKEVENT_H
#define	NETWORKEVENT_H

enum NETWORKEVENT
{
    UNDEFINED_NETWORKEVENT,
    
    /* GameTable */
    MOVEMENT,
    MONSTER_MOVEMENT,
    PUT_BOMB,
    SPAWN_VIRUS,
    INFECT_PLAYER,
    DIE,
    MONSTER_DIE,
    SPAWN_MONSTER,
    SPAWN_WALL,
    SHOW_STAT,
    
    /* GameManager */
    SET_MODE,
    SET_NPLAYERS,
    SET_START,
    SET_NETWORK_ID,
    SET_TEAM,
    MAP_BUILD,
    SCOREBOARD_DATA,
    RESPAWN_PLAYER,
    
    LAST_NETWORKEVENT
};

typedef char EVENT_TYPE;

static const char* NETWORK_TRACE[] = {
    "undefined",
    "movement",
    "monster_move",
    "put_bomb",
    "spawn_virus",
    "infect_player",
    "die",
    "monster_die",
    "spawn_monster",
    "spawn_wall",
    "show_stat",
    
    "set_mode",
    "set_nplayers",
    "set_start",
    "set_network_id",
    "set_team",
    "map_build",
    "scoreboard_data",
    "respawn_player",
    
    "last"
};

/** Network packet, use Translate class to handle */
class NetworkEvent
{
public:
    /**
     * Type of event
     */
    EVENT_TYPE e;
    
    /** ID of player (or monster) */
    unsigned char id;
    
    /** Coordinate X */
    float x;
    
    /** Coordinate Y */
    float y;
    
    /** Default constructor */
    NetworkEvent() {}
    
    /** Constructor */
    NetworkEvent(EVENT_TYPE e_, unsigned char id_, float x_, float y_)
        : e(e_), id(id_), x(x_), y(y_) {}
    
    /*static std::string trace(int n)
    {
        //if (n >= (sizeof NETWORK_TRACE / sizeof *NETWORK_TRACE) || n < 1) return "ERROR";
        //return std::string(NETWORK_TRACE[n]);
    }*/
    
    /** Size of all network packets */
    static const int packetSize = 6;
};

#endif
