#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include <string>
#include <cmath>

//Screen attributes
const int SCREEN_WIDTH = 825;
const int SCREEN_HEIGHT = 550;
const int SCREEN_BPP = 32;

SDL_Event event;

//The surfaces
SDL_Surface *mainScreen = NULL;
SDL_Surface *outlinePlay = NULL;
SDL_Surface *outlineQuit = NULL;
SDL_Surface *outlineCredits = NULL;
SDL_Surface *play = NULL;
SDL_Surface *quit = NULL;
SDL_Surface *credits = NULL;
SDL_Surface *playingScreen = NULL;
SDL_Surface *creditsScreen = NULL;
SDL_Surface *goBack = NULL;
SDL_Surface *outlineGoBack = NULL;
SDL_Surface *arrow = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *blackScreen = NULL;
SDL_Surface *roto = NULL;

SDL_Surface *load_image( std::string filename );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
bool init();
int loadncheck();
void clean_up();

int main( int argc, char* args[] )
{
    bool stop = false, skip = false, bypass = false, creditScreenOn = false, playingScreenOn = false;
    bool continueRotateRight = false, continueRotateLeft = false, continueMove = false;
    double angle = 90, arrowx = 412, arrowy = 225;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    if (loadncheck() == 1)
        return 1;

    while (stop == false)
    {
        //Clears Screen
        if (bypass == false)
        {
            apply_surface( 0, 0, mainScreen, screen );
            apply_surface( 70, 300, play, screen );
            apply_surface( 70, 390, quit, screen );
            apply_surface( 70, 470, credits, screen );
        }

        while (creditScreenOn == true)
        {
            if (skip == false)
            {
                apply_surface(0, 0, creditsScreen, screen);
                apply_surface(550, 450, goBack, screen);

            }

            while( SDL_PollEvent( &event ) )
            {
                if( event.type == SDL_QUIT )
                {
                        //Quit the program
                        creditScreenOn = false;
                        stop = true;
                }
                if( event.type == SDL_MOUSEBUTTONDOWN )
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if (event.button.x > 550 && event.button.x < 705 && event.button.y > 450 && event.button.y < 530)
                        {
                            creditScreenOn = false;
                        }
                    }
                }
                if ( event.type == SDL_MOUSEMOTION)
                {
                    if (event.motion.x > 550 && event.motion.x < 705 && event.motion.y > 450 && event.motion.y < 530)
                    {
                        apply_surface( 550, 450, outlineGoBack, screen );
                        skip = true;
                    }
                    else
                    {
                        skip = false;
                    }
                }
            }

            if( SDL_Flip( screen ) == -1 )
            {
                return 1;
            }
        }

        while (playingScreenOn == true)
        {
            apply_surface(0, 0, blackScreen, screen);

            if(angle < 0)
                angle += 360;
            if(angle > 360)
                angle -= 360;

            //Scaled image to .75 times origional
            roto = rotozoomSurface(arrow, angle - 90, .75, 1);
            apply_surface(arrowx, arrowy, roto, screen);

            //Displays the image with rotation accounted for
            apply_surface(0, 0, playingScreen, screen);

            Uint8 *keystates = SDL_GetKeyState(NULL);

            while( SDL_PollEvent( &event ) )
            {
                if( event.type == SDL_QUIT )
                {
                        //Quit the program
                        playingScreenOn = false;
                        stop = true;
                }
                if( event.type == SDL_MOUSEBUTTONDOWN )
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        //For testing, any mouseclick on playing screen brings user back to main screen
                        if (event.button.x > 0 && event.button.x < 825 && event.button.y > 0 && event.button.y < 550)
                        {
                            playingScreenOn = false;
                        }
                    }
                }
                if (keystates[SDLK_LEFT] == true)
                    continueRotateLeft = true;
                else
                    continueRotateLeft = false;
                if (keystates[SDLK_RIGHT] == true)
                    continueRotateRight = true;
                else
                    continueRotateRight = false;
                if (keystates[SDLK_UP] == true)
                    continueMove = true;
                else
                    continueMove = false;
            }
            if (continueRotateRight == true)
            {
                //Rotate to the right
                angle -= 5.0;
            }
            if (continueRotateLeft == true)
            {
                //Rotate to the left
                angle += 5.0;
            }
            if (continueMove == true)
            {
                //Move arrow in direction facing
                arrowx += 2 * cos((angle) * .01745329251994); //Coverts degrees to radians
                arrowy -= 2 * sin((angle) * .01745329251994);
            }
            if (arrowx < 5)
            {
                arrowx = 5;
            }
            if (arrowx > 785)
            {
                arrowx = 785;
            }
            if (arrowy < 80)
            {
                arrowy = 80;
            }
            if (arrowy > 510)
            {
                arrowy = 510;
            }
            if( SDL_Flip( screen ) == -1 )
            {
                return 1;
            }
        }

        while( SDL_PollEvent( &event ) )
        {
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                stop = true;
            }
            if( event.type == SDL_MOUSEBUTTONDOWN )
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (event.button.x > 70 && event.button.x < 225 && event.button.y > 300 && event.button.y < 380)
                    {
                        playingScreenOn = true;
                        bypass = true;
                    }
                    else if (event.button.x > 70 && event.button.x < 225 && event.button.y > 390 && event.button.y < 450)
                    {
                        stop = true;
                    }
                    else if (event.button.x > 70 && event.button.x < 290 && event.button.y > 470 && event.button.y < 540)
                    {
                        creditScreenOn = true;
                        bypass = true;
                    }
                }
            }
            if ( event.type == SDL_MOUSEMOTION)
            {
                if (event.motion.x > 70 && event.motion.x < 225 && event.motion.y > 300 && event.motion.y < 380)
                {
                    apply_surface( 70, 300, outlinePlay, screen );
                    bypass = true;
                }
                else if (event.motion.x > 70 && event.motion.x < 225 && event.motion.y > 390 && event.motion.y < 450)
                {
                    apply_surface( 70, 390, outlineQuit, screen );
                    bypass = true;
                }
                else if (event.motion.x > 70 && event.motion.x < 290 && event.motion.y > 470 && event.motion.y < 540)
                {
                    apply_surface( 70, 470, outlineCredits, screen );
                    bypass = true;
                }
                else
                {
                    bypass = false;
                }
            }
        }
         //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    }

    //Free the surface and quit SDL
    clean_up();

    return 0;
}

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image using SDL_image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old image
        SDL_FreeSurface( loadedImage );
    }

    //Set Transparancy - White
    SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB(optimizedImage->format, 255, 255, 255) );

    //Return the optimized image
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    //Rectangle to hold the offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Chase!", NULL );

    //If everything initialized fine
    return true;
}

