#include "Audio.h"

Audio Audio::m_instance=Audio();

Audio::Audio()
{
    //ctor
    //loading all the sounds file in their buffer
    if(!musicmenuSoundbuffer.loadFromFile(MENU_MUSIC)){
        cout<<"Error loading music menu sound effect"<<endl;
    }
    if(!victorySoundbuffer.loadFromFile(VICTORY_SOUND)){
        cout<<"Error loading victory sound effect"<<endl;
    }
    if(!ingamemusicSoundbuffer.loadFromFile(GAME_MUSIC)){
        cout<<"Error loading ingame music sound effect"<<endl;
    }
    if(!buttonSoundbuffer.loadFromFile(BUTTON_SOUND)){
        cout<<"Error loading button sound effect"<<endl;
    }
    if(!switchbuttonSoundbuffer.loadFromFile(SWITCHBUTTON_SOUND)){
        cout<<"Error loading switch button sound effect"<<endl;
    }
    if(!hitSoundbuffer.loadFromFile(HIT_SOUND)){
        cout<<"Error loading angel hit sound effect"<<endl;
    }
    if(!wingSoundbuffer.loadFromFile(WING_SOUND)){
        cout<<"Error loading angel wings sound effect"<<endl;
    }
    if(!passSoundbuffer.loadFromFile(PASS_SOUND)){
        cout<<"Error loading angel pass sound effect"<<endl;
    }
    //set the buffer for the different sound of the game
    buttonSound.setBuffer(buttonSoundbuffer);
    switchButtonSound.setBuffer(switchbuttonSoundbuffer);
    musicMenu.setBuffer(musicmenuSoundbuffer);
    victorySound.setBuffer(victorySoundbuffer);
    gameMusic.setBuffer(ingamemusicSoundbuffer);

    hitSound.setBuffer(hitSoundbuffer);
    wingSound.setBuffer(wingSoundbuffer);
    passSound.setBuffer(passSoundbuffer);

    //set the music sounds in looping mode
    musicMenu.setLoop(true);
    gameMusic.setLoop(true);
}

Audio::~Audio()
{
    //dtor
}

Audio::Audio(const Audio& other)
{
    buttonSound=other.buttonSound;
    switchButtonSound=other.switchButtonSound;
    musicMenu=other.musicMenu;
    victorySound=other.victorySound;
    gameMusic=other.gameMusic;
    hitSound=other.hitSound;
    wingSound=other.wingSound;
    passSound=other.passSound;
}
Audio& Audio::Instance()
{
    //return the only instance of the audio class (design pattern singleton)
    return m_instance;
}

void Audio::playMusicMenu()
{
    //stopping the ingame music and playing the music of the main menu
    gameMusic.stop();
    musicMenu.play();
}
void Audio::playMusicGame()
{
    //stopping the music of the main menu and playing the ingame music
    musicMenu.stop();
    gameMusic.play();
}
//playing the different sound
void Audio::playSoundButton()
{
    buttonSound.play();
}
void Audio::playSoundSwitchButton()
{
    switchButtonSound.play();
}
void Audio::playSoundVictory()
{
    //stopping the ingame music and playing the victory sound
    gameMusic.stop();
    victorySound.play();
}
void Audio::playSoundHit()
{
    hitSound.play();
}
void Audio::playSoundWing()
{
    wingSound.play();
}
void Audio::playSoundPass()
{
    passSound.play();
}
void Audio::setVolumeMusic(float volume)
{
    //set the volume of the ingame music and the music of the main menu
    musicMenu.setVolume(volume);
    gameMusic.setVolume(volume);
}
void Audio::setVolumeSound(float volume)
{
    //set all the different sound effects
    buttonSound.setVolume(volume);
    switchButtonSound.setVolume(volume);
    victorySound.setVolume(volume);
    hitSound.setVolume(volume);
    wingSound.setVolume(volume);
    passSound.setVolume(volume);
}
