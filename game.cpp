#include "game.h"

Game::Game()
{
    initGame();

    //network.getInternetList();
    Menu menu(view,input);
    menu.tween();

    while(input.getRun())
    {
        for(view.frameBegin(); menu.mainLoop(); view.frameEnd());
        if(input.getRun() == false) break;

        GameTable table(view);

        GameManager manager(menu.getMode(), menu.getPlace(), view, input, table, network);

        while(manager.getRun())
        {
            manager.newRound();
            while(manager.getRound())
            {
                network.mainLoop();
                view.frameBegin();
                input.poll(view.frameDelay());
                table.mainLoop();
                table.draw();
                view.swap();
                manager.checkState();
                view.frameEnd();
            }

            if (manager.getShowScoreBoard())
            {
                ScoreBoard scoreBoard(view,input,table);
                for(view.frameBegin(); scoreBoard.mainLoop(); view.frameEnd());
                if (input.getRun()) input.setRun(!scoreBoard.getEndOfGame());
            }
        }
    }
}

Game::~Game()
{
}

void Game::initGame()
{
    srand(time(0));
    SDL_WM_SetCaption("Bombás játék","Bombás játék");
    view.setTickInterval(16);
    input.setView(&view);

    view.loadPackage("img/img.ls");
}
