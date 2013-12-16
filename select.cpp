#include "select.h"
#include "textinput.h"

Select::Select(View &iview, Input &iinput, GameTable &itable, Network &iNetwork,
               GAME_MODE igameMode, GAME_PLACE igamePlace)
        : view(iview), input(iinput), table(itable), network(iNetwork),
          gameMode(igameMode), gamePlace(igamePlace)
{
    run = true;
    hasError = false;
    clientId = 0;
    input.updateJoySticks();
    input.noPut();

    if (gamePlace == LOCAL)
    {
        n = input.getNControllers();
        table.setPlayers(n);
        team.resize(n);
        place.resize(n);
        for(int i=0; i < n; ++i)
            table.bind(i,input(i),0,"");
    } else {
        n = 1;
        team.resize(16);
    }
    
    if (gamePlace == CLIENT)
    {
        searchServer = true;
        table.setCalc(false);
    }
    
    switch (gameMode)
    {
        case SINGLE:
            minimalPlayers = 1;
            break;
        default:
            minimalPlayers = 2;
            break;
    }
    
    for(view.frameBegin(); mainLoop(); view.frameEnd());
}

void Select::loopLocal()
{
    for(int i=0; i<n; ++i)
    {
        if(input(i)->getPut()) {
            int count = 0;
            for (std::vector<int>::const_iterator it = team.begin(); it != team.end(); ++it)
            {
                if ((*it) != 0) count++;
            }
            if (count >= minimalPlayers)
            {
                run = false;
                input(i)->setPut(false);
            }
        }
        if(input(i)->getX() < 0) {
            team[i] = 1;
        }
        if(input(i)->getX() > 0) {
            team[i] = 2;
        }
        if(input(i)->getY() > 0) {
            team[i] = 0;
        }
        
        std::stringstream ss;
        const int offset = 60;
        view.draw(0, 45+offset*i+25, 640, 40, "tween.mlt#16 0");
        ss << "Játékos " << (i+1) << ": ";
        view.draw(70,55+offset*i+25,10,20,"font_white.txh#" + ss.str());
        
        ss.str(std::string()); // empty
        if (team[i] == 1) ss << "test_d.png";
        if (team[i] == 2) ss << "b_test_d.png";
        view.draw(190,30+offset*i+25,40,55,ss.str());
        if (i < 2) view.draw(20,40+offset*i+25,40,40,"keyboard.png");
        else       view.draw(20,40+offset*i+25,40,40,"joystick.png");
        
        ss.str(std::string()); // empty
        if (i == 0) ss << "Nyilak Jobb CTRL";
        if (i == 1) ss << "  WASD TAB";
        view.draw(420,55+offset*i+25,10,20,"font_white.txh#" + ss.str());
    }
    
    if (run == false) bindPlayers();
}

void Select::loopServer()
{
    int newN = network.getServer().getNPlayers();
    if (n != newN)
    {
        std::vector<NetworkEvent> events;
        events.push_back(NetworkEvent(SET_NPLAYERS,newN,0,0));
        events.push_back(NetworkEvent(SET_MODE,gameMode,0,0));
        for (int i=0; i<n; ++i) events.push_back(NetworkEvent(SET_TEAM,i,team[i],0));
        network.sendPackets(events);
    }
    n = newN;
    
    std::vector<NetworkEvent> packets = network.getPackets();
    for (std::vector<NetworkEvent>::iterator it = packets.begin(); it != packets.end(); ++it)
    {
        if (it->e == SET_TEAM)
        {
            team[it->id] = (int)(it->x);
        }
    }
    network.sendPackets(packets);
    
    if (input()->getPut())
    {
        input.noPut();
        std::vector<NetworkEvent> events;
        events.push_back(NetworkEvent(SET_START,gameMode,0,0));
        network.sendPackets(events);
        /*table.setPlayers(n);
        table.bind(0,input(),team[0],"test");
        for (int i=1; i<n; ++i) table.bind(i, 0, team[i], "test");*/
        bindPlayers();
        run = false;
    }
    network.mainLoop();
    if (!network.getSuccess()) input.setRun(false);
    
    drawNetwork();
}

void Select::loopClient()
{
    network.mainLoop();

    std::vector<NetworkEvent> packets = network.getPackets();
    for (std::vector<NetworkEvent>::iterator it = packets.begin(); it != packets.end(); ++it)
    {
        if (it->e == SET_NPLAYERS)
        {
            n = it->id;
        }
        else if (it->e == SET_TEAM)
        {
            team[it->id] = (int)(it->x);
        }
        else if (it->e == SET_MODE)
        {
            gameMode = GAME_MODE(it->id);
        }
        else if (it->e == SET_NETWORK_ID)
        {
            clientId = it->id;
        }
        else if (it->e == SET_START)
        {
            printf("Start game\n");
            /*table.setPlayers(n);
            for (int i=0; i<n; ++i)
            {
                if (i == clientId) table.bind(i, input(), 1, "test");
                else table.bind(i, 0, 1, "test");
            }*/
            gameMode = GAME_MODE(it->id);
            bindPlayers();
            run = false;
        }
        else
        {
            printf("Unrecognized packet\n");
        }
    }
    

    drawNetwork();
}

