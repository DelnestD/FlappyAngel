#ifndef AUDIO_H
#define AUDIO_H

#include <iostream>
#include <SFML/Audio.hpp>
#include "Definitions.h"

using namespace std;
using namespace sf;
// The class Audio allows to manipulate the volume of the sound effects and the music.
// It also allows to play the different sounds.
class Audio
{
    public:
        static Audio& Instance();
        //menu methods
        void playMusicMenu();
        void playMusicGame();
        void playSoundButton();
        void playSoundSwitchButton();
        void playSoundVictory();
        //angel methods
        void playSoundHit();
        void playSoundWing();
        void playSoundPass();
        void setVolumeMusic(float volume);
        void setVolumeSound(float volume);
    protected:

    private:
        Audio();
        virtual ~Audio();
        Audio(const Audio& other);
        //one instance of the class Audio
        static Audio m_instance;
        //menu sounds
        SoundBuffer musicmenuSoundbuffer;
        Sound musicMenu;
        SoundBuffer victorySoundbuffer;
        Sound victorySound;
        SoundBuffer ingamemusicSoundbuffer;
        Sound gameMusic;
        SoundBuffer buttonSoundbuffer;
        SoundBuffer switchbuttonSoundbuffer;
        Sound buttonSound;
        Sound switchButtonSound;
        //angel sounds
        SoundBuffer hitSoundbuffer;
        SoundBuffer wingSoundbuffer;
        SoundBuffer passSoundbuffer;
        Sound hitSound;
        Sound wingSound;
        Sound passSound;
};

#endif // AUDIO_H
