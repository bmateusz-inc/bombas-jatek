#include "gametable.h"
#include "gamemode.h"
#include "translate.h"

GameTable::GameTable(View &iView) : view(iView), calc(true), waitRound(false)
{
}

void GameTable::setConfig(GameConfig &conf)
{
    gameConfig = conf;
    if(conf.gen)
    {
        sx = conf.sx;
        sy = conf.sy;
        sb = conf.sb;
        dx = sb/2;
        dy = sb/2;
        table.setSize(sx,sy);
    }

    if(conf.init)
    {
        for(int i=0; i < static_cast<int>(players.size()); ++i)
        {
            players[i].setId(i);
            players[i].init();
            if (conf.resetPlayer) players[i].newRound();
        }
    }
    
    goal = conf.goal;

    if (calc)
    {
        for(int i=0; i<conf.nBomb; ++i) allGoods.push_back(BOMB);
        for(int i=0; i<conf.nFlame; ++i) allGoods.push_back(FLAME);
        for(int i=0; i<conf.nVirus; ++i) allGoods.push_back(VIRUS);
        std::random_shuffle(allGoods.begin(), allGoods.end());
    }

    monsterCounter = 0;
    monsters.clear();
    flames.clear();
    bombs.clear();
    goods.clear();
}

void GameTable::spawnMonster(int i, int x, int y)
{
    if(i==-1) { i = rand()%2; }
    Point<int> q(x,y);
    if(x==-1 || y==-1) { q = randPoint(); }
    Point<float> p = Point<float>(q.getX()*sb, q.getY()*sb);
    monsters.push_back(Monster(i, sb, p, monsterCounter++, view.getNow()));
    
    if (calc)
    {
        networkEvent(SPAWN_MONSTER, i,
                     monsters.back().getNetworkX(),
                     monsters.back().getNetworkY());
    }

    view.draw(p.getX()+dx, p.getY()+dy, sb, sb, "spawn.anm");
}

void GameTable::spawnWalls()
{
    int countWalls = table.count(WALL);
    
    while (countWalls < Config::getInstance().CFG_WALLS_LIMIT &&
           static_cast<int>(walls.size()) < Config::getInstance().CFG_WALLS_BUILD_PARALLEL + 1) {
        Point<int> p = randWall();
        spawnWalls(p);
        ++countWalls;
    
        int r = rand() % (Config::getInstance().CFG_WALLS_BUILD_GOODS);
        if (r == 0)
        {
            printf("New random good\n");
            allGoods.push_back(Goods::getRandGood());
        }
    }
}

void GameTable::spawnWalls(Point<int> p, bool doAnimation)
{
    TIME t = view.getNow() + Config::getInstance().CFG_RESPAWN_MONSTER_INTERVAL;
    walls.push_back(std::pair< TIME, Point<int> >(t,p));
    if (doAnimation)
    {
        view.draw(p.getX()*sb+dx, p.getY()*sb+dy, sb, sb, "spawnWall.anm");
    }
    if (calc)
    {
        networkEvent(SPAWN_WALL, 0, p.getX(), p.getY());
    }
}

void GameTable::buildWalls()
{
    while (!walls.empty() && walls.front().first <= view.getNow())
    {
        int ix = walls.front().second.getX();
        int iy = walls.front().second.getY();
        table(ix, iy) = WALL;
        for (std::deque<Goods>::iterator i = goods.begin(); i != goods.end(); ++i)
        {
            if (ix == i->getX() && iy == i->getY())
            {
                i = goods.erase(i);
                break;
            }
        }
        walls.pop_front();
    }
}

void GameTable::setMapElement(Point<int> p)
{
    table(p) = ROAD;
    networkEvent(MAP_BUILD, ROAD, p.getX(), p.getY());
}

void GameTable::setStart(int x, int y, int i)
{
    players[i].setStart(Point<float>(x*sb,y*sb), view.getNow());

    Point<int> p(x,y);
    if(table.valid(p) && table(p) == WALL) setMapElement(p);
    p = Point<int>(x+1,y);
    if(table.valid(p) && table(p) == WALL) setMapElement(p);
    p = Point<int>(x,y+1);
    if(table.valid(p) && table(p) == WALL) setMapElement(p);
    p = Point<int>(x-1,y);
    if(table.valid(p) && table(p) == WALL) setMapElement(p);
    p = Point<int>(x,y-1);
    if(table.valid(p) && table(p) == WALL) setMapElement(p);
}

