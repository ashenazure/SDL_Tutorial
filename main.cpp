/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
 and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "ltexture.h"
#include "dot.h"

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Set the wall
SDL_Rect wall[10];
int numWalls = 0;
int inAir = 0;
bool won = false;

//Texture wrapper class

//The dot that will move around on the screen

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Box collision detector
//bool checkCollision( SDL_Rect a, SDL_Rect b );

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;
LTexture gBGTexture; //bg

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

bool init()
{
    //Initialization flag
    bool success = true;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }
        
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                
                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }
            }
        }
    }
    
    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;
    
    //Load press texture
    //if( !gDotTexture.loadFromFile( "27_collision_detection/dot.bmp" ) )
    if( !gDotTexture.loadFromFile( "dot.bmp", gRenderer ) )
    {
        printf( "Failed to load dot texture!\n" );
        success = false;
    }
    
    //Load background texture bg
    if( !gBGTexture.loadFromFile( "Cat.png", gRenderer ) )
    {
        printf( "Failed to load background texture!\n" );
        success = false;
    }
    
    return success;
}

void close()
{
    if ( won ) {
        //Deallocate surface
        SDL_FreeSurface( gHelloWorld );
        gHelloWorld = NULL;
    }
    //Free loaded images
    gDotTexture.free();
    gBGTexture.free(); // bg
    
    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;
    
    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}


void setWalls( int x, int y, int w, int h )
{
    wall[numWalls].x = x;
    wall[numWalls].y = y;
    wall[numWalls].w = w;
    wall[numWalls].h = h;
    numWalls += 1;
}

int main( int argc, char* args[] )
{
    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Main loop flag
            bool quit = false;
            bool positive = true;
            int a;
            
            //Event handler
            SDL_Event e;
            
            //The dot that will be moving around on the screen
            Dot dot;
            
            //The camera area
            SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
            SDL_Rect bgcam = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
            
            //Set the wall
            setWalls(40, 300, 400, 40);
            setWalls(80, 100, 400, 40);
            setWalls(300, 140, 40, 160);
            setWalls(0, 460, 600, 20);
            setWalls(300, 430, 30, 30);
            setWalls(800, 450, 400, 50);
            setWalls(100, 800, 400, 50);
            setWalls(0, 940, 600, 20);
            setWalls(600, 650, 100, 100);
            setWalls(20, 0, 1280, 20);
            
            //While application is running
            while( !quit )
            {
                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    
                    //Handle input for the dot
                    dot.handleEvent( e );
                }
                
                //Move the dot and check collision
                a = dot.move( wall );
                if ( wall[5].x <= 200 ) {
                    positive = true;
                }
                else if ( wall[5].x >= 1000 ) {
                    positive = false;
                }
                if ( positive ) {
                    wall[5].x += 1;
                }
                else {
                    wall[5].x -= 1;
                }
                
                //Center the camera over the dot
                camera.x = ( dot.getPosX() + Dot::DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
                camera.y = ( dot.getPosY() + Dot::DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;
                
                //Keep the camera in bounds
                if( camera.x < 0 )
                {
                    camera.x = 0;
                }
                if( camera.y < 0 )
                {
                    camera.y = 0;
                }
                if( camera.x > LEVEL_WIDTH - camera.w )
                {
                    camera.x = LEVEL_WIDTH - camera.w;
                }
                if( camera.y > LEVEL_HEIGHT - camera.h )
                {
                    camera.y = LEVEL_HEIGHT - camera.h;
                }
                
                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );
                
                //Render background bg
                gBGTexture.render( 0, 0, &bgcam, gRenderer );
                
                //Render wall
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
                for (int i = 0; i < numWalls; i++) {
                    wall[i].x -= camera.x;
                    wall[i].y -= camera.y;
                    SDL_RenderDrawRect( gRenderer, &wall[i] );
                    SDL_RenderFillRect( gRenderer, &wall[i] );
                    wall[i].x += camera.x;
                    wall[i].y += camera.y;
                }
                
                //Render dot
                dot.render( camera.x, camera.y, gDotTexture );
                
                //Update screen
                SDL_RenderPresent( gRenderer );
                if ( a == 8 ) {
                    quit = true;
                    won = true;
                }
            }
        }
    }
    
    if ( won ) {
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        gScreenSurface = SDL_GetWindowSurface( gWindow );
        gHelloWorld = SDL_LoadBMP( "hello_world.bmp" );
        
        //Apply the image
        SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
        
        //Update the surface
        SDL_UpdateWindowSurface( gWindow );
        
        //Wait two seconds
        SDL_Delay( 2000 );

        
    }
    
    //Free resources and close SDL
    close();
    
    return 0;
}
