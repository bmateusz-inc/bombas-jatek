#include "config.h"

Config::Config() :
        CFG_FLAMETIME(400),
        CFG_BOMBTIME(2000),
        CFG_DEFAULT_SPEED(0.2),
        CFG_SLOW_SPEED(0.075),
        CFG_FAST_SPEED(0.5),
        CFG_VIRUSTIME(10000),
        CFG_RESPAWN_PLAYER(5000),
        CFG_RESPAWN_MONSTER(1000),
        CFG_RESPAWN_MONSTER_INTERVAL(4000),
        CFG_WAIT_ROUND(1400),
        CFG_SHOW_SCOREBOARD(7000),
        CFG_IMMORTAL(2500),
        CFG_WALLS_BUILD_PARALLEL(5),
        CFG_WALLS_BUILD_GOODS(5),
        CFG_WALLS_LIMIT(25),
        CFG_SCORE_LIMIT(5),
        CFG_KILL_LIMIT(5),
        CFG_MONSTER_KILL_LIMIT(30),
        CFG_PORT(1324),
        CFG_MASTER_SERVER((char*)"localhost"),
        CFG_TEST_FPS(0)
{}

Config& Config::getInstance()
{
    static Config config;
    return config;
}