void GameTable::spawnPlayer(int i)
{
    players[i].init();
    players[i].setStart(view.getNow());
    if (calc) networkEvent(RESPAWN_PLAYER, i, 0, 0);
    //view.draw(players[i].getX()+dx, players[i].getY()+dx, sb, sb, "spawn.anm");
}

Point<int> GameTable::randPoint()
{
    int x,y;
    do {
        x = (1+rand()%(sx/2-1))*2;
        y = (1+rand()%(sy/2-1))*2;
        printf("randPoint(%d, %d)\n",x,y);
    } while(blocker(x,y));
    //    table(y,x) = ROAD;
    return Point<int>(x, y);
}

Point<int> GameTable::randWall()
{
    int x;
    int y;
    int safety = 0;
    do {
        x = (2+rand()%(sx-4));
        y = (2+rand()%(sy-4));
        printf("randWall(%d, %d)\n",x,y);
        safety++;
    } while(blocker(x,y) && safety < 500);
    //    table(y,x) = ROAD;
    return Point<int>(x, y);
}

void GameTable::setPlayers(int n)
{
    players.resize(n);
}

std::string GameTable::loadMap(std::string file)
{
    std::fstream f(file.c_str());
    if (f.fail())
    {
        printf("No map named \n");
    }
    f >> sb >> dx >> dy >> table;
    
    sx = table.getN();
    sy = table.getM();
    
    std::string nextMap;
    f >> nextMap;
    
    int mType, mx, my;
    while(f >> mType >> mx >> my)
    {
        spawnMonster(mType, mx, my);
    }
    f.close();
    
    return nextMap;
}

void GameTable::generateMap()
{
    if (calc) {
        for(int i = 0; i < sx; ++i)
        {
            for(int j = 0; j < sy; ++j)
            {
                if(i%2==1 && j%2==1) {
                    networkEvent(MAP_BUILD, BLOCK, i, j);
                    table(i,j) = BLOCK;
                }
                else if(path()) table(i,j) = ROAD;
                else {
                    networkEvent(MAP_BUILD, WALL, i, j);
                    table(i,j) = WALL;
                }
            }
        }
    }
    else
    {
        generateBase();
    }
}

void GameTable::generateBase()
{
    for(int i = 0; i < sx; ++i)
    {
        for(int j = 0; j < sy; ++j)
        {
            if(i%2==1 && j%2==1) table(i,j) = BLOCK;
            else table(i,j) = ROAD;
        }
    }
}

bool GameTable::path()
{
    return (rand()%10>4);
}

