#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

/** Configuration of the game */
class Config
{
public:
    /** Flame lasts for that time after ignited */
    int CFG_FLAMETIME;
    
    /** Bomb ticks for that time */
    int CFG_BOMBTIME;
    
    /** Default speed factor */
    float CFG_DEFAULT_SPEED;
    
    /** Slow virus speed factor */
    float CFG_SLOW_SPEED;
    
    /** Fast virus speed factor */
    float CFG_FAST_SPEED;

    /** Virus time */
    int CFG_VIRUSTIME;

    /** Player respawn time */
    int CFG_RESPAWN_PLAYER;
    
    /** Monster spawning time interval */
    int CFG_RESPAWN_MONSTER;
    
    /** Spawn monsters in interval */
    int CFG_RESPAWN_MONSTER_INTERVAL;
    
    /** Wait for next round */
    int CFG_WAIT_ROUND;

    /** Show scoreboard for given time */
    int CFG_SHOW_SCOREBOARD;

    /** The player cannot die after spawning for this time */
    int CFG_IMMORTAL;
    
    /** Number of walls built at once */
    int CFG_WALLS_BUILD_PARALLEL;
    
    /** Factor to built walls get something behind */
    int CFG_WALLS_BUILD_GOODS;
    
    /** Limit of walls */
    int CFG_WALLS_LIMIT;
    
    /** Score limit in Classic, Team and Survival mode */
    int CFG_SCORE_LIMIT;
    
    /** Kill limit in Team mode */
    int CFG_KILL_LIMIT;
    
    /** Monster kill limit in Survival mode */
    int CFG_MONSTER_KILL_LIMIT;

    /** Port for multiplayer */
    unsigned int CFG_PORT;

    /** Port for multiplayer */
    char* CFG_MASTER_SERVER;
    
    /** For testing, 0 - 60 fps, 1 - 10 fps, 2 - 5 fps, 3 - 2 fps, 4 - unlimited */
    int CFG_TEST_FPS;

    /** Constructor, set default values */
    Config();
    
    /** Get singleton */
    static Config& getInstance();
};

const int BUFFER_SIZE = 6;

const unsigned short MAX_SOCKETS = 16;
const unsigned short MAX_CLIENTS = MAX_SOCKETS - 1;

#endif
