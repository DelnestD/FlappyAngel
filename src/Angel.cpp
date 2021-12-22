#include "Angel.h"

Angel::Angel()
{
    //ctor
    //load each background images in the vector of textures
    //3 frames to make believe that the angel is flying
    texture[0].loadFromFile(ANGEL_FRAME1);
	texture[1].loadFromFile(ANGEL_FRAME2);
	texture[2].loadFromFile(ANGEL_FRAME3);
    //set the first texture on the sprite
	sprite.setTexture(texture[0]);
	//set the starting position of the angel in the game
    sprite.setPosition(250,300);
    //set the scale of the angel
    sprite.setScale(2,2);
}

Angel::~Angel()
{
    //dtor
}

Angel::Angel(const Angel& other)
{
    //copy ctor
}
void Angel::setPosition(float x,float y)
{
    //set the position of the angel in the game
    sprite.setPosition(x, y);
}
float Angel::getPositionX()
{
    //get the horizontal position of the angel in the game
    return sprite.getPosition().x;
}
float Angel::getPositionY()
{
    //get the vertical position of the angel in the game
    return sprite.getPosition().y;
}
float Angel::getScaleX()
{
    //get the horizontal scale of the angel
    return sprite.getScale().x;
}
float Angel::getScaleY()
{
      //get the vertical scale of the angel
    return sprite.getScale().y;
}
void Angel::nextFrame()
{
    //+one frame while the window is open
    frame += 1;
}
void Angel::resetFrame()
{
    //reset the number of frame of the angel
    frame = 0;
}
int Angel::getFrame()
{
    //get the number of frame of the angel
    return frame;
}
void Angel::move()
{
    //to make look like the angel is descending
    //each frame the angel vertical position will increase of 0.5
    sprite.move(0,v);
    v += GRAVITY_ANGEL;//gravity 0.5
}
void Angel::setV(int i)
{
    //set the vertical position
    v = i;
}
void Angel::setTexture(int i)
{
    //set the wanted texture on the sprite
    //by giving the position of the wanted texture in the vector
    sprite.setTexture(texture[i]);
}
void Angel::draw(RenderWindow &app)
{
    //draw the sprites
    app.draw(sprite);
}
