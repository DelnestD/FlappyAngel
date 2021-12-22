#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>

#include "GameState.h"
#include "GameMode.h"
#include "Definitions.h"

using namespace sf;
using namespace std;
//The class Game allow to set the current state of the game as well as the current game mode of the game
//It also gives the number of frame of the game
class Game
{
    public:
        static Game& Instance();
        void drawBackground(RenderWindow &app);
        void setGameState(GameState gs);
        void setGameMode(GameMode gm);
        GameState getGameState();
        GameMode getGameMode();
        void nextFrame();
        int getFrames();

    protected:

    private:
        Game();
        virtual ~Game();
        Game(const Game& other);
        //one instance of the class game
        static Game m_instance;
        int frames = 0;
        GameState gameState;//enum in GameState
        GameMode gameMode;//enum in GameMode
        Texture backgroundTexture;
        Sprite backgroundSprite[2];
};

#endif // GAME_H