int loadncheck()
{
     //Load the image
    mainScreen = load_image( "MainScreenRevised.png" );
    outlinePlay = load_image( "Small Outline Play.png" );
    outlineQuit = load_image( "Small Outline Quit.png" );
    outlineCredits = load_image( "Large Outline Credits.png" );
    play = load_image( "Play.png" );
    quit = load_image( "Quit.png" );
    credits = load_image( "Credits.png" );
    playingScreen = load_image("Playing Screen.png");
    creditsScreen = load_image("Credits Screen.png");
    outlineGoBack = load_image("Small Outline Back.png");
    goBack = load_image("Back.png");
    arrow = load_image("Arrow.png");
    blackScreen = load_image("Black Screen Setup.png");

    //If there was a problem in loading the image
    if( mainScreen == NULL )
    {
        return 1;
    }
    if( outlinePlay == NULL )
    {
        return 1;
    }
    if( outlineQuit == NULL )
    {
        return 1;
    }
    if( outlineCredits == NULL )
    {
        return 1;
    }
    if( play == NULL )
    {
        return 1;
    }
    if( quit == NULL )
    {
        return 1;
    }
    if( credits == NULL )
    {
        return 1;
    }
    if( playingScreen == NULL )
    {
        return 1;
    }
    if( creditsScreen == NULL )
    {
        return 1;
    }
    if( outlineGoBack == NULL )
    {
        return 1;
    }
    if( goBack == NULL )
    {
        return 1;
    }
    if( arrow == NULL )
    {
        return 1;
    }
    if( blackScreen == NULL )
    {
        return 1;
    }

    return 0;
}

void clean_up()
{
    //Free the surface
    SDL_FreeSurface( mainScreen );
    SDL_FreeSurface( outlinePlay );
    SDL_FreeSurface( outlineQuit );
    SDL_FreeSurface( outlineCredits );
    SDL_FreeSurface( play );
    SDL_FreeSurface( quit );
    SDL_FreeSurface( credits );
    SDL_FreeSurface( playingScreen );
    SDL_FreeSurface( creditsScreen );
    SDL_FreeSurface( goBack );
    SDL_FreeSurface( outlineGoBack );
    SDL_FreeSurface( arrow );
    SDL_FreeSurface( blackScreen );
    SDL_FreeSurface( roto );


    //Quit SDL
    SDL_Quit();
}


