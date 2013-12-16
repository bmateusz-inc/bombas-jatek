#include "gamemanager.h"

GameManager::GameManager(GAME_MODE iGameMode, GAME_PLACE iGamePlace,
        View &iview, Input &iInput, GameTable &iTable, Network &iNetwork)
    : gameMode(iGameMode), gamePlace(iGamePlace),
      view(iview), input(iInput), table(iTable), network(iNetwork),
      round(true), showScoreBoard(true), loadNextMap(true)
{
    stop = 0;
    
    if(input.getRun() == false) { gameMode = EXIT; }
    
    if (gamePlace == SERVER)
    {
        network.startListen();
    }
    
    if(gameMode != EXIT) {
        Select select(view,input,table,network,gameMode,gamePlace);
        
        for(view.frameBegin(); select.mainLoop(); view.frameEnd());
    
        if (gamePlace == CLIENT)
        {
            gameMode = select.getGameMode();
        }
    } else {
        round = false;
        input.setRun(false);
    }
}

GameManager::~GameManager()
{
    if (gamePlace == SERVER)
    {
        network.stopListen();
    }
    if (gamePlace == CLIENT)
    {
        network.disconnect();
    }
}

void GameManager::prepareTable()
{
    int n = table.getNPlayers();
    int sx = table.getConfig().sx;
    int sy = table.getConfig().sy;
    
    if (gameMode == SINGLE) {
        for (int i = 0; i < n; ++i) {
            table.setStart(0,0,i);
        }
    } else {
        if(n>0) { table.setStart(0,0,0); }
        if(n>1) { table.setStart(sx-1,sy-1,1); }
        if(n>2) { table.setStart(0,sy-1,2); }
        if(n>3) { table.setStart(sx-1,0,3); }
        if(n>4) { table.setStart(sx/2+1,0,4); }
        if(n>5) { table.setStart(sx/2-1,sy-1,5); }
    }
}

void GameManager::newRound()
{
    table.setWaitRound(false);
    if(gameMode != EXIT) {
        GameConfig conf;
        conf.sx = 15;
        conf.sy = 11;
        conf.sb = 40;
        
        if (gameMode == SURVIVE) conf.goal = GOAL_MONSTER;
        if (gameMode == SINGLE || gameMode == CLASSIC) conf.resetPlayer = false;
        conf.place = gamePlace;

        table.setConfig(conf);
    }
    
    if(input.getRun() == false) { gameMode = EXIT; }
    
    view.setNow();
    if (table.getCalc())
    {
        switch(gameMode)
        {
            case SINGLE:
            {
                if (nextMap.empty()) nextMap = "maps/map_first.map";
                std::cout << nextMap << std::endl;
                if (loadNextMap)
                {
                    currMap = nextMap;
                    nextMap = table.loadMap(nextMap);
                }
                else table.loadMap(currMap);

                //table.setStart(0,0,0);

                break;
            }
            case CLASSIC:
            {
                table.generateMap();

                table.spawnMonster(0);
                table.spawnMonster(0);
                table.spawnMonster(1);
                table.spawnMonster(1);

                break;
            }
            case TEAM:
            {
                spawn_monster = view.getNow();

                table.generateMap();

                break;
            }
            case SURVIVE:
            {
                spawn_monster = view.getNow();

                table.generateBase();

                break;
            }
            default:
            {
                return;
            }
        }
    } else {
        table.generateBase();
    }
    
    if(gameMode == EXIT) {
        input.setRun(false);
    } else {
        input.setRun(true);
    }
    
    prepareTable();

    printf("manager done\n");
}

void GameManager::checkState()
{
    if(stop)
    {
        if(stop < view.getNow())
        {
            checkAfter();
            round = false;
            stop = 0;
        }
        return;
    }
    
    if (table.getCalc())
    {
        alivePlayers = 0;
        maxPoint = 0;
        maxKills = 0;
        maxMonsterKills = 0;
        for(std::vector<Player>::const_iterator i = table.getPlayers().begin(); i != table.getPlayers().end(); ++i)
        {
            if(i->isAlive()) ++alivePlayers;
            if(i->getScore() > maxPoint) maxPoint = i->getScore();
            if(i->getKill() > maxKills) maxKills = i->getKill();
            if(i->getMonsterKill() > maxMonsterKills) maxMonsterKills = i->getMonsterKill();
        }
    }
    
    if (gamePlace == SERVER || gamePlace == CLIENT)
    {
        doNetwork();
    }

    checkBefore();
}