void GameTable::checkCollision()
{
    for(std::deque<Flame>::iterator j = flames.begin(); j != flames.end();)
    {
        if(j->expire(view.getNow())) j=flames.erase(j);
        else ++j;
    }

    for(std::vector<Player>::iterator i = players.begin(); i != players.end(); ++i)
    {
        if(i->isAlive())
        {
            float x = i->getX() / sb;
            float y = i->getY() / sb;
            int ix = x + 0.5;
            int iy = y + 0.5;
            //float ddx=i->getX() - i->getPrevX();
            //float ddy=i->getY() - i->getPrevY();
            Point<int> p(ix,iy);

            /* Collision detection */
            if(blocker(ix+1,iy,i) && x > ix)
            {
                i->setX(ix*sb);
            }
            if(blocker(ix-1,iy,i) && x < ix)
            {
                i->setX(ix*sb);
            }
            if(blocker(ix,iy+1,i) && y > iy)
            {
                i->setY(iy*sb);
            }
            if(blocker(ix,iy-1,i) && y < iy)
            {
                i->setY(iy*sb);
            }

            if(blocker(ix,iy,i))
            {
                i->setX(i->getPrevX());
                i->setY(i->getPrevY());
            }
            
            if(table.valid(ix,iy) && table(ix,iy) == WIN) {
                i->win();
                view.draw(i->getX()+dx,i->getY()+dy-sb/2,sb,sb*1.5, "win.anm#" + i->getSkin());
            }

            /* Check table */
            if (!(i->isImmortal()) && calc)
            {
                for(std::deque<Flame>::iterator j = flames.begin(); j != flames.end(); ++j)
                {
                    if(p == j->getPoint() && j->getDirection() != 7)
                    {
                        i->incKilled();
                        if(j->getOwner() != i->getId())
                        {
                            if (goal != GOAL_TEAM)
                            {
                                players[j->getOwner()].incKill(view.getNow() + 3500);
                            }
                            else
                            {
                                int scoreTeam = players[j->getOwner()].getTeam();
                                if (scoreTeam != i->getTeam())
                                {
                                    for (int k = 0; k < getNPlayers(); ++k)
                                    {
                                        if (players[k].getTeam() == scoreTeam)
                                        {
                                            players[k].incKill(view.getNow() + 3500);
                                        }
                                    }
                                }
                            }
                        }
                        else
                        {
                            //i->decKill(view.getNow() + 3500);
                        }
                        diePlayer(i->getId());
                        networkEvent(DIE, i->getId(), 1, 0);
                        break;
                    }
                }

                for(std::deque<Monster>::iterator j = monsters.begin(); j != monsters.end(); ++j)
                {
                    if(j->isActive() && distance(i->getPoint(), j->getPoint()) <= sb/2)
                    {
                        i->incKilled();
                        view.draw(i->getX()+dx,i->getY()+dy-sb/2,sb,sb*1.5, "die.anm#" + i->getSkin());

                        i->die();
                        networkEvent(DIE, i->getId(), 1, 0);
                        break;
                    }
                }
            }

            if (i->getPut())
            {
                i->setPut(false);
                putBomb(&*i, ix, iy);
            }

            for (std::deque<Goods>::iterator j = goods.begin(); j != goods.end();)
            {
                if(p == j->getPoint())
                {
                    if (calc)
                    {
                        std::string f;
                        int infect = j->effect(&*i, f);

                        networkEvent(INFECT_PLAYER, i->getId()*16 + infect, p.getX(), p.getY());

                        if (infect == 6) {
                            spawnMonster(-1, p.getX(), p.getY());
                        }

                        view.draw(p.getX()*sb+dx-20,p.getY()*sb+dy+sb/2,10,20, "font.anm##" + f);
                    }
                    
                    j = goods.erase(j);
                } else {
                    ++j;
                }
            }
        }
    }

    /* Move and check monsters */
    if (calc)
    {
        for(std::deque<Monster>::iterator i = monsters.begin(); i != monsters.end(); ++i)
        {
            float x = i->getX() / sb;
            float y = i->getY() / sb;
            int ix = x + 0.5;
            int iy = y + 0.5;

            int r = rand()%3;
            if(blocker(ix+1,iy,i) && x > ix)
            {
                i->setX(ix*sb);
                if(r==0) i->setDx(-1);
                if(r==1) i->setDy(1);
                if(r==2) i->setDy(-1);
            }
            if(blocker(ix-1,iy,i) && x < ix)
            {
                i->setX(ix*sb);
                if(r==0) i->setDx(1);
                if(r==1) i->setDy(1);
                if(r==2) i->setDy(-1);
            }
            if(blocker(ix,iy+1,i) && y > iy)
            {
                i->setY(iy*sb);
                if(r==0) i->setDx(1);
                if(r==1) i->setDx(-1);
                if(r==2) i->setDy(-1);
            }
            if(blocker(ix,iy-1,i) && y < iy)
            {
                i->setY(iy*sb);
                if(r==0) i->setDx(1);
                if(r==1) i->setDx(-1);
                if(r==2) i->setDy(1);
            }
        }

        for(std::deque<Flame>::iterator j = flames.begin(); j != flames.end(); ++j)
        {
            if(j->getDirection() != 7)
            for(std::deque<Monster>::iterator i = monsters.begin(); i != monsters.end();)
            {
                if(j->getX() == (int)(i->getX()/sb+0.5) && j->getY() == (int)(i->getY()/sb+0.5))
                {
                    view.draw(i->getX()+dx, i->getY()+dy, sb, sb, i->die());
                    networkEvent(MONSTER_DIE, i->getId(), i->getX(), i->getY());
                    i = monsters.erase(i);
                    players[j->getOwner()].incMonsterKill();
                    if (goal == GOAL_MONSTER) players[j->getOwner()].setShowKills(view.getNow() + 3500);
                } else ++i;
            }
        }
    }
}