void Select::drawNetwork()
{
    int originalTeam = team[clientId];
    if(input()->getX() < 0) {
        team[clientId] = 1;
    }
    if(input()->getX() > 0) {
        team[clientId] = 2;
    }
    if(input()->getY() > 0) {
        team[clientId] = 0;
    }
    
    if (team[clientId] != originalTeam)
    {
        std::vector<NetworkEvent> events;
        events.push_back(NetworkEvent(SET_TEAM,clientId,team[clientId],0));
        network.sendPackets(events);
    }
        
    for(int i=0; i <= n; ++i)
    {
        view.draw(20,90+i*80,600,10,"border.png#r");
        if(i<n)
        {
            view.draw(90,100+i*80,520,70,"tween.mlt#16 0");
            
            std::stringstream ss; // empty
            if (team[i] == 1) ss << "test_d.png";
            if (team[i] == 2) ss << "b_test_d.png";
        
            view.draw(35,105+i*80,40,60,ss.str());
        }
    }

    view.draw(20,90,10,n*80+10,"border.png");
    view.draw(80,90,10,n*80+10,"border.png");
    view.draw(610,90,10,n*80+10,"border.png");
}

void Select::loopClientSearch()
{
    TextInput textInput(view);
    while (textInput.mainLoop());
    network.connect(textInput.getInput());
    input.noPut();
    if (network.getSuccess()) searchServer = false;
    else
    {
        searchServer = false;
        hasError = true;
        //input.setRun(false);
    }
}

bool Select::mainLoop()
{
    view.setNow();
    view.draw(0,0,640,480,"menu.png");
    input.poll(0);
    
    if (gamePlace == LOCAL) loopLocal();
    else if (gamePlace == SERVER) loopServer();
    else if (gamePlace == CLIENT && searchServer) loopClientSearch();
    else if (gamePlace == CLIENT && !searchServer) loopClient();
    
    std::stringstream ss;
    std::stringstream modeDesc;
    switch (gameMode)
    {
        case SINGLE: ss <<  "Egyjátékos"; modeDesc << "Játszd végig mind az öt pályát!"; break;
        case CLASSIC: ss << "Klasszikus"; modeDesc << "Maradj életben utolsóként! Ponthatár: " << Config::getInstance().CFG_SCORE_LIMIT; break;
        case TEAM: ss <<    "Csapat";     modeDesc << "Öljetek meg minél több ellenséget! Ponthatár: " << Config::getInstance().CFG_KILL_LIMIT; break;
        case SURVIVE: ss << "Vadászat";   modeDesc << "Ölj meg minél több szörnyet! Ponthatár: " << Config::getInstance().CFG_MONSTER_KILL_LIMIT; break;
        default: break;
    }
    ss << " - ";
    switch (gamePlace)
    {
        case LOCAL: ss << "Helyi játék"; break;
        case CLIENT: ss << network.getClient().getHost(); break;
        case SERVER: ss << "Szerver (az indításhoz nyomd meg az Entert)"; break;
        default: break;
    }
    view.draw(0, 5, 640, 30, "tween.mlt#16 0");
    view.draw(20, 10, 10, 20, "font_white.txh#" + ss.str());
    
    view.draw(0, 445, 640, 30, "tween.mlt#16 0");
    view.draw(20, 450, 10, 20, "font_white.txh#Csapatválasztáshoz használd a balra/jobbra nyilakat");
    
    view.draw(0, 410, 640, 30, "tween.mlt#16 0");
    view.draw(20, 415, 10, 20, "font_white.txh#" + modeDesc.str());
    
    view.swap();
    
    if(run && input.getRun() && !hasError)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Select::bindPlayers()
{
    if (gamePlace == LOCAL)
    {
        int count = 0;
        for (std::vector<int>::iterator it = team.begin(); it != team.end(); it++)
        {
            if ((*it) != 0)
            {
                count++;
            }
        }

        if (count == 0)
        {
            input.setRun(false);
            return;
        }

        table.setPlayers(count);
        team.resize(count);
        place.resize(count);

        count = 0;
        int i = 0;
        for (std::vector<int>::iterator it = team.begin(); it != team.end(); it++)
        {
            if ((*it) != 0)
            {
                std::string skin = "test";
                if (team[i] == 2) skin = "b_test";
                table.bind(i, input(count), team[i], skin);

                count++;
            }
            i++;
        }
    }
    else // SERVER || CLIENT
    {
        table.setPlayers(n);
        for (int i=0; i<n; ++i)
        {
            Control *ptrInput = 0;
            if (i == clientId) ptrInput = input();
            std::string skin = "test";
            int curTteam = 1;
            
            if (team[i] == 2)
            {
                skin = "b_test";
                curTteam = 2;
            }
            table.bind(i, ptrInput, curTteam, skin);
        }
    }
}