void GameManager::checkBefore()
{
    if (table.getCalc())
    {
        if(gameMode == SINGLE)
        {
            if(alivePlayers < 1)
            {
                waitRound();
                loadNextMap = false;
            }

            for(int i = 0; i < table.getNPlayers(); ++i)
            {
                if(table.getPlayers()[i].getLife() == -1024) {
                    table.incScore(i);
                    waitRound();
                    loadNextMap = true;
                }
            }
        }
        if(gameMode == CLASSIC)
        {
            if(alivePlayers <= 1)
            {
                for(int i = 0; i < table.getNPlayers(); ++i)
                {
                    if(table.getPlayers()[i].isAlive())
                    {
                        table.incScore(i);
                    }
                }
                waitRound();
            }
        }
        if(gameMode == TEAM || gameMode == SURVIVE)
        {
            for(std::vector<Player>::const_iterator i = table.getPlayers().begin(); i != table.getPlayers().end(); ++i)
            {
                if( i->getDie() > 0 && i->getDie() + Config::getInstance().CFG_RESPAWN_PLAYER < view.getNow() )
                {
                    printf("respawn\n");
                    table.spawnPlayer(i->getId());
                }
            }
            if(view.getNow() > spawn_monster)
            {
                printf("spawn\n");
                spawn_monster = view.getNow() + Config::getInstance().CFG_RESPAWN_MONSTER_INTERVAL;
                table.spawnMonster(-1);
                table.spawnWalls();
            }
            table.buildWalls();
        }

        if (gameMode == TEAM && maxKills >= Config::getInstance().CFG_KILL_LIMIT)
        {
            for(int i = 0; i < table.getNPlayers(); ++i)
            {
                if (table.getPlayers()[i].getKill() >= maxKills) table.incScore(i);
            }
            waitRound();
        }

        if (gameMode == SURVIVE && maxMonsterKills >= Config::getInstance().CFG_MONSTER_KILL_LIMIT)
        {
            for(int i = 0; i < table.getNPlayers(); ++i)
            {
                if (table.getPlayers()[i].getMonsterKill() >= maxMonsterKills) table.incScore(i);
            }
            waitRound();
        }
    }
    else
    {
        if (table.getWaitRound()) waitRound();
        if(gameMode == TEAM || gameMode == SURVIVE) table.buildWalls();
    }
}

void GameManager::checkAfter()
{
}

void GameManager::doNetwork()
{
    std::vector<NetworkEvent>& packets = network.getPackets();
    for (std::vector<NetworkEvent>::iterator it = packets.begin(); it != packets.end(); ++it)
    {
        table.execute(*it);
    }
    packets.clear();
    
    network.sendPackets(table.getMovement());
    table.getMovement().clear();
    
    if (!network.getSuccess())
    {
        Msgbox("Hálózati probléma", view, input);
        input.setRun(false);
    }
}

void GameManager::waitRound()
{
    view.draw(0,0,640,480,"fade_out.anm");
    stop = view.getNow() + Config::getInstance().CFG_WAIT_ROUND;
    if (table.getCalc())
    {
        std::vector<NetworkEvent> scoreboardPackets;
        scoreboardPackets.push_back(NetworkEvent(SCOREBOARD_DATA, 0, 0, 0));
        for(std::vector<Player>::const_iterator i = table.getPlayers().begin(); i != table.getPlayers().end(); ++i)
        {
            scoreboardPackets.push_back(NetworkEvent(SCOREBOARD_DATA, i->getId(), 1, i->getScore()));
            scoreboardPackets.push_back(NetworkEvent(SCOREBOARD_DATA, i->getId(), 2, i->getKill()));
            scoreboardPackets.push_back(NetworkEvent(SCOREBOARD_DATA, i->getId(), 3, i->getKilled()));
            scoreboardPackets.push_back(NetworkEvent(SCOREBOARD_DATA, i->getId(), 4, i->getMonsterKill()));
        }
        network.sendPackets(scoreboardPackets);
    }
    table.setWaitRound(true);
}

bool GameManager::getRun()
{
    if (gameMode == EXIT) return false;
    return input.getRunSoft();
}

bool GameManager::getRound()
{
    if (input.getRun() == false) { return false; }
    if (round == false) { round = true; return false; }
    return round;
}

bool GameManager::getShowScoreBoard()
{
    return showScoreBoard;
}