void GameTable::diePlayer(int id)
{
    Player &i = players.at(id);
    view.draw(i.getX()+dx,i.getY()+dy-sb/2,sb,sb*1.5, "die.anm#" + i.getSkin());
    i.die();
}

bool GameTable::blocker(int x, int y, std::deque<Monster>::iterator i)
{
    if(x<0 || y<0 || x>=sx || y>=sy) return true;
    Ground ground = table(x,y);
    if(i->change() && rand()%8 == 0) { return true; }
    if(i->getGoesThroughWall())
    {
        if(ground != ROAD && ground != WIN) return true;
    }
    else
    {
        if(ground == BLOCK) return true;
    }
    for(std::deque<Bomb>::iterator j = bombs.begin(); j != bombs.end(); ++j)
    {
        if(j->getX() == x && j->getY() == y) return true;
    }
    return false;
}

bool GameTable::blocker(int x, int y, std::vector<Player>::iterator i)
{
    if(x<0 || y<0 || x>=sx || y>=sy) return true;
    Ground ground = table(x,y);
    if(ground != ROAD && ground != WIN) return true;
    for(std::deque<Bomb>::iterator j = bombs.begin(); j != bombs.end(); ++j)
    {
        if(j->getX() == x && j->getY() == y)
        {
            float pdx = fabs(i->getPrevX() - (j->getX()+0.5)*sb);
            float dx = fabs(i->getX() - (j->getX()+0.5)*sb);
            float pdy = fabs(i->getPrevY() - (j->getY()+0.5)*sb);
            float dy = fabs(i->getY() - (j->getY()+0.5)*sb);
            int ix = i->getPrevX() / sb + 0.5;
            int iy = i->getPrevY() / sb + 0.5;
            if((pdx > dx || pdy > dy) && (ix!=x || iy!=y)) return true;
        }
    }
    return false;
}

bool GameTable::blocker(int x, int y)
{
    if(x<0 || y<0 || x>=sx || y>=sy) return true;
    Ground ground = table(x,y);
    if(ground != ROAD && ground != WIN) return true;
    return false;
}

void GameTable::putBomb(Player* player, int x, int y)
{
    if(player==0) return;
    for(std::deque<Bomb>::iterator i = bombs.begin(); i != bombs.end(); ++i)
    {
        if(i->getX() == x && i->getY() == y) return;
    }

    bombs.push_back(Bomb(view.getNow() + Config::getInstance().CFG_BOMBTIME,
                         Point<int>(x, y), player));
    if (calc || !player->isRemotePlayer()) networkEvent(PUT_BOMB, player->getId(), x, y);
    player->setBombX(x);
    player->setBombY(y);
}

void GameTable::checkBombs()
{
    std::deque<Bomb>::iterator i = bombs.begin();
    while(i != bombs.end())
    {
        if(i->boom(view.getNow()))
        {
            bombExplode(i,0);
            return;
        }
        else
        {
            ++i;
        }
    }
}

bool GameTable::isAnyPlayerPut() const
{
    for(std::vector<Player>::const_iterator it = players.begin(); it != players.end(); ++it)
    {
        if (it->getPutForce()) return true;
    }
    return false;
}

void GameTable::bombExplode(std::deque<Bomb>::iterator i, int from, int who)
{
    int x = i->getX();
    int y = i->getY();
    int size = i->getSize();
    //int who = i->getOwner();
    if(who == -1) who = i->getOwner();
    i->boom();
    i = bombs.erase(i);
    flame(x, y, 0, 5, who);
    flame(x, y, 0, 6, who);
    if(from!=3) flame(x, y+1, size, 1, who);
    if(from!=4) flame(x+1, y, size, 2, who);
    if(from!=1) flame(x, y-1, size, 3, who);
    if(from!=2) flame(x-1, y, size, 4, who);
}

