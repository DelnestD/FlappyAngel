#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include "Audio.h"
#include "Angel.h"
#include "Game.h"
#include "GameState.h"
#include "Definitions.h"
#include "MenuType.h"
#include "Menu.h"

using namespace sf;
using namespace std;

bool collides(float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2) {
	if (x1 + w1 >= x2 && x1 <= x2 + w2 && y1 + h1 >= y2 && y1 <= y2 + h2) {
		return true;
	}
	return false;
}

int main()
{
    //creating the window
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), NAME_GAME);
	window.setFramerateLimit(WINDOW_FRAMELIMIT);
	window.setKeyRepeatEnabled(false);

	int winCondition = WIN_NUMBER_HARD; //hard by default
	int obstaclePassed = 0;
	int gapFire = GAP_FIRE_HARD;
	int distanceFire = DISTANCE_FIRE_HARD;
	bool settingsMainMenu = false;

	srand(time(0));
	Event e;

	//creating the different menu
	Menu menu(window.getSize().x,window.getSize().y,"PLAY","SETTINGS","EXIT",startMenuType);
    Menu difficulties(window.getSize().x,window.getSize().y,"EASY","HARD","BACK",normalMenuType);
    Menu breakMenu(window.getSize().x,window.getSize().y,"CONTINUE","SETTINGS","BACK TO MENU",breakMenuType);
    Menu gameOverMenu(window.getSize().x,window.getSize().y,"","RETRY","BACK TO MENU",gameOverMenuType);
    Menu victoryMenu(window.getSize().x,window.getSize().y,"","BACK TO LEVEL","EXIT GAME",victoryMenuType);
    Menu settings(window.getSize().x,window.getSize().y,"Music : 100%","Sound effects : 100%","BACK",normalMenuType);
    //creation of the instance of the audio
	Audio& audio = Audio::Instance();
	Angel angel;
	Texture fireTexture;
	fireTexture.loadFromFile(FIRE_FRAME);
	//vector of sprite containing the fire obstacles
	vector<Sprite> fires;
	//creation of the instance of the game
	Game& game = Game::Instance();

	audio.playMusicMenu();

	while (window.isOpen()) {

	    //Creation of the game

        //update angel
        float ax = angel.getPositionX();
        float ay = angel.getPositionY();
        float aw = 34 * angel.getScaleX();
        float ah = 24 * angel.getScaleY();

        //flap wings if playing (based on the angel frame)
        if(game.getGameState() == waiting || game.getGameState() == started){
            if(game.getFrames() % 6 == 0){
                angel.nextFrame();
            }
            if(angel.getFrame() == 3){//only 3 frames because there are 3 items in the texture vector
                angel.resetFrame();
            }
        }
        angel.setTexture(angel.getFrame());

        //move angel
        if (game.getGameState() == started) {
			angel.move();
		}

        //if hit top of the screen, stop angel's ascens
        //if hit bottom of the screen, game over
		if (game.getGameState() == started) {
			if (ay < 0) {
				angel.setPosition(250, 0);
				angel.setV(0);
			} else if (ay > 800) {
				angel.setV(0);
				audio.playSoundHit();
				game.setGameState(gameover);
				audio.playMusicMenu();
			}
		}

		//count obstacle passed
		for (vector<Sprite>::iterator itr = fires.begin(); itr != fires.end(); itr++) {
			if (game.getGameState() == started && (*itr).getPosition().x == 250) {

				obstaclePassed++;
				//if the number of obstacle passed is >= the number of column to win for the mode -> win the game
				if(obstaclePassed < winCondition){
                    audio.playSoundPass();
				}else{
                    audio.playMusicMenu();
                    audio.playSoundVictory();
                    game.setGameState(victory);
				}
				break;
			}
		}

        //generate fire obstacle
		if (game.getGameState() == started && game.getFrames() % distanceFire == 0) {
			int r = rand() % 275 + 75;

			// lower fire obstacles
			Sprite fireL;
			fireL.setTexture(fireTexture);
			fireL.setPosition(1000, r + gapFire);
			fireL.setScale(2, 2);

			// upper fire obstacles
			Sprite fireU;
			fireU.setTexture(fireTexture);
			fireU.setPosition(1000, r);
			fireU.setScale(2, -2);

			// push to the array
			fires.push_back(fireL);
			fires.push_back(fireU);
		}

		// move fires obstacles
		if (game.getGameState() == started) {
			for (vector<Sprite>::iterator itr = fires.begin(); itr != fires.end(); itr++) {
				(*itr).move(-3, 0);
			}
		}

		// remove fires obstacles if out of screen
		if (game.getFrames() % 100 == 0) {
			vector<Sprite>::iterator startitr = fires.begin();
			vector<Sprite>::iterator enditr = fires.begin();

			for (; enditr != fires.end(); enditr++) {
				if ((*enditr).getPosition().x > -104) {
					break;
				}
			}

			fires.erase(startitr, enditr);
		}

		// collision detection
		if (game.getGameState() == started) {
			for (vector<Sprite>::iterator itr = fires.begin(); itr != fires.end(); itr++) {

				float fx, fy, fw, fh;

				if ((*itr).getScale().y > 0) {
					fx = (*itr).getPosition().x;
					fy = (*itr).getPosition().y;
					fw = 52 * (*itr).getScale().x;
					fh = 320 * (*itr).getScale().y;
				} else {
					fw = 52 * (*itr).getScale().x;
					fh = -320 * (*itr).getScale().y;
					fx = (*itr).getPosition().x;
					fy = (*itr).getPosition().y - fh;
				}
                //if the vertical and horizontal position of the angel and the obstacle are the same or higher -> gameover
				if (collides(ax, ay, aw, ah, fx, fy, fw, fh)) {
                    audio.playSoundHit();
					game.setGameState(gameover);
					audio.playMusicMenu();
				}
			}
		}

        //-------------------MENUS----------------------------
    while (window.pollEvent(e)){

        if (e.type == Event::Closed) {
				window.close();
        }
        //Press space bar or left click to start te game
        if ((e.type == Event::KeyPressed && e.key.code == Keyboard::Space) || (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)) {
            if (game.getGameState() == waiting) {//gamestate : waiting -> started
                game.setGameState(started);
            }

            if (game.getGameState() == started) {
                //to make look like the angel is ascending
                //the angel vertical position will decrease of 8 each time the left click/ space bar is pressed
                angel.setV(NUMBER_ANGEL_ASCENDING);//-8
                audio.playSoundWing();
            }
        }
        //MAIN MENU
        if(game.getGameState() == menuStart){
                //keyboard event / up arrow /down arrow /enter
                if (e.type == Event::KeyReleased){
                    //up and down event for moving between menu item
                    switch(e.key.code){
                    case Keyboard::Up:
                        audio.playSoundSwitchButton();
                        menu.moveUp();
                        break;
                    case Keyboard::Down:
                        audio.playSoundSwitchButton();
                        menu.moveDown();
                        break;
                    case Keyboard::Return:
                        //do something according to the menu item selected
                        switch(menu.getSelectedItemId()){
                            case 0:
                                //move to menu of difficulties
                                audio.playSoundButton();
                                game.setGameState(menuDifficulties);
                                break;
                            case 1:
                                //move to settings menu
                                settingsMainMenu = true;
                                audio.playSoundButton();
                                game.setGameState(menuSettings);
                                break;
                            case 2:
                                //exit game
                                audio.playSoundButton();
                                window.close();
                                break;
                        }
                    }
                }
                //mouse click event
                else if(e.type == Event::MouseButtonPressed){
                    if(Mouse::isButtonPressed(Mouse::Left)){
                        Vector2f mouse_position = window.mapPixelToCoords(Mouse::getPosition(window));
                        if(mouse_position.x >=MOUSE_POSITION_X_LEFT && mouse_position.x <=MOUSE_POSITION_X_RIGHT  ){
                            if(mouse_position.y >= MOUSE_POSITION_Y_ITEM1_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM1_RIGHT){
                                //move to menu of difficulties
                                audio.playSoundButton();
                                game.setGameState(menuDifficulties);
                            }
                            else if(mouse_position.y >= MOUSE_POSITION_Y_ITEM2_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM2_RIGHT){
                                //move to settings menu
                                settingsMainMenu = true;
                                audio.playSoundButton();
                                game.setGameState(menuSettings);
                            }
                            else if(mouse_position.y >= MOUSE_POSITION_Y_ITEM3_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM3_RIGHT){
                                //exit game
                                audio.playSoundButton();
                                window.close();
                            }
                        }
                    }
                }
            }
            //MENU OF DIFFICULTIES
            else if(game.getGameState() == menuDifficulties){
                //keyboard event / up arrow /down arrow /enter
                if (e.type == Event::KeyReleased){
                    switch(e.key.code){
                    case Keyboard::Up:
                        audio.playSoundSwitchButton();
                        difficulties.moveUp();
                        break;
                    case Keyboard::Down:
                        audio.playSoundSwitchButton();
                        difficulties.moveDown();
                        break;
                    case Keyboard::Return:
                          //do something according to the menu item selected
                        switch(difficulties.getSelectedItemId()){
                            case 0:
                                //gamemode = easy
                                //gamestate waiting, start of the game
                                audio.playSoundButton();
                                game.setGameMode(easy);
                                game.setGameState(waiting);
                                obstaclePassed = 0;
                                fires.clear();
                                audio.playMusicGame();
                                angel.setPosition(250,300);
                                break;
                            case 1:
                                //gamemode hard
                                //gamestate waiting, start of the game
                                audio.playSoundButton();
                                game.setGameMode(hard);
                                game.setGameState(waiting);
                                obstaclePassed = 0;
                                fires.clear();
                                audio.playMusicGame();
                                angel.setPosition(250,300);
                                break;
                            case 2:
                                //back to main menu
                                audio.playSoundButton();
                                game.setGameState(menuStart);
                                menu.resetSelectedItem();
                                difficulties.resetSelectedItem();
                                break;
                        }
                    }
                }
                //mouse click event
                if(e.type == Event::MouseButtonPressed){
                     if(Mouse::isButtonPressed(Mouse::Left)){
                        Vector2f mouse_position = window.mapPixelToCoords(Mouse::getPosition(window));
                        if(mouse_position.x >=MOUSE_POSITION_X_LEFT && mouse_position.x <=MOUSE_POSITION_X_RIGHT  ){
                            if(mouse_position.y >= MOUSE_POSITION_Y_ITEM1_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM1_RIGHT){
                                //gamemode = easy
                                //gamestate waiting, start of the game
                                audio.playSoundButton();
                                game.setGameMode(easy);
                                game.setGameState(waiting);
                                obstaclePassed = 0;
                                fires.clear();
                                angel.setPosition(250,300);
                                audio.playMusicGame();
                            }
                            else if(mouse_position.y >= MOUSE_POSITION_Y_ITEM2_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM2_RIGHT){
                                //gamemode hard
                                //gamestate waiting, start of the game
                                audio.playSoundButton();
                                game.setGameMode(hard);
                                game.setGameState(waiting);
                                obstaclePassed = 0;
                                fires.clear();
                                angel.setPosition(250,300);
                                audio.playMusicGame();
                            }
                            else if(mouse_position.y >= MOUSE_POSITION_Y_ITEM3_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM3_RIGHT){
                                //back to main menu
                                audio.playSoundButton();
                                game.setGameState(menuStart);
                                menu.resetSelectedItem();
                                difficulties.resetSelectedItem();
                            }
                        }
                    }
                 }
            }
            //SETTINGS MENU (sound)
            else if(game.getGameState() == menuSettings){
                //keyboard event / up arrow /down arrow/ left arrow / right arrow/ enter
                if (e.type == Event::KeyReleased){
                    switch(e.key.code){
                    case Keyboard::Up:
                        audio.playSoundSwitchButton();
                        settings.moveUp();
                        break;
                    case Keyboard::Down:
                        audio.playSoundSwitchButton();
                        settings.moveDown();
                        break;
                    case Keyboard::Right:
                        //rise the volume by 10
                        settings.moveRight();
                        audio.setVolumeMusic(settings.getVolumeMusic());
                        audio.setVolumeSound(settings.getVolumeSound());
                        break;
                    case Keyboard::Left:
                        //low the volume by 10
                        settings.moveLeft();
                        audio.setVolumeMusic(settings.getVolumeMusic());
                        audio.setVolumeSound(settings.getVolumeSound());
                        break;
                    case Keyboard::Return:
                        //do something according to the menu item selected
                        switch(settings.getSelectedItemId()){
                            case 0:
                                //cut music
                                settings.cutMusic();
                                audio.setVolumeMusic(settings.getVolumeMusic());
                                break;
                            case 1:
                                //cut sound effects
                                 settings.cutSoundEffect();
                                 audio.setVolumeSound(settings.getVolumeSound());
                                break;
                            case 2:
                                //back to main menu if not in a game
                                //back to break menu if in game
                                if(settingsMainMenu){
                                    audio.playSoundButton();
                                    game.setGameState(menuStart);
                                    menu.resetSelectedItem();
                                    settings.resetSelectedItem();
                                    settingsMainMenu = false;
                                }
                                else{
                                    audio.playSoundButton();
                                    game.setGameState(onBreak);
                                    breakMenu.resetSelectedItem();
                                    settings.resetSelectedItem();
                                }
                                break;
                        }
                    }
                }
                //mouse click event
                 if(e.type == Event::MouseButtonPressed){
                     if(Mouse::isButtonPressed(Mouse::Left)){
                        Vector2f mouse_position = window.mapPixelToCoords(Mouse::getPosition(window));
                        if(mouse_position.x >=MOUSE_POSITION_X_LEFT && mouse_position.x <=MOUSE_POSITION_X_RIGHT  ){
                            if(mouse_position.y >= MOUSE_POSITION_Y_ITEM1_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM1_RIGHT){
                                    //cut music
                                    settings.setSelectedItemId(0);
                                    settings.cutMusic();
                                    audio.setVolumeMusic(settings.getVolumeMusic());

                            }
                            else if(mouse_position.y >= MOUSE_POSITION_Y_ITEM2_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM2_RIGHT){
                                    //cut sound effects
                                    settings.setSelectedItemId(1);
                                    settings.cutSoundEffect();
                                    audio.setVolumeSound(settings.getVolumeSound());

                            }
                            else if(mouse_position.y >= MOUSE_POSITION_Y_ITEM3_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM3_RIGHT){
                                //back to main menu if not in a game
                                //back to break menu if in game
                                if(settingsMainMenu){
                                    audio.playSoundButton();
                                    game.setGameState(menuStart);
                                    menu.resetSelectedItem();
                                    settings.resetSelectedItem();
                                    settingsMainMenu = false;
                                }
                                else{
                                    audio.playSoundButton();
                                    game.setGameState(onBreak);
                                    breakMenu.resetSelectedItem();
                                    settings.resetSelectedItem();
                                }
                            }
                        }
                    }
                 }
            }
            //BREAK MENU
            else if(game.getGameState() == onBreak){
                //keyboard event / up arrow /down arrow /enter
                if (e.type == Event::KeyReleased){

                        switch(e.key.code){
                        case Keyboard::Up:
                            audio.playSoundSwitchButton();
                            breakMenu.moveUp();
                            break;
                        case Keyboard::Down:
                            audio.playSoundSwitchButton();
                            breakMenu.moveDown();
                            break;
                        case Keyboard::Return:
                            //do something according to the menu item selected
                            switch(breakMenu.getSelectedItemId()){
                                case 0:
                                    //continue the game -> gamestate in waiting
                                    audio.playSoundButton();
                                    breakMenu.resetSelectedItem();
                                    game.setGameState(waiting);
                                    break;
                                case 1:
                                    //go to settings menu
                                    audio.playSoundButton();
                                    game.setGameState(menuSettings);
                                    //settings
                                    break;
                                case 2:
                                    //back to main menu
                                    audio.playSoundButton();
                                    audio.playMusicMenu();
                                    game.setGameState(menuStart);
                                    difficulties.resetSelectedItem();
                                    breakMenu.resetSelectedItem();
                                    break;
                            }
                        }
                }
                else if(e.type == Event::MouseButtonPressed){
                     if(Mouse::isButtonPressed(Mouse::Left)){
                        Vector2f mouse_position = window.mapPixelToCoords(Mouse::getPosition(window));
                        if(mouse_position.x >=MOUSE_POSITION_X_LEFT && mouse_position.x <=MOUSE_POSITION_X_RIGHT ){
                            if(mouse_position.y >= MOUSE_POSITION_Y_ITEM1_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM1_RIGHT){
                                //continue the game -> gamestate in waiting
                                audio.playSoundButton();
                                breakMenu.resetSelectedItem();
                                game.setGameState(waiting);
                            }
                            else if(mouse_position.y >= MOUSE_POSITION_Y_ITEM2_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM2_RIGHT){
                                //go to settings menu
                                 audio.playSoundButton();
                                 game.setGameState(menuSettings);
                                //settings
                            }
                            else if(mouse_position.y >= MOUSE_POSITION_Y_ITEM3_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM3_RIGHT){
                                //back to main menu
                                audio.playSoundButton();
                                audio.playMusicMenu();
                                game.setGameState(menuStart);
                                difficulties.resetSelectedItem();
                                breakMenu.resetSelectedItem();
                            }
                        }
                    }
                }

            }
            //VICTORY MENU
            else if(game.getGameState() == victory){
                //keyboard event / up arrow /down arrow /enter
                if (e.type == Event::KeyReleased){
                    switch(e.key.code){
                        case Keyboard::Up:
                            audio.playSoundSwitchButton();
                            victoryMenu.moveUp();
                        break;
                        case Keyboard::Down:
                            audio.playSoundSwitchButton();
                            victoryMenu.moveDown();
                        break;
                        case Keyboard::Return:
                            //do something according to the menu item selected
                            switch(victoryMenu.getSelectedItemId()){
                                case 1:
                                    //go to menu of difficulties
                                    audio.playSoundButton();
                                    game.setGameState(menuDifficulties);
                                    difficulties.resetSelectedItem();
                                    breakMenu.resetSelectedItem();
                                    menu.resetSelectedItem();
                                    victoryMenu.setSelectedItemId(1);
                                break;
                                case 2:
                                    //exit game
                                    audio.playSoundButton();
                                    window.close();
                                break;

                            }
                    }
                }
                else if(e.type == Event::MouseButtonPressed){
                    if(Mouse::isButtonPressed(Mouse::Left)){
                        Vector2f mouse_position = window.mapPixelToCoords(Mouse::getPosition(window));
                        if(mouse_position.x >=MOUSE_POSITION_X_LEFT && mouse_position.x <=MOUSE_POSITION_X_RIGHT  ){
                            if(mouse_position.y >= MOUSE_POSITION_Y_ITEM2_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM2_RIGHT){
                                //go to menu of difficulties
                                audio.playSoundButton();
                                game.setGameState(menuDifficulties);
                                difficulties.resetSelectedItem();
                                breakMenu.resetSelectedItem();
                                menu.resetSelectedItem();
                                victoryMenu.setSelectedItemId(1);
                            }
                            else if(mouse_position.y >= MOUSE_POSITION_Y_ITEM3_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM3_RIGHT){
                                //exit game
                                audio.playSoundButton();
                                window.close();

                            }
                        }
                    }
                }
            }

        //---------------------GAME---------------------------

        if(game.getGameState() == waiting || game.getGameState() == started){
            //configure gamemode
            if(game.getGameMode() == easy){
                winCondition = WIN_NUMBER_EASY;//need to pass 8 column to win
                gapFire = GAP_FIRE_EASY;//250
                distanceFire = DISTANCE_FIRE_EASY;//250
            }else{
                winCondition = WIN_NUMBER_HARD;//need to pass 15 column to win
                gapFire = GAP_FIRE_HARD;//225
                distanceFire = DISTANCE_FIRE_HARD;//150
            }
            //flap wings of the angel (based on the angel frame)
            if(game.getFrames() % 6 == 0){
                angel.nextFrame();
            }
            if(angel.getFrame() == 3){//only 3 frames because there are 3 items in the texture vector
                angel.resetFrame();
            }

            angel.setTexture(angel.getFrame());
            //during the game if the key "esc" is pressed set the gamestate on break -> onbreak menu
            if(e.type == Event::KeyPressed && e.key.code == Keyboard::Escape){
                game.setGameState(onBreak);
            }
        }
        //GAMEOVER MENU
        else if(game.getGameState() == gameover){
            //keyboard event / up arrow /down arrow /enter
            if (e.type == Event::KeyReleased){
                switch(e.key.code){
                    case Keyboard::Up:
                        audio.playSoundSwitchButton();
                        gameOverMenu.moveUp();
                    break;
                    case Keyboard::Down:
                        audio.playSoundSwitchButton();
                        gameOverMenu.moveDown();
                    break;
                    //do something according to the menu item selected
                    case Keyboard::Return:
                        switch(gameOverMenu.getSelectedItemId()){
                            case 1:
                                //retry -> gamestate in waiting
                                audio.playSoundButton();
                                audio.playMusicGame();
                                game.setGameState(waiting);
                                angel.setPosition(250, 300);
                                obstaclePassed = 0;
                                fires.clear();
                            break;
                            case 2:
                                //back to the main menu
                                audio.playSoundButton();
                                game.setGameState(menuStart);
                                difficulties.resetSelectedItem();
                                breakMenu.resetSelectedItem();
                                gameOverMenu.setSelectedItemId(1);
                            break;
                        }
                }
            }
            else if(e.type == Event::MouseButtonPressed){
                if(Mouse::isButtonPressed(Mouse::Left)){
                    Vector2f mouse_position = window.mapPixelToCoords(Mouse::getPosition(window));
                    if(mouse_position.x >=MOUSE_POSITION_X_LEFT && mouse_position.x <=MOUSE_POSITION_X_RIGHT  ){
                        if(mouse_position.y >= MOUSE_POSITION_Y_ITEM2_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM2_RIGHT){
                            //retry -> gamestate in waiting
                            audio.playSoundButton();
                            audio.playMusicGame();
                            game.setGameState(waiting);
                            angel.setPosition(250, 300);
                            obstaclePassed = 0;
                            fires.clear();
                        }
                        else if(mouse_position.y >= MOUSE_POSITION_Y_ITEM3_LEFT && mouse_position.y<=MOUSE_POSITION_Y_ITEM3_RIGHT){
                            //back to the main menu
                            audio.playSoundButton();
                            game.setGameState(menuStart);
                            difficulties.resetSelectedItem();
                            breakMenu.resetSelectedItem();
                            gameOverMenu.setSelectedItemId(1);
                        }
                    }
                }
            }
        }

        }
		// clear, draw, display all elements in the game
		window.clear();
        if(game.getGameState() == menuStart){
            menu.draw(window);
        }
        else if(game.getGameState() == menuDifficulties){
            difficulties.draw(window);
        }
        else if(game.getGameState() == menuSettings){
            settings.draw(window);
        }
        else if(game.getGameState()  == onBreak){
            breakMenu.draw(window);
        }
        else if(game.getGameState()  == victory){
            victoryMenu.draw(window);
        }
        else if(game.getGameState()  == gameover){
            gameOverMenu.draw(window);
        }
		else if (game.getGameState() == waiting || game.getGameState() == started) {
            game.drawBackground(window);
            angel.draw(window);

            // draw fires obstacles
            for (vector<Sprite>::iterator itr = fires.begin(); itr != fires.end(); itr++) {
                window.draw(*itr);
            }
		}
		//only 2 menu items for the victory menu and gameover menu
		if(gameOverMenu.getSelectedItemId() == 0){
            gameOverMenu.setSelectedItemId(1);
        }
        if(victoryMenu.getSelectedItemId() == 0){
            victoryMenu.setSelectedItemId(1);
        }
        window.display();

        // dont forget to update total frames
		game.nextFrame();
	}
    return 0;
}
