#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>
#include "ltexture.h"
#include "dot.h"

Dot::Dot()
{
    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;
    
    //Set collision box dimension
    mCollider.w = DOT_WIDTH;
    mCollider.h = DOT_HEIGHT;
    
    //Initialize the velocity
    mVelX = 0;
    mVelY = 0.5;
}

void Dot::handleEvent( SDL_Event& e )
{
    //If a key was pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
                //case SDLK_UP: mVelY -= DOT_VEL; break;
            case SDLK_UP:
                if ( inAir == 0 ) {
                    mVelY = -10;
                    inAir = 1;
                }
                else if ( inAir == 1 ) {
                    mVelY = -10;
                    inAir = 2;
                }
                break;
        ////case SDLK_DOWN: 
		////	if ( inAir > 0 ) {
		////		mVelY += 5;
		////	}
		////	break;
            case SDLK_LEFT: mVelX -= DOT_VEL; break;
            case SDLK_RIGHT: mVelX += DOT_VEL; break;
        }
    }
    //If a key was released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        //Adjust the velocity
        switch( e.key.keysym.sym )
        {
                //case SDLK_UP: mVelY += DOT_VEL; break;
            	//case SDLK_DOWN: mVelY -= DOT_VEL; break;
            case SDLK_LEFT: mVelX += DOT_VEL; break;
            case SDLK_RIGHT: mVelX -= DOT_VEL; break;
        }
    }
}

int Dot::move( SDL_Rect wall[] )
{
    //Move the dot left or right
    mPosX += mVelX;
    mCollider.x = mPosX;
    bool collidedX = false;
    bool collidedY = false;
    int wallCollidedWith = -1;
    for (int i = 0; i < 10/*TODO*/; i++) {
        if (checkCollision( mCollider, wall[i])) {
            collidedX = true;
            wallCollidedWith = i;
        }
    }
    
    //If the dot collided or went too far to the left or right
    if( ( mPosX < 0 ) || ( mPosX + DOT_WIDTH > 1280/*TODO*/ ) || collidedX )
    {
        //Move back
        mPosX -= mVelX;
        mCollider.x = mPosX;
    }
    
    //Move the dot up or down
    mPosY += mVelY;
    mCollider.y = mPosY;
    for (int i = 0; i < 10/*TODO*/; i++) {
        if (checkCollision( mCollider, wall[i])) {
            collidedY = true;
            wallCollidedWith = i;
        }
    }
    
    //If the dot collided or went too far up or down
    if( ( mPosY < 0 ) )
    {
        //Move back
        mPosY = 0;
        mCollider.y = mPosY;
        mVelY = 0.5;
    }
    else if ( collidedY ) {
        if ( mVelY < 0) { // upward collision
            mPosY = wall[wallCollidedWith].y + wall[wallCollidedWith].h;
        }
        else if ( mVelY > 0 ) { // downward collision
            mPosY = wall[wallCollidedWith].y - DOT_HEIGHT;
            inAir = 0;
        }
        mCollider.y = mPosY;
        mVelY = 0.5;
    }
    else if ( ( mPosY + DOT_HEIGHT > 960/*TODO*/ ) ) {
        mPosY = 960/*TODO*/ - DOT_HEIGHT;
        mCollider.y = mPosY;
        mVelY = 0.5;
        inAir = 0;
    }
    else {
        mVelY += 0.5;
    }
    return wallCollidedWith;
}

void Dot::render( int camX, int camY, LTexture* gDotTexture, SDL_Renderer* gRenderer )
{
    //Show the dot relative to the camera
    SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
    gDotTexture->render( mPosX - camX, mPosY - camY, NULL, gRenderer );
}

int Dot::getPosX()
{
    return mPosX;
}

int Dot::getPosY()
{
    return mPosY;
}

bool Dot::checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;
    
    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;
    
    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;
    
    //If any of the sides from A are outside of B
    if( bottomA <= topB )
    {
        return false;
    }
    
    if( topA >= bottomB )
    {
        return false;
    }
    
    if( rightA <= leftB )
    {
        return false;
    }
    
    if( leftA >= rightB )
    {
        return false;
    }
    
    //If none of the sides from A are outside B
    return true;
}
