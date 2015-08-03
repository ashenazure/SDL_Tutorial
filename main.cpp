/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
 and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#ifdef __linux__
#include <SDL2/SDL_image.h>
#else
#include <SDL2_image/SDL_image.h>
#endif
#include <stdio.h>
#include <string>
#include "ltexture.h"
#include "wall.h"
#include "dot.h"

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//Set the wall
Wall wall[10];
int numWalls = 0;
bool win = false;
bool lose = false;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Scene textures
LTexture gDotTexture;
LTexture gBGTexture;
LTexture gWallTexture;

//powerup animation
const int POWERUP_ANIMATION_FRAMES = 4;
SDL_Rect gPowerupClips[ POWERUP_ANIMATION_FRAMES ];
LTexture gPowerupTexture;

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
				
				//reset the renderer for all textures
				gDotTexture.setRenderer(gRenderer);
				gBGTexture.setRenderer(gRenderer);
				gPowerupTexture.setRenderer(gRenderer);
                gWallTexture.setRenderer(gRenderer);
                
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

	//Load powerup texture
	if( !gPowerupTexture.loadFromFile( "powerup.png", gRenderer ) )
	{
		printf( "Failed to load powerup animation texture!\n" );
		success = false;
	}
	else
	{
		//set sprite clips
		gPowerupClips[0].x = 0;
		gPowerupClips[0].y = 0;
		gPowerupClips[0].w = 20;
		gPowerupClips[0].h = 20;

		gPowerupClips[1].x = 20;
		gPowerupClips[1].y = 0;
		gPowerupClips[1].w = 20;
		gPowerupClips[1].h = 20;

		gPowerupClips[2].x = 40;
		gPowerupClips[2].y = 0;
		gPowerupClips[2].w = 20;
		gPowerupClips[2].h = 20;

		gPowerupClips[3].x = 60;
		gPowerupClips[3].y = 0;
		gPowerupClips[3].w = 20;
		gPowerupClips[3].h = 20;
	}
    
    return success;
}

void close()
{
    if ( win ) {
        //Deallocate surface
        SDL_FreeSurface( gHelloWorld );
        gHelloWorld = NULL;
    }
    if ( lose ) {
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

SDL_Surface* loadSurface( std::string path )
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;
    
    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, gScreenSurface->format, NULL );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        
        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }
    
    return optimizedSurface;
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
            int a;

			//animation frame
			int frame = 0;
            
            //Event handler
            SDL_Event e;
            
            //The dot that will be moving around on the screen
            Dot dot( &gDotTexture );
            
            //The camera area
            SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
            SDL_Rect bgcam = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
            
            //Set the wall
            wall[0] = Wall(&gWallTexture, 40, 300, 400, 40);
            wall[1] = Wall(&gWallTexture, 80, 100, 400, 40);
            wall[2] = Wall(&gWallTexture, 300, 140, 40, 160);
            wall[3] = Wall(&gWallTexture, 0, 460, 600, 20);
            wall[4] = Wall(&gWallTexture, 300, 430, 30, 30);
            wall[5] = Wall(&gWallTexture, 800, 450, 400, 50);
            wall[6] = Wall(&gWallTexture, 100, 800, 400, 50);
            wall[7] = Wall(&gWallTexture, 0, 940, 600, 20);
            wall[8] = Wall(&gWallTexture, 600, 650, 100, 100);
            numWalls = 9;
            //setWalls(20, 0, 1280, 20);
            
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
                gBGTexture.render( 0, 0, &bgcam );
                
                //Render wall
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
                for (int i = 0; i < numWalls; i++) {
                    wall[i].render( camera.x, camera.y );
                }
                
                //Render dot
                dot.render( camera.x, camera.y );
				
                //Render current frame
                SDL_Rect* currentClip = &gPowerupClips[ frame / 4 ];
                gPowerupTexture.render( 50, 50, currentClip );
				
				//Go to next frame
				++frame;

				//cycle animation
				if( frame / 4 >= POWERUP_ANIMATION_FRAMES )
				{
					frame = 0;
				}

                //Update screen
                SDL_RenderPresent( gRenderer );
                if ( a == 8 ) {
                    quit = true;
                    win = true;
                }
            }
        }
    }
    
    if ( win ) {
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        gScreenSurface = SDL_GetWindowSurface( gWindow );
        gHelloWorld = loadSurface( "Win.png" );
        
        //Apply the image
        SDL_BlitSurface( gHelloWorld, NULL, gScreenSurface, NULL );
        
        //Update the surface
        SDL_UpdateWindowSurface( gWindow );
        
        //Wait two seconds
        SDL_Delay( 2000 );
    }
    
    if ( lose ) {
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        gScreenSurface = SDL_GetWindowSurface( gWindow );
        gHelloWorld = loadSurface( "Lose.png" );
        
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
