#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>

#include "point.h"
#include "control.h"
#include "utils.h"
#include "config.h"

/** Type of infections */
enum PLAYER_VIRUS {
    NOVIRUS,
    PUTPUT,
    NOPUT,
    UNLIMITED_BOMB,
    UNLIMITED_FLAME,
    UNEXPECTED_MONSTER
};

/**
 * This class contains all information about a player
 */
class Player
{
    private:
        int id;
        Control *control;
        Point<float> start;
        Point<float> point;
        Point<float> prev;
        Point<int> lastBomb;
        float speed;
        int size;
        int bombs;
        int life;
        bool put;
        WAY prevWay;
        int step;
        TIME change;
        TIME now;
        TIME updated;
        TIME virus;
        TIME died;
        TIME showKills;
        TIME spawned;
        PLAYER_VIRUS virusMode;
        int score;
        int kill;
        int killed;
        int monsterKill;
        std::string skin;
        int team;
        bool remote;
        bool sendShowKills;
        int receivedShowKills;

    public:
        /** Constructor, initialize player */
        Player();
        
        /** Destructor */
        ~Player();
        
        /** Set values to default, including position and anything that
         *  effected the player in previous rounds */
        void init();
        
        /** Set scores (kill, killed, monsters killed) to 0 */
        void newRound();
        
        /** Reset virus attributes */
        void novirus();

        /**
         * Move the player to the direction given by Control class,
         * with calculating the time difference, so the
         * frame rate doesn't matter. For local players.
         * @param inow current frame time
         */
        void move(TIME inow);
        
        /**
         * Move the player to a specific point. For remote players.
         * @param p point where it moves
         * @param inow current frame time
         */
        void move(Point<float> p, TIME inow);
        
        /**
         * Get movement of player.
         * @param p the new position
         * @return the player changes it's position or not
         */
        bool getMove(Point<float> &p);
        
        /** Infect player with specific virus */
        int infect(int type);

        /** Player dies */
        void die();
        
        /** Player wins */
        void win();

        /**
         * Returns the current animation phase of player
         * @return graphic name
         */
        std::string state();
        
        /**
         * Get special effect around player
         * @return graphic name
         */
        std::string getAura() const;
        
        /** Return that the player puts bomb or not */
        bool getPut() const;
        
        /** Return in any case that the player puts bomb or not (for scoreboard skip) */
        bool getPutForce() const;

        /** Return the ID of the player */
        int getId() const { return id; }
        
        /** Returns the Control class of player */
        Control* getControl() { return control; }
        
        /** Get position of player */
        Point<float> getPoint() { return point; }
        
        /** Get X coordinate of player */
        float getX() { return point.getX(); }
        
        /** Get Y coordinate of player */
        float getY() { return point.getY(); }
        
        /** Get previous X coordinate of player */
        float getPrevX() { return prev.getX(); }
        
        /** Get previous Y coordinate of player */
        float getPrevY() { return prev.getY(); }
        
        /** Get the X coordinate where the player put bomb */
        int getBombX() { return lastBomb.getX(); }
        
        /** Get the Y coordinate where the player put bomb */
        int getBombY() { return lastBomb.getY(); }
        
        /** Get flame size */
        int getSize() { if(virusMode == UNLIMITED_FLAME) return 64; return size; }
        
        /** Get number of bombs */
        int getBombs() { return bombs; }
        
        /** Get life of player */
        int getLife() const { return life; }
        
        /** Return if the player is alive or not */
        bool isAlive() const { return life>0; }
        
        /** Get score of player */
        int getScore() const { return score; }
        
        /** Get the number of kills */
        int getKill() const { return kill; }
        
        /** Get the number of get killed */
        int getKilled() const { return killed; }
        
        /** Get the number of monsters killed */
        int getMonsterKill() const { return monsterKill; }
        
        /** Get if the player is remote */
        bool getRemote() const { return remote; }
        
        /** Get skin of player (prefix for graphic files) */
        std::string getSkin() const { return skin; }
        
        /** Set team */
        int getTeam() const { return team; }
        
        /** Return the frame time when player died */
        TIME getDie() const { return died; }
        
        /** Return when showing kill statistic should disappear */
        TIME getShowKills() const { return showKills; }
        
        /** Set id of player */
        void setId(int i) { id = i; }
        
        /** Set Control class for player, only one can be active per player */
        void setControl(Control* icontrol) { control = icontrol; }
        
        /** Set X coordinate, used at collision detection */
        void setX(float x);
        
        /** Set Y coordinate, used at collision detection */
        void setY(float y);
        
        /** Set X coordinate for the last bomb put */
        void setBombX(int x) { lastBomb.setX(x); }
        
        /** Set Y coordinate for the last bomb put */
        void setBombY(int y) { lastBomb.setY(y); }
        
        /** Increase flame size */
        void incSize() { ++size; }
        
        /** Increase number of bombs */
        void incBombs() { ++bombs; }
        
        /** Decrease number of bombs */
        void decBombs() { --bombs; }
        
        /** Set life */
        void setLife(int l) { life = l; }
        
        /** Set put */
        void setPut(bool p) { put = p; }
        
        /** Set score */
        void setScore(int s) { score = s; }
        
        /** Increase score */
        void incScore() { ++score; }
        
        /** Set kills */
        void setKill(int k) { kill = k; }
        
        /** Increase kills */
        void incKill() { ++kill; }
        
        /** 
         * Increase kills with showing the statistics
         * @param iShowKills frame time when it should disappear
         */
        void incKill(TIME iShowKills) { incKill(); setShowKills(iShowKills); }
        
        /** Decrease kills (when suicide) */
        void decKill() { --kill; }
        
        /** 
         * Decrease kills (when suicide) with showing the statistics
         * @param iShowKills frame time when it should disappear
         */
        void decKill(TIME iShowKills) { decKill(); setShowKills(iShowKills); }
        
        /** Set end time of showing statistics */
        void setShowKills(TIME iShowKills) { sendShowKills = true; showKills = iShowKills; }
        
        /** Set end time of showing statistics */
        void setShowKills(TIME iShowKills, int number) { showKills = iShowKills; receivedShowKills = number; }
        
        /** Set killed by other */
        void setKilled(int k) { killed = k; }
        
        /** Increase killed by other */
        void incKilled() { ++killed; }
        
        /** Set monsters killed */
        void setMonsterKill(int k) { monsterKill = k; }
        
        /** Increase monsters killed */
        void incMonsterKill() { ++monsterKill; }
        
        /** Set if the player is remote player */
        void setRemote(bool iRemote) { remote = iRemote; }
        
        /** Set skin of player */
        void setSkin(std::string iSkin) { skin = iSkin; }
        
        /** Set team */
        void setTeam(int iteam) { team = iteam; }
        
        /** Set frame time when the player died */
        void setDie(TIME idie) { died = idie; }
        
        /** Set the start point of player */
        void setStart(Point<float> p, TIME t) { start = p; setStart(t); }
        
        /** Position the player to the start point already set */
        void setStart(TIME t) { point = prev = start; spawned = t; }
        
        /** Is player immortal (for a few second after spawn) */
        bool isImmortal() const;
        
        /** True if that instance is not controlled by the player(s) */
        bool isRemotePlayer() const { return control == 0; }
        
        /** True if show kills changed and needs to be sent */
        bool getSendShowKills() { if (sendShowKills) { sendShowKills = false; return true; }
                                  else { return false; } }
        
        /** For client, return the data to show on table */
        int getReceivedShowKills() const { return receivedShowKills; }
};

#endif
