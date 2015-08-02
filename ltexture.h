#ifndef LTEXTURE_H
#define LTEXTURE_H

class LTexture
{
public:
    //Initializes variables
    LTexture();
    
    LTexture( SDL_Renderer* gRenderer );
    
    //Deallocates memory
    ~LTexture();
    
    //Loads image at specified path
    bool loadFromFile( std::string path, SDL_Renderer* gRenderer );
    
#ifdef _SDL_TTF_H
    //Creates image from font string
    bool loadFromRenderedText( std::string textureText, SDL_Color textColor, SDL_Renderer* gRenderer );
#endif
    
    //Deallocates texture
    void free();
    
    //Set color modulation
    void setColor( Uint8 red, Uint8 green, Uint8 blue );
    
    //Set blending
    void setBlendMode( SDL_BlendMode blending );
    
    //Set alpha modulation
    void setAlpha( Uint8 alpha );
    
    //Renders texture at given point
    void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
    
    //Gets image dimensions
    int getWidth();
    int getHeight();

	//Gets the renderer
	SDL_Renderer* getRenderer();

	//Set the renderer
	void setRenderer(SDL_Renderer* renderer);
    
private:
    //The actual hardware texture
    SDL_Texture* mTexture;

	//Renderer used for texture
	SDL_Renderer* gRenderer;
    
    //Image dimensions
    int mWidth;
    int mHeight;
};

#endif //LTEXTURE_H
