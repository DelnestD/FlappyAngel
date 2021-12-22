#include "Menu.h"
Menu::Menu(float width,float height,string menu1Label , string menu2Label ,string menu3Label, MenuType mt)
{
    //setting the type of the menu
    menuType = mt;
    //loading font
    if(!font.loadFromFile(PACIFICO_FONT)){
       cout<<"Error loading font"<<endl;
    }
    //sett the font, color, text of the different menu items
    text[0].setFont(font);
    text[0].setColor(Color::Blue);
    text[0].setString(menu1Label);
    text[0].setPosition(Vector2f(width / 1.5,height /(3 + 1)*1));

    text[1].setFont(font);
    text[1].setColor(Color::White);
    text[1].setString(menu2Label);
    text[1].setPosition(Vector2f(width / 1.5,height /(3 + 1)*2));

    text[2].setFont(font);
    text[2].setColor(Color::White);
    text[2].setString(menu3Label);
    text[2].setPosition(Vector2f(width / 1.5,height /(3 + 1)*3));
    resetSelectedItem();
}

Menu::~Menu()
{

}

void Menu::draw(RenderWindow &app){
    Texture t;
    //load the background image depending on the menu type
    if(menuType == normalMenuType){
        t.loadFromFile(MENU_FRAME);
    }else if(menuType == startMenuType){
        t.loadFromFile(START_FRAME);
    }else if(menuType == breakMenuType){
        t.loadFromFile(BREAK_FRAME);
    }else if(menuType == victoryMenuType){
        t.loadFromFile(VICTORY_FRAME);
    }else if(menuType == gameOverMenuType){
        t.loadFromFile(GAMEOVER_FRAME);
    }
    //set the texture on the sprite
    Sprite s(t);
    //draw the sprite and the different menu items
    app.draw(s);
    for(int i=0;i<3;i++){
        app.draw(text[i]);
    }
}

void Menu::moveUp(){
    //move up between the different menu item
    //can't go under 0 in the list (only 3 menu items)
    if(selectedItemId -1 >=0){
        text[selectedItemId].setColor(Color::White);
        selectedItemId--;
        //set the color blue for the selected menu item
        text[selectedItemId].setColor(Color::Blue);
    }
}

void Menu::moveDown(){
    //move down between the different menu item
    //can't go futher 2 in the list (only 3 menu items)
    if(selectedItemId + 1 < 3){
        text[selectedItemId].setColor(Color::White);
        selectedItemId++;
        text[selectedItemId].setColor(Color::Blue);
    }
}

int Menu::getSelectedItemId()const{
    return selectedItemId;
}

void Menu::resetSelectedItem(){
    //set the menu item to the first one
    setSelectedItemId(0);
}

void Menu::setSelectedItemId(int id){
    //set the selected menu item
    if(id >=0 && id<=2){
        this->selectedItemId = id;

        switch(selectedItemId){
        case 0:
            text[0].setColor(Color::Blue);
            text[1].setColor(Color::White);
            text[2].setColor(Color::White);
            break;
        case 1 :
            text[0].setColor(Color::White);
            text[1].setColor(Color::Blue);
            text[2].setColor(Color::White);
            break;
        case 2 :
            text[0].setColor(Color::White);
            text[1].setColor(Color::White);
            text[2].setColor(Color::Blue);
            break;
        }
    }
}
void Menu::moveRight(){
    //rise the volume by 10 of the music
    if(selectedItemId == 0)
    {
        volumeMusic +=VOLUME_STEP;
        //can't go upper 100
        if(volumeMusic > VOLUME_CANT_GO_UPPER){
            volumeMusic = VOLUME_CANT_GO_UPPER;
        }
        text[selectedItemId].setString("Music : "+to_string((int)volumeMusic)+"%");
    }
    //rise the volume by 10 of the sound effects
    else if(selectedItemId == 1){
        volumeSound +=VOLUME_STEP;
        //can't go over 100
        if(volumeSound > VOLUME_CANT_GO_UPPER){
            volumeSound = VOLUME_CANT_GO_UPPER;
        }
        text[selectedItemId].setString("Sound effects : "+to_string((int)volumeSound)+"%");
    }

}

void Menu::moveLeft(){
     //lower the volume by 10 of the music
    if(selectedItemId == 0)
    {
        volumeMusic -=VOLUME_STEP;
        //can't go under 0
        if(volumeMusic < VOLUME_CANT_GO_UNDER){
            volumeMusic = VOLUME_CANT_GO_UNDER;
        }
        text[selectedItemId].setString("Music : "+to_string((int)volumeMusic)+"%");
    }
    //lower the volume by 10 of the sound effects
    else if(selectedItemId == 1){
        volumeSound -=VOLUME_STEP;
        //can't go under 0
        if(volumeSound < VOLUME_CANT_GO_UNDER){
            volumeSound = VOLUME_CANT_GO_UNDER;
        }
        text[selectedItemId].setString("Sound effects : "+to_string((int)volumeSound)+"%");
    }

}
void Menu::setVolumeMusic(float volume)
{
    //set the volume of the music
    volumeMusic = volume;
    text[selectedItemId].setString("Music : "+to_string((int)volumeMusic)+"%");
}
void Menu::setVolumeSound(float volume)
{
    //set the volume of the sound effects
    volumeSound = volume;
    text[selectedItemId].setString("Sound effects : "+to_string((int)volumeSound)+"%");
}
float Menu::getVolumeMusic()
{
    return volumeMusic;
}
float Menu::getVolumeSound()
{
    return volumeSound;
}

void Menu::cutMusic(){
    //cut the music (put to 0) and save the last music volume to go back to that volume if the menu item is clicked/pressed again
    if(volumeMusic == 0){
        volumeMusic = savelastvolumemusic;
        setVolumeMusic(volumeMusic);

    }
    else{
        savelastvolumemusic = volumeMusic;
        volumeMusic = 0;
        setVolumeMusic(volumeMusic);
    }

}
void Menu::cutSoundEffect(){
    //cut the sound effect (put to 0) and save the last sound effect volume to go back to that volume if the menu item is clicked/pressed again
    if(volumeSound == 0){
        volumeSound = savelastvolumesoundeffect;
        setVolumeSound(volumeSound);
    }
    else{
        savelastvolumesoundeffect = volumeSound;
        volumeSound = 0;
        setVolumeSound(volumeSound);
    }
}
