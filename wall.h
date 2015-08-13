//
//  wall.h
//  SDL_Tutorial
//
//  Created by Alexander Ma on 8/2/15.
//  Copyright (c) 2015 Alexander Ma. All rights reserved.
//

#ifndef __SDL_Tutorial__wall__
#define __SDL_Tutorial__wall__

class Wall
{
public:
    
    //Default constructor
    Wall();
    
    //Basic wall
    Wall(LTexture* texture, int x, int y, int w, int h);
    
    //Extra features
    Wall(LTexture* texture, int x, int y, int w, int h, int movement, int speed, int turns);
    
    //Moves the dot and checks collision
    void move();
    
    //Shows the dot on the screen relative to the camera
    void render( int camX, int camY );
    
    //Accessor methods
    int getX();
    int getY();
    int getW();
    int getH();
    int getVel();
    bool getDir();
    
private:
    
    //The wall rectangle
    SDL_Rect myWall;
    
    //Wall coordinates, width and height
    int wallX, wallY, wallW, wallH;
    
    //Variables to keep track of wall movement
    int start, end, moveType, vel;
    
    //Whether the wall is on its way back or not
    bool wayBack;
    
    //Pointer to dot texture
    LTexture* gWallTexture;
};
#endif /* defined(__SDL_Tutorial__wall__) */