void GameTable::flame(int x, int y, int size, int from, int who)
{
    if(x<0 || y<0 || x>=sx || y>=sy) return;

    if(table(x,y) == WALL)
    {
        table(x,y) = ROAD;
        flames.push_back(Flame(x, y, view.getNow() + Config::getInstance().CFG_FLAMETIME, 7, who));
        int count = table.count(Ground(WALL));
        int r = count != 0 ? rand() % count : 1;
        if(r < static_cast<int>(allGoods.size()))
        {
            goods.push_back(Goods(Point<int>(x,y), allGoods.back()));
            networkEvent(SPAWN_VIRUS, (int)allGoods.back(), x, y);
            allGoods.pop_back();
        }
        return;
    }
    if(table(x,y) == BLOCK) return;

    for(std::deque<Bomb>::iterator i = bombs.begin(); i != bombs.end(); ++i)
    {
        if(i->getX() == x && i->getY() == y)
        {
            bombExplode(i,from,who);
            return;
        }
    }

    for(std::deque<Goods>::iterator i = goods.begin(); i != goods.end(); ++i)
    {
        if(i->getX() == x && i->getY() == y)
        {
            flames.push_back(Flame(x, y, view.getNow() + Config::getInstance().CFG_FLAMETIME, 7, who));
            i = goods.erase(i);
            return;
        }
    }
    if(size==1) from += 8;
    flames.push_back(Flame(x, y, view.getNow() + Config::getInstance().CFG_FLAMETIME, from, who));

    if(size <= 1) return;

    if(from==1) flame(x, y+1, size-1, 1, who);
    if(from==2) flame(x+1, y, size-1, 2, who);
    if(from==3) flame(x, y-1, size-1, 3, who);
    if(from==4) flame(x-1, y, size-1, 4, who);
}

void GameTable::draw()
{
    view.draw(0,0,640,480,"menu.png");

    for(int i=0; i<sx; ++i)
    {
        for(int j=0; j<sy; ++j)
        {
            view.draw(i*sb+dx, j*sb+dy, sb, sb, table(i,j).state());
        }
    }
    
    for(std::deque<Goods>::iterator i = goods.begin(); i != goods.end(); ++i)
        view.draw(i->getX()*sb+dx, i->getY()*sb+dy, sb, sb, i->state());

    for(std::deque<Bomb>::iterator i = bombs.begin(); i != bombs.end(); ++i)
        view.draw(i->getX()*sb+dx, i->getY()*sb+dy, sb, sb, i->state());

    for(std::deque<Monster>::iterator i = monsters.begin(); i != monsters.end(); ++i)
        view.draw(i->getX()+dx, i->getY()+dy, sb, sb, i->state());

    for(std::deque<Flame>::iterator i = flames.begin(); i != flames.end(); ++i)
        view.draw(i->getX()*sb+dx, i->getY()*sb+dy, sb, sb, i->state());

    for(std::vector<Player>::iterator i = players.begin(); i != players.end(); ++i)
    {
        if(i->getLife()>0)
        {
            view.draw(i->getX()+dx,i->getY()+dy-sb/2,sb,sb*1.5,i->state());
            view.draw(i->getX()+dx,i->getY()+dy,sb,sb,i->getAura());
        }
        if(view.getNow() < i->getShowKills())
        {
            std::stringstream ss;
            int result = 0;
            
            if (calc) {
                if (goal == GOAL_KILL || goal == GOAL_TEAM) result = i->getKill();
                else if (goal == GOAL_MONSTER) result = i->getMonsterKill();
            
                if (i->getSendShowKills()) { networkEvent(SHOW_STAT, i->getId(), result, 0); }
            } else {
                result = i->getReceivedShowKills();
            }
            
            ss << result;
            
            //view.draw(20+i->getId()*50,5,5,10,"font.txh#" + ss.str());
            int offset = 20 - ss.str().length() *5;
            view.draw(i->getX()+dx+offset,i->getY()+dy-sb/2-15,10,20,"font.txh#" + ss.str());
        }
    }

    view.drawAnim();
}

void GameTable::mainLoop()
{
    if (!waitRound)
    {
        for(std::vector<Player>::iterator i = players.begin(); i != players.end(); ++i)
        {
            i->move(view.getNow());
        }
        for(std::deque<Monster>::iterator i = monsters.begin(); i != monsters.end(); ++i)
        {
            i->move(view.getNow());
        }
        checkCollision();
        checkBombs();
        networkMoves();
    }
}

