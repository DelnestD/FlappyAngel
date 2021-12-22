#ifndef ANGEL_H
#define ANGEL_H

#include<SFML/Graphics.hpp>
#include "Definitions.h"

using namespace sf;
using namespace std;
// The class Angel allows to manipulate the position of the angel.
//It also gives the number of frame of the angel (frame of the Angel is different from the frame of the game)
class Angel
{
    public:
        Angel();
        virtual ~Angel();
        Angel(const Angel& other);
        void setPosition(float x,float y);
        float getPositionX();
        float getPositionY();
        float getScaleX();
        float getScaleY();
        void nextFrame();
        void resetFrame();
        int getFrame();
        void move();
        void setV(int i);
        void draw(RenderWindow &app);
        void setTexture(int i);

    protected:

    private:
        double v = 0;
        int frame = 0;
        Texture texture[3];//3 background images
        Sprite sprite;
};

#endif // ANGEL_H
