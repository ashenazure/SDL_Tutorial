//
//  wall.cpp
//  SDL_Tutorial
//
//  Created by Alexander Ma on 8/2/15.
//  Copyright (c) 2015 Alexander Ma. All rights reserved.
//

#include <SDL2/SDL.h>
#ifdef __linux__
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif
#include <stdio.h>
#include <string>
#include "ltexture.h"
#include "wall.h"

Wall::Wall()
{
    myWall.x = 0;
    myWall.y = 0;
    myWall.w = 0;
    myWall.h = 0;
    moveType = 0;
    vel = 0;
    start = 0;
    end = 0;
    wayBack = false;
    gWallTexture = NULL;
}

Wall::Wall(LTexture* texture, int x, int y, int w, int h)
{
    myWall.x = x;
    myWall.y = y;
    myWall.w = w;
    myWall.h = h;
    moveType = 0;
    vel = 0;
    start = 0;
    end = 0;
    wayBack = false;
    gWallTexture = texture;
}

Wall::Wall(LTexture* texture, int x, int y, int w, int h, int movement, int speed, int turns)
{
    myWall.x = x;
    myWall.y = y;
    myWall.w = w;
    myWall.h = h;
    moveType = movement; // 0 for horizontal, 1 for vertical
    vel = speed;
    end = turns;
    wayBack = false;
    gWallTexture = NULL;
    if (moveType == 0) {
        start = myWall.x;
    }
    else {
        start = myWall.y;
    }
    if (start - end > 0) { // so start always less than end
        end = start;
        start = turns;
    }
    gWallTexture = texture;
}

void Wall::move()
{
    if (vel != 0) {
        if (moveType == 0) {
            if (!wayBack) {
                if (myWall.x + vel >= end) {
                    myWall.x = end;
                    wayBack = true;
                }
                else {
                    myWall.x += vel;
                }
            }
            else {
                if (myWall.x - vel <= start) {
                    myWall.x = start;
                    wayBack = false;
                }
                else {
                    myWall.x -= vel;
                }
            }
        }
        else {
            if (!wayBack) {
                if (myWall.y + vel >= end) {
                    myWall.y = end;
                    wayBack = true;
                }
                else {
                    myWall.y += vel;
                }
            }
            else {
                if (myWall.y - vel <= start) {
                    myWall.y = start;
                    wayBack = false;
                }
                else {
                    myWall.y -= vel;
                }
            }
        }
    }
}

void Wall::render( int camX, int camY )
{
    myWall.x -= camX;
    myWall.y -= camY;
    SDL_RenderDrawRect( gWallTexture->getRenderer(), &myWall );
    SDL_RenderFillRect( gWallTexture->getRenderer(), &myWall );
    myWall.x += camX;
    myWall.y += camY;
}

int Wall::getX() {
    return myWall.x;
}

int Wall::getY() {
    return myWall.y;
}

int Wall::getW() {
    return myWall.w;
}

int Wall::getH() {
    return myWall.h;
}

int Wall::getVel() {
    return vel;
}

bool Wall::getDir() {
    return wayBack;
}