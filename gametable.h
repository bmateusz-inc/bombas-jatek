#ifndef GAMETABLE_H
#define GAMETABLE_H

#include <vector>
#include <queue>
#include <sstream>
#include <algorithm>

#include "ground.h"
#include "view/matrix.h"
#include "player.h"
#include "bomb.h"
#include "flame.h"
#include "goods.h"
#include "monster.h"
#include "view/animation.h"
#include "view/view.h"
#include "config.h"
#include "gameconfig.h"
#include "networkevent.h"
#include "gamemode.h"

/** The game table, lasts for a game's all rounds (5 or more if not terminated) */
class GameTable
{
    private:
        /** The View class */
        View &view;
        
        /** Number of squares horizontally */
        int sx;
        
        /** Number of squares vertically */
        int sy;
        
        /** Size of a square */
        int sb;
        
        /** Offset x */
        int dx;
        
        /** Offset y */
        int dy;
        
        /** Monster ID counter */
        int monsterCounter;

        Matrix<Ground> table;
        std::vector<Player> players;
        std::deque<Flame> flames;
        std::deque<Bomb> bombs;
        std::deque<Goods> goods;
        std::vector<GOODS> allGoods;
        std::deque<Monster> monsters;
        std::deque<std::pair< TIME, Point<int> > > walls;
        std::vector<NetworkEvent> networkEvents;

        GAME_GOAL goal;
        
        GameConfig gameConfig;
        
        bool calc;
        bool waitRound;

        /** When generating map, road or brick is built */
        bool path();
        void setMapElement(Point<int> p);
        
        /** The collision detection */
        void checkCollision();
        bool blocker(int x, int y, std::deque<Monster>::iterator i);
        bool blocker(int x, int y, std::vector<Player>::iterator i);
        bool blocker(int x, int y);
        void putBomb(Player* player, int x, int y);
        void checkBombs();
        void bombExplode(std::deque<Bomb>::iterator i, int from, int who = -1);
        void flame(int x, int y, int size, int from, int who);
        Point<int> randPoint();
        Point<int> randWall();
        void networkEvent(EVENT_TYPE e, char id, float x, float y);
        void networkMoves();
        void setMonster(int id, float x, float y);
        void dieMonster(int id);
        void diePlayer(int id);

    public:
        /**
         * Constructor
         * @param iView View class
         */
        GameTable(View &iView);
        
        /** Destructor */
        ~GameTable() { printf("gametable unload\n"); }
        
        /** Set number of players */
        void setPlayers(int n);
        
        /**
         * Load map from file
         * @param file file name
         * @return name of the next map
         */
        std::string loadMap(std::string file);
        
        /** Generate a random map */
        void generateMap();
        
        /** Generate an empty map*/
        void generateBase();
        
        /**
         * Set start position of player
         * @param x coordinate x
         * @param y coordinate y
         * @param i player ID
         */
        void setStart(int x, int y, int i);
        
        /**
         * Spawn player on map
         * @param i player ID
         */
        void spawnPlayer(int i);
        
        /**
         * Spawn monster. Call it with -1 to get random position or type.
         * @param i type of monster
         * @param x coordinate x
         * @param y coordinate y
         */
        void spawnMonster(int i = -1, int x = -1, int y = -1);
        
        /**
         * Start building maximum given number of walls on map
         */
        void spawnWalls();
        
        /** Start building a wall at given position */
        void spawnWalls(Point<int> p, bool doAnimation = true);
        
        /** If the wall is build, then put it on the map (after animation) */
        void buildWalls();
        
        /** Move things on map and check collision */
        void mainLoop();
        
        /** Draw everything */
        void draw();
        
        /** Set configuration for map */
        void setConfig(GameConfig &conf);
        
        /** Get configuration of map */
        GameConfig& getConfig() { return gameConfig; }
        
        /** Get players array (const) */
        const std::vector<Player> &getPlayers() const {return players;};
        
        /** Get players array (const) */
        const int getNPlayers() const {return players.size();};
        
        /** Returns if any of the active players push bomb put button */
        bool isAnyPlayerPut() const;
        
        /**
         * Increase score of player
         * @param pId player ID
         */
        void incScore(int pId) { players[pId].incScore(); }
        
        /** Count alive monsters */
        int countMonster() const { return monsters.size(); }
        
        /**
         * Bind player to Control class
         * @param i player ID
         * @param c Control class
         * @param team 1 or 2
         * @param skin the skin of player
         */
        void bind(int i, Control* c, int team, std::string skin);

        /* For network */
        /** Execute remote command */
        void execute(NetworkEvent event);
        
        /** Get movement of player for network */
        std::vector<NetworkEvent>& getMovement();
        
        /** Setter of calc, true if client */
        void setCalc(bool iCalc) { calc = iCalc; }
        
        /** Getter of calc, true if client */
        bool getCalc() const { return calc; }
        
        /** Get game goal, for score computing */
        GAME_GOAL getGoal() const { return goal; }
        
        /** Wait round, for client */
        bool getWaitRound() { return waitRound; }
        
        /** Set wait round, for client */
        void setWaitRound(bool iWaitRound) { waitRound = iWaitRound; }
};

#endif
