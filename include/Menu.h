#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

#include "Definitions.h"
#include "MenuType.h"

using namespace std;
using namespace sf;
// The class Menu allows to move between the menu items of the menu.
// It also allows to reduce or increase the volume sound for the settings menu.
class Menu
{
    public:
        Menu(float width,float height,string menu1Label, string menu2Label ,string menu3Label, MenuType mt);
        virtual ~Menu();
        void draw(RenderWindow &app);
        void moveUp();//to move up and down between menu item
        void moveDown();
        int getSelectedItemId()const;//get the selected menu item
        void setSelectedItemId(int id);
        void resetSelectedItem();
        void moveRight();//rise the sound volume
        void moveLeft();//lower the sound volume
        void setVolumeMusic(float volume);
        void setVolumeSound(float volume);
        float getVolumeMusic();
        float getVolumeSound();
        void cutSoundEffect();
        void cutMusic();

    private:
        int selectedItemId;
        Font font;
        Text text[3];
        float volumeMusic = 100;
        float volumeSound = 100;
        MenuType menuType;//enum in MenuType.h
        float savelastvolumemusic;
        float savelastvolumesoundeffect;
};

#endif // MENU_H
