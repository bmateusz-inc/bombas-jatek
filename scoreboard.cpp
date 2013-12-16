#include "scoreboard.h"

ScoreBoard::ScoreBoard(View &iView, Input &iInput, GameTable &iTable)
                     : view(iView), input(iInput), table(iTable)
{
    view.setNow();
    view.stopAnim();
    stop = view.getNow() + Config::getInstance().CFG_SHOW_SCOREBOARD;
    view.draw(0,0,640,480,"fade_in.anm");
    skip = false;
    endOfGame = false;

    mainLoop();
    if (endOfGame) stop = view.getNow() + 60000;
}

bool ScoreBoard::mainLoop()
{
    view.setNow();
    
    if(view.getNow() > stop || !input.getRun())
    {
        view.stopAnim();
        return false;
    }
    
    input.poll(0);
    if (!skip && (( (table.getConfig().place == LOCAL || endOfGame) && table.isAnyPlayerPut())
                  || (stop < view.getNow() + Config::getInstance().CFG_WAIT_ROUND)))
    {
        stop = view.getNow() + Config::getInstance().CFG_WAIT_ROUND;
        view.draw(0,0,640,480,"fade_out.anm");
        skip = true;
    }
    
    view.draw(0,0,640,480,"menu.png");
    
    // GAME_GOAL goal = table.getGoal();
    
    view.draw(100,15,500,25,"border.png");

    view.draw(160-20,20,10,20,"font.txh#Győzelem");
    view.draw(280-20,20,10,20,"font.txh#Megölt");
    view.draw(400-20,20,10,20,"font.txh#Meghalt");
    view.draw(520-20,20,10,20,"font.txh#Szörny");
    
    int n = table.getNPlayers();
    for(int i=0; i <= n; ++i)
    {
        view.draw(20,40+i*80,600,10,"border.png#r");
        if(i<n)
        {
            std::string background = "tween.mlt#16 0";
            if (table.getPlayers()[i].getScore() >= Config::getInstance().CFG_SCORE_LIMIT)
            {
                background = "green.png";
                endOfGame = true;
            }
            view.draw(90,50+i*80,520,70,background);
            view.draw(35,55+i*80,40,60,table.getPlayers()[i].getSkin() + "_d.png");
            /*for(int j=0; j<table.getPlayers()[i].getScore(); ++j)
            {
                view.draw(110+j*40,35+i*80,30,60,"win.png");
            }*/
            
            std::stringstream points;
            points << table.getPlayers()[i].getScore();
            std::stringstream kill;
            kill << table.getPlayers()[i].getKill();
            std::stringstream killed;
            killed << table.getPlayers()[i].getKilled();
            std::stringstream monster;
            monster << table.getPlayers()[i].getMonsterKill();
            
            view.draw(160,56+i*80,30,60,"font_white.txh#"+points.str());
            view.draw(280,56+i*80,30,60,"font_white.txh#"+kill.str());
            view.draw(400,56+i*80,30,60,"font_white.txh#"+killed.str());
            view.draw(520,56+i*80,30,60,"font_white.txh#"+monster.str());
        }
    }

    view.draw(20,40,10,n*80+10,"border.png");
    view.draw(80,40,10,n*80+10,"border.png");
    view.draw(610,40,10,n*80+10,"border.png");
    
    std::stringstream ss;
    if (endOfGame) ss << "Vége a játéknak! Nyomd meg az entert a továbblépéshez.";
    else if (table.getConfig().place == LOCAL) ss << "Nyomd meg a tűz gombot a következő körhöz.";
    view.draw(0, 445, 640, 30, "tween.mlt#16 0");
    view.draw(20, 450, 10, 20, "font_white.txh#" + ss.str());
    
    view.drawAnim();
    view.swap();
    return true;
}