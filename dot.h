#ifndef DOT_H
#define DOT_H

class Dot
{
public:
    //The dimensions of the dot
    static const int DOT_WIDTH = 20;
    static const int DOT_HEIGHT = 20;
    
    //Maximum axis velocity of the dot
    static const int DOT_VEL = 5;
    
    //Initializes the variables
    Dot();
    
    //Takes key presses and adjusts the dot's velocity
    void handleEvent( SDL_Event& e );
    
    //Moves the dot and checks collision
    int move( SDL_Rect wall[] );
    
    //Shows the dot on the screen relative to the camera
    void render( int camX, int camY, LTexture gDotTexture );
    
    //Position accessors
    int getPosX();
    int getPosY();

	//checkCollision
	bool checkCollision( SDL_Rect a, SDL_Rect b );
    
private:
    //The X and Y offsets of the dot
    int mPosX, mPosY;
    
    //The velocity of the dot
    float mVelX, mVelY;
    
    //Dot's collision box
    SDL_Rect mCollider;

	//inair
	int inAir = 0;
};

#endif //DOT_H