void GameTable::bind(int i, Control* c, int team, std::string skin)
{
    if (c == 0)
    {
        players[i].setRemote(true);
    }
    else
    {
        players[i].setControl(c);
    }
    players[i].setTeam(team);
    players[i].setSkin(skin);
    printf("bind #%d\n",i);
}

void GameTable::setMonster(int id, float x, float y)
{
    for(std::deque<Monster>::iterator i = monsters.begin(); i != monsters.end(); ++i)
    {
        if (i->getId() == id)
        {
            i->setNetworkX(x);
            i->setNetworkY(y);
            return;
        }
    }
    std::cout << "Monster not found " << id << std::endl;
}

void GameTable::dieMonster(int id)
{
    for(std::deque<Monster>::iterator i = monsters.begin(); i != monsters.end();)
    {
        if (i->getId() == id)
        {
            view.draw(i->getX()+dx, i->getY()+dy, sb, sb, i->die());
            i->die();
            i = monsters.erase(i);
            return;
        } else {
            ++i;
        }
    }
    std::cout << "Monster not found die " << id << std::endl;
}

void GameTable::execute(NetworkEvent event)
{
    switch(event.e)
    {
        case MOVEMENT:
            players[event.id].move(Point<float>(event.x, event.y), view.getNow());
            break;
        case PUT_BOMB:
            putBomb(&players[event.id], event.x, event.y);
            break;
        case MONSTER_MOVEMENT:
            setMonster(event.id, event.x, event.y);
            break;
        case SPAWN_VIRUS:
            goods.push_back(Goods(Point<int>(event.x, event.y), GOODS(event.id)));
            break;
        case INFECT_PLAYER:
            view.draw(event.x*sb+dx-20,event.y*sb+dy+sb/2,10,20,
                      "font.anm##" + Goods::effect(&(players[Translate::slice(event.id, false)]),
                                                   Translate::slice(event.id, true)));
            break;
        case SHOW_STAT:
            players[event.id].setShowKills(view.getNow() + 3500, event.x);
            break;
        case DIE:
            //players[event.id].die();
            diePlayer(event.id);
            break;
        case MONSTER_DIE:
            dieMonster(event.id);
            break;
        case SPAWN_MONSTER:
            spawnMonster(event.id, event.x/sb, event.y/sb);
            setMonster(monsterCounter-1, event.x, event.y);
            break;
        case SPAWN_WALL:
            spawnWalls(Point<int>(event.x, event.y));
            break;
        case RESPAWN_PLAYER:
            spawnPlayer(event.id);
            break;
        case MAP_BUILD:
            table(event.x, event.y) = GROUND(event.id);
            break;
        case SCOREBOARD_DATA:
        {
            int type = event.x;
            int value = event.y;
            if (type == 0) waitRound = true;
            if (type == 1) players[event.id].setScore(value);
            if (type == 2) players[event.id].setKill(value);
            if (type == 3) players[event.id].setKilled(value);
            if (type == 4) players[event.id].setMonsterKill(value);
        }
            break;
        default:
            std::cout << NETWORK_TRACE[(int)event.e] << " not implemented yet\n";
            break;
    }
}

std::vector<NetworkEvent>& GameTable::getMovement()
{
    return networkEvents;
}

void GameTable::networkMoves()
{
    for(std::vector<Player>::iterator i = players.begin(); i != players.end(); ++i)
    {
        Point<float> p;
        if (i->getMove(p) && (!i->isRemotePlayer() || calc))
        {
            networkEvent(MOVEMENT, i->getId(), p.getX(), p.getY());
        }
    }
    if (calc)
    {
        for(std::deque<Monster>::iterator i = monsters.begin(); i != monsters.end(); ++i)
        {
            Point<float> p;
            if (i->getMove(p))
            {
                networkEvent(MONSTER_MOVEMENT, i->getId(), i->getNetworkX(), i->getNetworkY());
            }
        }
    }
}

void GameTable::networkEvent(EVENT_TYPE e, char id, float x, float y)
{
    // std::cout << "NET " << NETWORK_TRACE[(int)e] << " " << (int)id << " " << x << " " << y << std::endl;
    networkEvents.push_back(NetworkEvent(e,id,x,y));
}
