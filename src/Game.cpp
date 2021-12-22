#include "Game.h"

Game Game::m_instance=Game();

Game::Game()
{
    //ctor
    //load the background image in the texture
    backgroundTexture.loadFromFile(BACKGROUND_FRAME);
    //set the texture on the sprite
    backgroundSprite[0].setTexture(backgroundTexture);
    backgroundSprite[1].setTexture(backgroundTexture);
    //set the scale of the background
    backgroundSprite[1].setScale(-1,1);
    //set the position of the background
    backgroundSprite[1].setPosition(974,0);//489

	gameState = menuStart;
	gameMode = hard;
}

Game::~Game()
{
    //dtor
}

Game::Game(const Game& other)
{
    backgroundTexture=other.backgroundTexture;
    backgroundSprite[0]=other.backgroundSprite[0];
    backgroundSprite[1]=other.backgroundSprite[1];
 	gameState = other.gameState;
	gameMode = other.gameMode;
}
Game& Game::Instance()
{
    //return the only instance of the game class (design pattern singleton)
    return m_instance;
}
void Game::drawBackground(RenderWindow &app)
{
    //draw the sprite
    app.draw(backgroundSprite[0]);
    app.draw(backgroundSprite[1]);
}
//set the gamemode , gamestate
void Game::setGameMode(GameMode gm)
{
    gameMode = gm;
}
void Game::setGameState(GameState gs)
{
    gameState = gs;
}
//get the gamemode , gamestate
GameMode Game::getGameMode()
{
    return gameMode;
}
GameState Game::getGameState()
{
    return gameState;
}
void Game::nextFrame()
{
    //+one frame while the window is open
    frames++;
}
int Game::getFrames()
{
    //get the number of frames of the game
    return frames;
}
