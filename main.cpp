#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_rotozoom.h"
#include <string>
#include <cmath>
#include <cstdlib>
#include <time.h>

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
SDL_Surface *redDot = NULL;
SDL_Surface *redDotBig = NULL;
SDL_Surface *redDotHuge = NULL;
SDL_Surface *redDotMassive = NULL;
SDL_Surface *greyDot = NULL;
SDL_Surface *greyDotBig = NULL;
SDL_Surface *greyDotHuge = NULL;
SDL_Surface *greyDotMassive = NULL;
SDL_Surface *speedyPower = NULL;
SDL_Surface *pacmanPower = NULL;
SDL_Surface *freezePower = NULL;
SDL_Surface *nukePower = NULL;
SDL_Surface *vortexPower = NULL;
SDL_Surface *shieldPower = NULL;
SDL_Surface *teleportPower = NULL;
SDL_Surface *gravestonePower = NULL;
SDL_Surface *ninjaPower = NULL;
SDL_Surface *bulletPower = NULL;
SDL_Surface *loseScreen = NULL;
SDL_Surface *arrowPacman = NULL;
SDL_Surface *picOne = NULL;
SDL_Surface *picTwo = NULL;
SDL_Surface *picThree = NULL;
SDL_Surface *picFour = NULL;
SDL_Surface *picFive = NULL;
SDL_Surface *picSix = NULL;
SDL_Surface *picSeven = NULL;
SDL_Surface *picEight = NULL;
SDL_Surface *picNine = NULL;
SDL_Surface *picZero = NULL;
SDL_Surface *blueDot = NULL;
SDL_Surface *blueDotBig = NULL;
SDL_Surface *blueDotHuge = NULL;
SDL_Surface *blueDotMassive = NULL;
SDL_Surface *explosion = NULL;
SDL_Surface *blackHole = NULL;
SDL_Surface *shield = NULL;
SDL_Surface *iceField = NULL;
SDL_Surface *teleporterU = NULL;
SDL_Surface *teleporterR = NULL;
SDL_Surface *teleporterD = NULL;
SDL_Surface *teleporterL = NULL;
SDL_Surface *numPlayersScreen = NULL;
SDL_Surface *onePlayer = NULL;
SDL_Surface *outlineOnePlayer = NULL;
SDL_Surface *twoPlayers = NULL;
SDL_Surface *outlineTwoPlayers = NULL;
SDL_Surface *ninjaStar = NULL;
SDL_Surface *shot = NULL;
SDL_Surface *evilDot = NULL;
SDL_Surface *evilDotBig = NULL;
SDL_Surface *evilDotHuge = NULL;
SDL_Surface *evilDotMassive = NULL;
SDL_Surface *barDot = NULL;
SDL_Surface *teleportDot = NULL;
SDL_Surface *ghostArrow = NULL;
SDL_Surface *coop = NULL;
SDL_Surface *vs = NULL;
SDL_Surface *outlineCoop = NULL;
SDL_Surface *outlineVs = NULL;
SDL_Surface *roto = NULL;
SDL_Surface *ghostRoto = NULL;

SDL_Surface *load_image( std::string filename );
void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination );
bool init();
int loadncheck();
void clean_up();
double distance( double x1, double y1, double x2, double y2 );
bool CircleCollision( double x1, double y1, double x2, double y2, double r1, double r2 );
void ScoreConversion(int score, double x,double y);
int PowerUpAssign();
double BoundaryCheckX(double x, int left, int right);
double BoundaryCheckY(double y, int up, int down);

struct Dot
{
    double x, y, dotSpeed;
    int r, type, direction, directionStart, cooldown;
    bool dead, frozen;
};

struct Power
{
    double x, y;
    int type;
    bool dead;
};

struct Vortex
{
    double x, y;
    bool exist;
    int time;
};

struct Ninja
{
    double x, y;
    bool exist;
};

struct Bullet
{
    double x, y, angle;
    bool exist;
};

int main( int argc, char* args[] )
{
    bool stop = false, skip = false, bypass = false, creditScreenOn = false, playingScreenOn = false;
    bool continueRotateRight = false, continueRotateLeft = false, continueMove = false, blast, twoPlayer;
    bool speedyOn, pacmanOn, freezeOn, nukeOn, shieldOn, teleportOn, loseScreenOn = false, restart = true, totalRestart = true;
    bool continueMoveUpDot = false, continueMoveRightDot = false, continueMoveDownDot = false, continueMoveLeftDot = false;
    bool numPlayersScreenOn = false, gravestoneOn, gravestoneArrowTouch, bulletOn, coopVsScreenOn = false, teammate;
    bool continueMoveGhostArrow = false, continueRotateGhostArrowLeft = false, continueRotateGhostArrowRight = false;
    double arrowangle, arrowx, arrowy, arrowRotateSpeed, arrowMoveSpeed, iceFieldx, iceFieldy;
    int timer, numberOfDots, wave, collidedMove, speedyTime, pacmanTime, freezeTime, nukeTime, teleportTime;
    int numberOfDotsAlive, dotsKilled, frozenTimer, blastTime, teleportCount, gravestoneTime, applyIce = 51;
    int bulletTime, reload, formation, teleportCooldown, ghostArrowangle, ghostArrowx, ghostArrowy;

    int powertime[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    Power power[5];

    Dot dot[101];

    Vortex vortex[10];

    Ninja ninja[40];

    Bullet bullet[100];

    srand(time(NULL));

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
                        creditScreenOn = false; //Needs this to quit this loop
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

        while (numPlayersScreenOn == true)
        {
            if (skip == false)
            {
                apply_surface(0, 0, numPlayersScreen, screen);
                apply_surface(262, 134, onePlayer, screen);
                apply_surface(262, 316, twoPlayers, screen);
            }

            while( SDL_PollEvent( &event ) )
            {
                if( event.type == SDL_QUIT )
                {
                        //Quit the program
                        numPlayersScreenOn = false;
                        stop = true;
                }
                if( event.type == SDL_MOUSEBUTTONDOWN )
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if (event.motion.x > 262 && event.motion.x < 562 && event.motion.y > 134 && event.motion.y < 234)
                        {
                            twoPlayer = false;
                            teammate = false;
                            playingScreenOn = true;
                            numPlayersScreenOn = false;
                        }
                        if (event.motion.x > 262 && event.motion.x < 562 && event.motion.y > 316 && event.motion.y < 416)
                        {
                            twoPlayer = true;
                            coopVsScreenOn = true;
                            numPlayersScreenOn = false;
                            skip = false;
                        }
                    }
                }
                if ( event.type == SDL_MOUSEMOTION)
                {
                    if (event.motion.x > 262 && event.motion.x < 562 && event.motion.y > 134 && event.motion.y < 234)
                    {
                        apply_surface( 262, 134, outlineOnePlayer, screen );
                        skip = true;
                    }
                    else if (event.motion.x > 262 && event.motion.x < 562 && event.motion.y > 316 && event.motion.y < 416)
                    {
                        apply_surface( 262, 316, outlineTwoPlayers, screen );
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

        while (coopVsScreenOn == true)
        {
            if (skip == false)
            {
                apply_surface(0, 0, numPlayersScreen, screen);
                apply_surface(262, 316, vs, screen);
                apply_surface(262, 134, coop, screen);
            }
            while( SDL_PollEvent( &event ) )
            {
                if( event.type == SDL_QUIT )
                {
                        //Quit the program
                        coopVsScreenOn = false;
                        stop = true;
                }
                if( event.type == SDL_MOUSEBUTTONDOWN )
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if (event.motion.x > 262 && event.motion.x < 562 && event.motion.y > 134 && event.motion.y < 234)
                        {
                            teammate = true;
                            playingScreenOn = true;
                            coopVsScreenOn = false;
                        }
                        if (event.motion.x > 262 && event.motion.x < 562 && event.motion.y > 316 && event.motion.y < 416)
                        {
                            teammate = false;
                            playingScreenOn = true;
                            coopVsScreenOn = false;
                        }
                    }
                }
                if ( event.type == SDL_MOUSEMOTION)
                {
                    if (event.motion.x > 262 && event.motion.x < 562 && event.motion.y > 134 && event.motion.y < 234)
                    {
                        apply_surface( 262, 134, outlineCoop, screen );
                        skip = true;
                    }
                    else if (event.motion.x > 262 && event.motion.x < 562 && event.motion.y > 316 && event.motion.y < 416)
                    {
                        apply_surface( 262, 316, outlineVs, screen );
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
            if (restart == true)
            {
                if (totalRestart == true)
                {
                    arrowx = 412;
                    arrowy = 225;
                    arrowangle = 90;
                    ghostArrowx = 412;
                    ghostArrowy = 225;
                    ghostArrowangle = 90;
                    dotsKilled = 0;
                    wave = 1;
                    totalRestart = false;
                }
                if (wave % 5 != 0)
                {
                    numberOfDots = (pow(wave, 2) * 40) / 225 + 10;
                    if (numberOfDots > 100)
                        numberOfDots = 100;
                }
                else
                    numberOfDots = 10;
                if (twoPlayer == true && teammate == false)
                    numberOfDots++;
                numberOfDotsAlive = numberOfDots;
                timer = wave * 5;
                if (timer > 100)
                    timer = 100;
                arrowRotateSpeed = .25 * sqrt(wave) * 5 + 1;
                arrowMoveSpeed = .25 * sqrt(wave) * 5 + 1;
                formation = rand() % 3 + 1;
                if (wave > 10)
                    formation = rand() % 4 + 1;
                if (wave % 5 == 0 || wave == 1) //1 is normal, 2 is slide only, 3 is teleport only, 4 is mixed
                    formation = 1;
                teleportCooldown = 200 - (pow(wave, 2) * 40) / 225;
                if (teleportCooldown < 100)
                    teleportCooldown = 100;
                speedyOn = false;
                pacmanOn = false;
                freezeOn = false;
                nukeOn = false;
                shieldOn = false;
                teleportOn = false;
                gravestoneOn = false;
                bulletOn = false;
                blast = false;
                restart = false;
                frozenTimer = 0;
                blastTime = 0;
                reload = 0;

                for (int i = 0; i < 20; i++)
                {
                    powertime[i] = 0;
                }

                for(int i = 0; i < numberOfDots; i++)
                {
                    if (wave == 5)
                    {
                        dot[i].r = 15;
                    }
                    else if (wave == 10)
                    {
                        dot[i].r = 23;
                    }
                    else if (wave >= 15 && wave % 5 == 0)
                    {
                        dot[i].r = 30;
                    }
                    else
                    {
                        dot[i].r = 10;
                    }
                    dot[i].x = rand() % (815 - 2 * dot[i].r - 5 - dot[i].r) + 5 + dot[i].r;
                    dot[i].y = rand() % (545 - 2 * dot[i].r - 80 - dot[i].r) + 80 + dot[i].r;
                    dot[i].dead = false;
                    dot[i].frozen = false;
                    dot[i].dotSpeed = (.25 * sqrt(wave) * (rand() % 10 + 5) / 10.0);
                    if (formation == 2)
                    {
                        dot[i].type = 2;
                        dot[i].direction = rand() % 2 + 1; //0 is up-down, 1 is left-right;
                        if (rand() % 2 + 1 == 1)
                            dot[i].directionStart = 1;
                        else
                            dot[i].directionStart = -1;
                        dot[i].dotSpeed *= 3;
                    }
                    else if (formation == 3)
                        dot[i].type = 3;
                    else if (formation == 4)
                    {
                        switch (rand() % 3 + 1)
                        {
                            case 1:
                                dot[i].type = 1;
                                break;
                            case 2:
                                dot[i].type = 2;
                                dot[i].direction = rand() % 3;
                                if (rand() % 2 + 1 == 1)
                                    dot[i].directionStart = 1;
                                else
                                    dot[i].directionStart = -1;
                                dot[i].dotSpeed *= 3;
                                break;
                            case 3:
                                dot[i].type = 3;
                                break;
                        }
                    }
                    else
                        dot[i].type = 1;
                }

                for(int i = 0; i < 10; i++)
                {
                    vortex[i].exist = false;
                    vortex[i].time = 0;
                }

                for(int i = 0; i < 40; i++)
                {
                    ninja[i].exist = false;
                }

                for(int i = 0; i < 100; i++)
                {
                    bullet[i].exist = false;
                }

                for(int i = 0; i < 5; i++)
                {
                    power[i].x = rand() % 780 + 5;
                    power[i].y = rand() % 430 + 80;
                    power[i].dead = false;
                    power[i].type = PowerUpAssign();
                }
            }

            apply_surface(0, 0, blackScreen, screen);

            if (teammate == true)
            {
                ghostRoto = rotozoomSurface(ghostArrow, ghostArrowangle - 90, .75, 1);
                apply_surface(ghostArrowx - 15, ghostArrowy - 15, ghostRoto, screen);

            }

            //Scaled image to .75 times origional
            if (pacmanOn == true)
            {
                roto = rotozoomSurface(arrowPacman, arrowangle - 90, .75, 1);
                apply_surface(arrowx - 15, arrowy - 15, roto, screen);
            }
            else
            {
                roto = rotozoomSurface(arrow, arrowangle - 90, .75, 1);
                apply_surface(arrowx - 15, arrowy - 15, roto, screen);
            }

            if (shieldOn == true)
                apply_surface(arrowx - 19, arrowy - 19, shield, screen);

            if (teleportOn == true)
            {
                switch(teleportCount)
                {
                    case 1:
                        apply_surface(arrowx - 15, arrowy - 15, teleporterU, screen);
                        teleportCount++;
                        break;
                    case 2:
                        apply_surface(arrowx - 15, arrowy - 15, teleporterR, screen);
                        teleportCount++;
                        break;
                    case 3:
                        apply_surface(arrowx - 15, arrowy - 15, teleporterD, screen);
                        teleportCount++;
                        break;
                    case 4:
                        apply_surface(arrowx - 15, arrowy - 15, teleporterL, screen);
                        teleportCount++;
                        break;
                }
                if (teleportCount == 5)
                    teleportCount = 1;
            }

            for (int i = 0; i < 40; i++)
            {
                if (ninja[i].exist == true)
                    apply_surface(ninja[i].x - 25, ninja[i].y - 25, ninjaStar, screen);
            }

            for (int i = 0; i < 10; i++)
            {
                if (vortex[i].exist == true)
                    apply_surface(vortex[i].x - 90, vortex[i].y - 90, blackHole, screen);
            }

            for (int i = 0; i < 100; i++)
            {
                if (bullet[i].exist == true)
                    apply_surface(bullet[i].x - 1, bullet[i].y -  1, shot, screen);
            }

            //Displays the image with rotation accounted for
            apply_surface(0, 0, playingScreen, screen);

            ScoreConversion(wave, 220, 15);
            ScoreConversion(dotsKilled, 650, 15);

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
                            restart = true;
                            totalRestart = true;
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
                if (twoPlayer == true && teammate == false)
                {
                    if (keystates[SDLK_w] == true)
                        continueMoveUpDot = true;
                    else
                        continueMoveUpDot = false;
                    if (keystates[SDLK_d] == true)
                        continueMoveRightDot = true;
                    else
                        continueMoveRightDot = false;
                    if (keystates[SDLK_s] == true)
                        continueMoveDownDot = true;
                    else
                        continueMoveDownDot = false;
                    if (keystates[SDLK_a] == true)
                        continueMoveLeftDot = true;
                    else
                        continueMoveLeftDot = false;
                }
                if (twoPlayer == true && teammate == true)
                {
                    if (keystates[SDLK_w] == true)
                        continueMoveGhostArrow = true;
                    else
                        continueMoveGhostArrow = false;
                    if (keystates[SDLK_d] == true)
                        continueRotateGhostArrowRight = true;
                    else
                        continueRotateGhostArrowRight = false;
                    if (keystates[SDLK_a] == true)
                        continueRotateGhostArrowLeft = true;
                    else
                        continueRotateGhostArrowLeft = false;
                }
                if (keystates[SDLK_SPACE] == true && teleportOn == false)
                    arrowangle -= 180;
                if (keystates[SDLK_SPACE] == true && teleportOn == true)
                {
                    arrowx += 100 * cos((arrowangle) * .01745329251994);
                    arrowy -= 100 * sin((arrowangle) * .01745329251994);
                }
            }
            if (continueRotateRight == true)
            {
                //Rotate to the right
                arrowangle -= arrowRotateSpeed;
            }
            if (continueRotateLeft == true)
            {
                //Rotate to the left
                arrowangle += arrowRotateSpeed;
            }
            if (continueRotateGhostArrowRight == true)
            {
                //Rotate to the right
                ghostArrowangle -= 5;
            }
            if (continueRotateGhostArrowLeft == true)
            {
                //Rotate to the left
                ghostArrowangle += 5;
            }
            if (continueMove == true)
            {
                if (speedyOn == true)
                {
                //Move arrow in direction facing
                arrowx += 1.5 * arrowMoveSpeed * cos((arrowangle) * .01745329251994); //Coverts degrees to radians
                arrowy -= 1.5 * arrowMoveSpeed * sin((arrowangle) * .01745329251994);
                }
                if (speedyOn == false)
                {
                //Move arrow in direction facing
                arrowx += arrowMoveSpeed * cos((arrowangle) * .01745329251994); //Coverts degrees to radians
                arrowy -= arrowMoveSpeed * sin((arrowangle) * .01745329251994);
                }
            }
            if (continueMoveGhostArrow == true)
            {
                ghostArrowx += 4 * cos((ghostArrowangle) * .01745329251994); //Coverts degrees to radians
                ghostArrowy -= 4 * sin((ghostArrowangle) * .01745329251994);
            }

            arrowx = BoundaryCheckX(arrowx, 20, 800);
            arrowy = BoundaryCheckY(arrowy, 95, 525);

            if (teammate == true)
            {
                ghostArrowx = BoundaryCheckX(ghostArrowx, 20, 800);
                ghostArrowy = BoundaryCheckY(ghostArrowy, 95, 525);
            }

            if (applyIce < 50)
            {
                apply_surface(iceFieldx, iceFieldy, iceField, screen);
                applyIce++;
            }

            if (timer < 200)
            {
                for(int i = 0; i < numberOfDots; i++)
                {
                    if (twoPlayer == true && teammate == false && i == 0)
                    {
                        if (wave == 5)
                        {
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, evilDotBig, screen);
                        }
                        else if (wave == 10)
                        {
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, evilDotHuge, screen);
                        }
                        else if (wave >= 15 && wave % 5 == 0)
                        {
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, evilDotMassive, screen);
                        }
                        else
                        {
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, evilDot, screen);
                        }
                    }
                    else
                    {
                        if (wave == 5)
                        {
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, greyDotBig, screen);
                        }
                        else if (wave == 10)
                        {
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, greyDotHuge, screen);
                        }
                        else if (wave >= 15 && wave % 5 == 0)
                        {
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, greyDotMassive, screen);
                        }
                        else
                        {
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, greyDot, screen);
                        }
                    }
                }
            }
            else
            {
                for(int i = 0; i < numberOfDots; i++)
                {
                    if (dot[i].dead == true)
                        continue;
                    if (dot[i].frozen == true)
                    {
                        frozenTimer++;
                        if (frozenTimer >= 2000)
                            dot[i].frozen = false;
                    }
                    if (freezeOn == true && CircleCollision(arrowx,arrowy, dot[i].x, dot[i].y, 150, dot[i].r) == true)
                        dot[i].frozen = true;
                    for (int k = 0; k < 10; k++)
                    {
                        if (vortex[k].exist == true && CircleCollision(vortex[k].x, vortex[k].y, dot[i].x, dot[i].y, 90, dot[i].r) == true)
                        {
                            dot[i].dead = true;
                            numberOfDotsAlive--;
                            dotsKilled++;
                            break;

                        }
                    }
                    for (int k = 0; k < 40; k++)
                    {
                        if (ninja[k].exist == true && CircleCollision(ninja[k].x, ninja[k].y, dot[i].x, dot[i].y, 25, dot[i].r) == true)
                        {
                            dot[i].dead = true;
                            numberOfDotsAlive--;
                            dotsKilled++;
                            break;

                        }
                    }
                    for (int k = 0; k < 100; k++)
                    {
                        if (bullet[k].exist == true && CircleCollision(bullet[k].x, bullet[k].y, dot[i].x, dot[i].y, 1, dot[i].r) == true)
                        {
                            dot[i].dead = true;
                            numberOfDotsAlive--;
                            dotsKilled++;
                            break;

                        }
                    }
                    if (blast == true && CircleCollision(arrowx, arrowy, dot[i].x, dot[i].y, 50, dot[i].r) == true)
                    {
                        if (arrowx > dot[i].x)
                            dot[i].x -= 50;
                        else
                            dot[i].x += 50;
                        if (arrowy > dot[i].y)
                            dot[i].y -= 50;
                        else
                            dot[i].y += 50;
                    }
                    if (nukeOn == true)
                    {
                        dot[i].dead = true;
                        numberOfDotsAlive--;
                        dotsKilled++;
                    }
                    if (wave == 5)
                    {
                        if (dot[i].frozen == true)
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, blueDotBig, screen);
                        else if (nukeOn == true)
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, explosion, screen);
                        else
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, redDotBig, screen);
                    }
                    if (wave == 10)
                    {
                        if (dot[i].frozen == true)
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, blueDotHuge, screen);
                        else if (nukeOn == true)
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, explosion, screen);
                        else
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, redDotHuge, screen);
                    }
                    if (wave >= 15 && wave % 5 == 0)
                    {
                        if (dot[i].frozen == true)
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, blueDotMassive, screen);
                        else if (nukeOn == true)
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, explosion, screen);
                        else
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, redDotMassive, screen);
                    }
                    if (wave % 5 != 0)
                    {
                        if (dot[i].frozen == true)
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, blueDot, screen);
                        else if (nukeOn == true)
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, explosion, screen);
                        else if (dot[i].type == 2)
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, barDot, screen);
                        else if (dot[i].type == 3)
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, teleportDot, screen);
                        else
                            apply_surface(dot[i].x - dot[i].r, dot[i].y - dot[i].r, redDot, screen);
                    }
                    if (CircleCollision(arrowx, arrowy, dot[i].x, dot[i].y, 11.25, dot[i].r) == true)
                    {
                        if (pacmanOn == true)
                        {
                            dot[i].dead = true;
                            numberOfDotsAlive--;
                            dotsKilled++;
                            break;
                        }
                        if (shieldOn == true)
                        {
                            shieldOn = false;
                            blast = true;
                            blastTime = 0;
                            break;
                        }
                        playingScreenOn = false;
                        loseScreenOn = true;
                    }
                    for (int u = 0; u < numberOfDots; u++)
                    {
                        //Checks for same dot
                        if (u == i)
                        {
                            continue;
                        }
                        //Prevents dots from getting too close to each other
                        if (dot[i].type == 1 && dot[u].type == 1 && CircleCollision(dot[i].x, dot[i].y, dot[u].x, dot[u].y, dot[i].r, dot[u].r) == true)
                        {
                            collidedMove = rand() % 4 + 1;
                            switch(collidedMove)
                            {
                                case 1:
                                    dot[i].x += .5;
                                    dot[i].y += .5;
                                    break;
                                case 2:
                                    dot[i].x -= .5;
                                    dot[i].y += .5;
                                    break;
                                case 3:
                                    dot[i].x += .5;
                                    dot[i].y -= .5;
                                    break;
                                case 4:
                                    dot[i].x -= .5;
                                    dot[i].y -= .5;
                                    break;
                            }
                        }
                    }

                    //Chasing/Moving
                    if (dot[i].type == 1)
                    {
                        if (twoPlayer == false || i != 0 || (twoPlayer == true && teammate == true))
                        {
                            if (dot[i].frozen == true)
                            {
                                if ((arrowx -  5 * cos((arrowangle) * .01745329251994)) > dot[i].x)
                                    dot[i].x += (dot[i].dotSpeed / 2);
                                else
                                    dot[i].x -= (dot[i].dotSpeed / 2);
                                if ((arrowy + 5 * sin((arrowangle) * .01745329251994)) > dot[i].y)
                                    dot[i].y += (dot[i].dotSpeed / 2);
                                else
                                    dot[i].y -= (dot[i].dotSpeed / 2);
                            }
                            else
                            {
                                if ((arrowx -  5 * cos((arrowangle) * .01745329251994)) > dot[i].x)
                                    dot[i].x += dot[i].dotSpeed;
                                else
                                    dot[i].x -= dot[i].dotSpeed;
                                if ((arrowy + 5 * sin((arrowangle) * .01745329251994)) > dot[i].y)
                                    dot[i].y += dot[i].dotSpeed;
                                else
                                    dot[i].y -= dot[i].dotSpeed;
                            }
                        }
                        if (twoPlayer == true && teammate == false && i == 0 && dot[i].frozen == false)
                        {
                            if (continueMoveUpDot == true)
                            {
                                dot[i].y -= dot[i].dotSpeed;
                            }
                            if (continueMoveRightDot == true)
                            {
                                dot[i].x += dot[i].dotSpeed;
                            }
                            if (continueMoveDownDot == true)
                            {
                                dot[i].y += dot[i].dotSpeed;
                            }
                            if (continueMoveLeftDot == true)
                            {
                                dot[i].x -= dot[i].dotSpeed;
                            }
                        }
                        if (twoPlayer == true && teammate == false && i == 0 && dot[i].frozen == true)
                        {
                            if (continueMoveUpDot == true)
                            {
                                dot[i].y -= .5 * dot[i].dotSpeed;
                            }
                            if (continueMoveRightDot == true)
                            {
                                dot[i].x += .5 * dot[i].dotSpeed;
                            }
                            if (continueMoveDownDot == true)
                            {
                                dot[i].y += .5 * dot[i].dotSpeed;
                            }
                            if (continueMoveLeftDot == true)
                            {
                                dot[i].x -= .5 * dot[i].dotSpeed;
                            }
                        }
                    }
                    if (dot[i].type == 2)
                    {
                        if (dot[i].direction == 1)
                        {
                            if (twoPlayer == false || i != 0 || (twoPlayer == true && teammate == true))
                            {
                                if (dot[i].frozen == true)
                                {
                                    dot[i].y += .5 * dot[i].dotSpeed * dot[i].directionStart;
                                    if (dot[i].y < 85 || dot[i].y > 530)
                                        dot[i].directionStart *= -1;
                                }
                                else
                                {
                                    dot[i].y += dot[i].dotSpeed * dot[i].directionStart;
                                    if (dot[i].y < 85 || dot[i].y > 530)
                                        dot[i].directionStart *= -1;
                                }
                            }
                            if (twoPlayer == true && teammate == false && i == 0 && dot[i].frozen == false)
                            {
                                if (continueMoveUpDot == true)
                                {
                                    dot[i].y -= dot[i].dotSpeed;
                                }
                                if (continueMoveDownDot == true)
                                {
                                    dot[i].y += dot[i].dotSpeed;
                                }
                            }
                            if (twoPlayer == true && teammate == false && i == 0 && dot[i].frozen == true)
                            {
                                if (continueMoveUpDot == true)
                                {
                                    dot[i].y -= .5 * dot[i].dotSpeed;
                                }
                                if (continueMoveDownDot == true)
                                {
                                    dot[i].y += .5 * dot[i].dotSpeed;
                                }
                            }
                        }
                        else
                        {
                            if (twoPlayer == false || i != 0 || (twoPlayer == true && teammate == true))
                            {
                                if (dot[i].frozen == true)
                                {
                                    dot[i].x += .5 * dot[i].dotSpeed * dot[i].directionStart;
                                    if (dot[i].x < 15 || dot[i].x > 800)
                                        dot[i].directionStart *= -1;
                                }
                                else
                                {
                                    dot[i].x += dot[i].dotSpeed * dot[i].directionStart;
                                    if (dot[i].x < 15 || dot[i].x > 800)
                                        dot[i].directionStart *= -1;
                                }
                            }
                            if (twoPlayer == true && teammate == false && i == 0 && dot[i].frozen == false)
                            {
                                if (continueMoveUpDot == true)
                                {
                                    dot[i].y -= dot[i].dotSpeed;
                                }
                                if (continueMoveRightDot == true)
                                {
                                    dot[i].x += dot[i].dotSpeed;
                                }
                                if (continueMoveDownDot == true)
                                {
                                    dot[i].y += dot[i].dotSpeed;
                                }
                                if (continueMoveLeftDot == true)
                                {
                                    dot[i].x -= dot[i].dotSpeed;
                                }
                            }
                            if (twoPlayer == true && teammate == false && i == 0 && dot[i].frozen == true)
                            {
                                if (continueMoveUpDot == true)
                                {
                                    dot[i].y -= .5 * dot[i].dotSpeed;
                                }
                                if (continueMoveRightDot == true)
                                {
                                    dot[i].x += .5 * dot[i].dotSpeed;
                                }
                                if (continueMoveDownDot == true)
                                {
                                    dot[i].y += .5 * dot[i].dotSpeed;
                                }
                                if (continueMoveLeftDot == true)
                                {
                                    dot[i].x -= .5 * dot[i].dotSpeed;
                                }
                            }
                        }
                    }
                    if (dot[i].type == 3)
                    {
                        if (dot[i].cooldown >= teleportCooldown)
                        {
                            if (twoPlayer == false || i != 0 || (twoPlayer == true && teammate == true))
                            {
                                if (dot[i].frozen == true)
                                {
                                    dot[i].x += 50 * cos((rand() % 360 + 1) * .01745329251994) * dot[i].dotSpeed;
                                    dot[i].y -= 50 * sin((rand() % 360 + 1) * .01745329251994) * dot[i].dotSpeed;
                                }
                                else
                                {
                                    dot[i].x += 100 * cos((rand() % 360 + 1) * .01745329251994) * dot[i].dotSpeed;
                                    dot[i].y -= 100 * sin((rand() % 360 + 1) * .01745329251994) * dot[i].dotSpeed;
                                }
                                dot[i].cooldown = 0;
                            }
                            if (twoPlayer == true && teammate == false && i == 0 && dot[i].frozen == false)
                            {
                                if (continueMoveUpDot == true)
                                {
                                    dot[i].y -= rand() % 100 + 1;
                                    dot[i].cooldown = 0;
                                }
                                if (continueMoveRightDot == true)
                                {
                                    dot[i].x += rand() % 100 + 1;
                                    dot[i].cooldown = 0;
                                }
                                if (continueMoveDownDot == true)
                                {
                                    dot[i].y += rand() % 100 + 1;
                                    dot[i].cooldown = 0;
                                }
                                if (continueMoveLeftDot == true)
                                {
                                    dot[i].x -= rand() % 100 + 1;
                                    dot[i].cooldown = 0;
                                }
                            }
                            if (twoPlayer == true && teammate == false && i == 0 && dot[i].frozen == true)
                            {
                                if (continueMoveUpDot == true)
                                {
                                    dot[i].y -= rand() % 50 + 1;
                                    dot[i].cooldown = 0;
                                }
                                if (continueMoveRightDot == true)
                                {
                                    dot[i].x += rand() % 50 + 1;
                                    dot[i].cooldown = 0;
                                }
                                if (continueMoveDownDot == true)
                                {
                                    dot[i].y += rand() % 50 + 1;
                                    dot[i].cooldown = 0;
                                }
                                if (continueMoveLeftDot == true)
                                {
                                    dot[i].x -= rand() % 50 + 1;
                                    dot[i].cooldown = 0;
                                }
                            }
                        }
                        else
                            dot[i].cooldown++;
                    }
                    //Makes it harder for user if stalls
                    if (timer % 3000 == 2999)
                        dot[i].dotSpeed += 1.0;

                    //Stops dots from going over boundaries
                    dot[i].x = BoundaryCheckX(dot[i].x, 25 - dot[i].r, 780 + 2 * dot[i].r);
                    dot[i].y = BoundaryCheckY(dot[i].y, 95 - dot[i].r, 510 + 2 * dot[i].r);

                    for (int u = 0; u < 5; u++)
                    {
                        if (CircleCollision(dot[i].x, dot[i].y, power[u].x, power[u].y, dot[i].r, 10) == true && i == 0 && twoPlayer == true && teammate == false)
                        {
                            power[u].dead = true;
                            if (power[u].type == 9)
                            {
                                gravestoneOn = true;
                                gravestoneArrowTouch = false;
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        gravestoneTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }

            if (numberOfDotsAlive <= 0)
            {
                wave++;
                restart = true;
            }

            for (int u = 0; u < 5; u++)
            {
                if (power[u].dead == true)
                {
                    power[u].x = rand() % 780 + 5;
                    power[u].y = rand() % 430 + 80;
                    power[u].dead = false;
                    power[u].type = PowerUpAssign();
                }
                if (teammate == true)
                {
                    if (CircleCollision(ghostArrowx, ghostArrowy, power[u].x, power[u].y, 12, 10) == true)
                    {
                        power[u].dead = true;
                        switch(power[u].type)
                        {
                            case 1:
                                speedyOn = true;
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        speedyTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                            case 2:
                                pacmanOn = true;
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        pacmanTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                            case 3:
                                shieldOn = true;
                                break;
                            case 4:
                                freezeOn = true;
                                apply_surface(arrowx - 150, arrowy - 150, iceField, screen);
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        freezeTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                            case 5:
                                for (int k = 0; k < 10; k++)
                                {
                                    if (vortex[k].exist == true)
                                        continue;
                                    else
                                    {
                                        vortex[k].exist = true;
                                        vortex[k].x = ghostArrowx;
                                        vortex[k].y = ghostArrowy;
                                        break;
                                    }
                                }
                                break;
                            case 6:
                                nukeOn = true;
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        nukeTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                            case 7:
                                teleportOn = true;
                                teleportCount = 1;
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        teleportTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                            case 8:
                                for (int u = 0; u < 33; u += 8)
                                {
                                    //Need to separate these stars by groups of 8
                                    if (ninja[u].exist == false && ninja[u + 1].exist == false && ninja[u + 2].exist == false && ninja[u + 3].exist == false && ninja[u + 4].exist == false && ninja[u + 5].exist == false && ninja[u + 6].exist == false && ninja[u + 7].exist == false)
                                    {
                                        for (int k = u; k < (u + 8); k++)
                                        {
                                            ninja[k].exist = true;
                                            ninja[k].x = ghostArrowx;
                                            ninja[k].y = ghostArrowy;
                                        }
                                        break;
                                    }
                                }
                                break;
                            case 9:
                                gravestoneOn = true;
                                gravestoneArrowTouch = true;
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        gravestoneTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                            case 10:
                                bulletOn = true;
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        bulletTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                        }
                    }
                }
                else
                {
                    if (CircleCollision(arrowx, arrowy, power[u].x, power[u].y, 15, 10) == true)
                    {
                        power[u].dead = true;
                        switch(power[u].type)
                        {
                            case 1:
                                speedyOn = true;
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        speedyTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                            case 2:
                                pacmanOn = true;
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        pacmanTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                            case 3:
                                shieldOn = true;
                                break;
                            case 4:
                                freezeOn = true;
                                applyIce = 0; //Displays the image longer
                                iceFieldx = arrowx - 150;
                                iceFieldy = arrowy - 150;
                                //apply_surface(arrowx - 150, arrowy - 150, iceField, screen);
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        freezeTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                            case 5:
                                for (int k = 0; k < 10; k++)
                                {
                                    if (vortex[k].exist == true)
                                        continue;
                                    else
                                    {
                                        vortex[k].exist = true;
                                        vortex[k].x = arrowx;
                                        vortex[k].y = arrowy;
                                        break;
                                    }
                                }
                                break;
                            case 6:
                                nukeOn = true;
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        nukeTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                            case 7:
                                teleportOn = true;
                                teleportCount = 1;
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        teleportTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                            case 8:
                                for (int u = 0; u < 33; u += 8)
                                {
                                    //Need to separate these stars by groups of 8
                                    if (ninja[u].exist == false && ninja[u + 1].exist == false && ninja[u + 2].exist == false && ninja[u + 3].exist == false && ninja[u + 4].exist == false && ninja[u + 5].exist == false && ninja[u + 6].exist == false && ninja[u + 7].exist == false)
                                    {
                                        for (int k = u; k < (u + 8); k++)
                                        {
                                            ninja[k].exist = true;
                                            ninja[k].x = arrowx;
                                            ninja[k].y = arrowy;
                                        }
                                        break;
                                    }
                                }
                                break;
                            case 9:
                                gravestoneOn = true;
                                gravestoneArrowTouch = true;
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        gravestoneTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                            case 10:
                                bulletOn = true;
                                for (int k = 0; k < 20; k++)
                                {
                                    if (powertime[k] > 0)
                                        continue;
                                    else
                                    {
                                        bulletTime = k;
                                        powertime[k] = 1;
                                        break;
                                    }
                                }
                                break;
                        }
                    }
                }
                switch(power[u].type)
                {
                    case 1:
                        apply_surface(power[u].x, power[u].y, speedyPower, screen);
                        break;
                    case 2:
                        apply_surface(power[u].x, power[u].y, pacmanPower, screen);
                        break;
                    case 3:
                        apply_surface(power[u].x, power[u].y, shieldPower, screen);
                        break;
                    case 4:
                        apply_surface(power[u].x, power[u].y, freezePower, screen);
                        break;
                    case 5:
                        apply_surface(power[u].x, power[u].y, vortexPower, screen);
                        break;
                    case 6:
                        apply_surface(power[u].x, power[u].y, nukePower, screen);
                        break;
                    case 7:
                        apply_surface(power[u].x, power[u].y, teleportPower, screen);
                        break;
                    case 8:
                        apply_surface(power[u].x, power[u].y, ninjaPower, screen);
                        break;
                    case 9:
                        apply_surface(power[u].x, power[u].y, gravestonePower, screen);
                        break;
                    case 10:
                        apply_surface(power[u].x, power[u].y, bulletPower, screen);
                        break;
                }
            }
            for (int k = 0; k < 20; k++)
            {
                if (powertime[k] == 0)
                    continue;
                if (speedyOn == true && k == speedyTime)
                {
                    powertime[k]++;
                    if (powertime[k] >= 500)
                    {
                        powertime[k] = 0;
                        speedyOn = false;
                    }
                }
                if (pacmanOn == true && k == pacmanTime)
                {
                    powertime[k]++;
                    if (powertime[k] >= 300)
                    {
                        powertime[k] = 0;
                        pacmanOn = false;
                    }
                }
                if (freezeOn == true && k == freezeTime)
                {
                    powertime[k]++;
                    frozenTimer = 0;
                    if (powertime[k] >= 3)
                    {
                        powertime[k] = 0;
                        freezeOn = false;
                    }
                }
                if (nukeOn == true && k == nukeTime)
                {
                    powertime[k]++;
                    if (powertime[k] >= 3)
                    {
                        powertime[k] = 0;
                        nukeOn = false;
                    }
                }
                if (teleportOn == true && k == teleportTime)
                {
                    powertime[k]++;
                    if (powertime[k] >= 700)
                    {
                        powertime[k] = 0;
                        teleportOn = false;
                    }
                }
                if (gravestoneOn == true && k == gravestoneTime)
                {
                    powertime[k]++;
                    if (powertime[k] >= 3)
                    {
                        powertime[k] = 0;
                        gravestoneOn = false;
                    }
                }
                if (bulletOn == true && k == bulletTime)
                {
                    powertime[k]++;
                    if (powertime[k] >= 500)
                    {
                        powertime[k] = 0;
                        bulletOn = false;
                    }
                }
            }

            for (int k = 0; k < 10; k++)
            {
                if (vortex[k].exist == true)
                {
                    vortex[k].time++;
                    if (vortex[k].time >= 300)
                    {
                        vortex[k].time = 0;
                        vortex[k].exist = false;
                    }
                }
            }

            for (int k = 0; k < 40; k++)
            {
                if (ninja[k].x < 30 || ninja[k].x > 795 || ninja[k].y < 105 || ninja[k].y > 520)
                {
                    ninja[k].exist = false;
                }
                if (ninja[k].exist == true)
                {
                    if (k == 0)
                    {
                        ninja[k].x += 5 * cos((0.0) * .01745329251994);
                        ninja[k].y -= 5 * sin((0.0) * .01745329251994);
                        continue;
                    }
                    for (int w = 8; w > 0; w--)
                    {
                        if (k % w == 0)
                        {
                            ninja[k].x += 5 * cos((45 * w) * .01745329251994);
                            ninja[k].y -= 5 * sin((45 * w) * .01745329251994);
                            break;
                        }
                    }
                }
            }

            if (gravestoneOn == true && gravestoneArrowTouch == true)
            {
                for (int u = numberOfDots - 1; u >= 0; u--)
                {
                    if (dot[u].dead == true)
                        continue;
                    else
                    {
                        dot[u].dead = true;
                        numberOfDotsAlive--;
                        dotsKilled++;
                        break;
                    }
                }
            }
            if (gravestoneOn == true && gravestoneArrowTouch == false)
            {
                for (int u = 1; u < numberOfDots; u++)
                {
                    if (dot[u].dead == false)
                        continue;
                    else
                    {
                        dot[u].dead = false;
                        numberOfDotsAlive++;
                        dotsKilled--;
                        totalRestart = false;
                        break;
                    }
                }
            }

            if (bulletOn == true)
            {
                if (reload == 5)
                {
                    for (int u = 0; u < 100; u++)
                    {
                            if (bullet[u].exist == true)
                                continue;
                            bullet[u].exist = true;
                            if (teammate == true)
                            {
                                bullet[u].x = ghostArrowx;
                                bullet[u].y = ghostArrowy;
                                bullet[u].angle = ghostArrowangle;
                            }
                            else
                            {
                                bullet[u].x = arrowx;
                                bullet[u].y = arrowy;
                                bullet[u].angle = arrowangle;
                            }
                            break;
                    }
                    reload = 0;
                }
                else
                    reload++;
            }
            for (int u = 0; u < 100; u++)
            {
                if (bullet[u].exist == true)
                {
                    bullet[u].x += 10 * cos((bullet[u].angle) * .01745329251994);
                    bullet[u].y -= 10 * sin((bullet[u].angle) * .01745329251994);
                    if (bullet[u].x < 6 || bullet[u].x > 819 || bullet[u].y < 81 || bullet[u].y > 544)
                    {
                        bullet[u].exist = false;
                    }
                }
            }

            timer++;
            if (blastTime == 1)
                blast = false;
            blastTime++;

            if( SDL_Flip( screen ) == -1 )
            {
                return 1;
            }
        }

        while (loseScreenOn == true)
        {
            if (skip == false)
            {
                apply_surface(0, 0, loseScreen, screen);
                apply_surface(550, 450, goBack, screen);
                ScoreConversion(dotsKilled, 320, 330);
            }

            while( SDL_PollEvent( &event ) )
            {
                if( event.type == SDL_QUIT )
                {
                        //Quit the program
                        loseScreenOn = false;
                        stop = true;
                }
                if( event.type == SDL_MOUSEBUTTONDOWN )
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        if (event.button.x > 550 && event.button.x < 705 && event.button.y > 450 && event.button.y < 530)
                        {
                            loseScreenOn = false;
                            playingScreenOn = false;
                            restart = true;
                            totalRestart = true;
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

                Uint8 *keystates = SDL_GetKeyState(NULL);


                if (keystates[SDLK_SPACE] == true)
                {
                    loseScreenOn = false;
                    restart = true;
                    totalRestart = true;
                    playingScreenOn = true;
                }
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
                        numPlayersScreenOn = true;
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
            if ( event.type == SDL_MOUSEMOTION )
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
    arrow = load_image("ArrowCircular.png");
    blackScreen = load_image("Black Screen Setup.png");
    redDot = load_image("Red Dot.png");
    redDotBig = load_image("Red Dot Big.png");
    redDotHuge = load_image("Red Dot Huge.png");
    redDotMassive = load_image("Red Dot Massive.png");
    greyDot = load_image("Grey Dot.png");
    greyDotBig = load_image("Grey Dot Big.png");
    greyDotHuge = load_image("Grey Dot Huge.png");
    greyDotMassive = load_image("Grey Dot Massive.png");
    speedyPower = load_image("Speedy Power.png");
    pacmanPower = load_image("Pacman Power.png");
    freezePower = load_image("Freeze Power.png");
    nukePower = load_image("Nuke Power.png");
    vortexPower = load_image("Vortex Power.png");
    shieldPower = load_image("Shield Power.png");
    teleportPower = load_image("Teleport Power.png");
    ninjaPower = load_image("Ninja Power.png");
    gravestonePower = load_image("Death Power.png");
    bulletPower = load_image("Bullet Power.png");
    loseScreen = load_image("Lose Screen.png");
    arrowPacman = load_image("ArrowCircularPacman.png");
    picOne = load_image("One.png");
    picTwo = load_image("Two.png");
    picThree = load_image("Three.png");
    picFour = load_image("Four.png");
    picFive = load_image("Five.png");
    picSix = load_image("Six.png");
    picSeven = load_image("Seven.png");
    picEight = load_image("Eight.png");
    picNine = load_image("Nine.png");
    picZero = load_image("Zero.png");
    blueDot = load_image("Blue Dot.png");
    blueDotBig = load_image("Blue Dot Big.png");
    blueDotHuge = load_image("Blue Dot Huge.png");
    blueDotMassive = load_image("Blue Dot Massive.png");
    explosion = load_image("Explosion.png");
    blackHole = load_image("Vortex.png");
    shield = load_image("Shield.png");
    iceField = load_image("Ice Field.png");
    teleporterU = load_image("Teleport Symbol Up.png");
    teleporterR = load_image("Teleport Symbol Right.png");
    teleporterD = load_image("Teleport Symbol Down.png");
    teleporterL = load_image("Teleport Symbol Left.png");
    numPlayersScreen = load_image("NumPlayerScreen.png");
    onePlayer = load_image("One Player.png");
    outlineOnePlayer = load_image("Outline One Player.png");
    twoPlayers = load_image("Two Players.png");
    outlineTwoPlayers = load_image("Outline Two Players.png");
    ninjaStar = load_image("Ninja Star.png");
    shot = load_image("Bullet.png");
    evilDot = load_image("Evil Grey Dot.png");
    evilDotBig = load_image("Evil Grey Dot Big.png");
    evilDotHuge = load_image("Evil Grey Dot Huge.png");
    evilDotMassive = load_image("Evil Grey Dot Massive.png");
    barDot = load_image("Bar Dot.png");
    teleportDot = load_image("Teleport Dot.png");
    ghostArrow = load_image("ArrowCircularGhost.png");
    coop = load_image("Cooperative.png");
    vs = load_image("Versus.png");
    outlineCoop = load_image("Outline Cooperative.png");
    outlineVs = load_image("Outline Versus.png");


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
    if( redDot == NULL )
    {
        return 1;
    }
    if( redDotBig == NULL )
    {
        return 1;
    }
    if( redDotHuge == NULL )
    {
        return 1;
    }
    if( redDotMassive == NULL )
    {
        return 1;
    }
    if( greyDot == NULL )
    {
        return 1;
    }
    if( greyDotBig == NULL )
    {
        return 1;
    }
    if( greyDotHuge == NULL )
    {
        return 1;
    }
    if( greyDotMassive == NULL )
    {
        return 1;
    }
    if( speedyPower == NULL )
    {
        return 1;
    }
    if( pacmanPower == NULL )
    {
        return 1;
    }
    if( freezePower == NULL )
    {
        return 1;
    }
    if( nukePower == NULL )
    {
        return 1;
    }
    if( vortexPower == NULL )
    {
        return 1;
    }
    if( shieldPower == NULL )
    {
        return 1;
    }
    if( teleportPower == NULL )
    {
        return 1;
    }
    if( ninjaPower == NULL )
    {
        return 1;
    }
    if( gravestonePower == NULL )
    {
        return 1;
    }
    if( bulletPower == NULL )
    {
        return 1;
    }
    if( loseScreen == NULL )
    {
        return 1;
    }
    if( arrowPacman == NULL )
    {
        return 1;
    }
    if( picOne == NULL )
    {
        return 1;
    }
    if( picTwo == NULL )
    {
        return 1;
    }
    if( picThree == NULL )
    {
        return 1;
    }
    if( picFour == NULL )
    {
        return 1;
    }
    if( picFive == NULL )
    {
        return 1;
    }
    if( picSix == NULL )
    {
        return 1;
    }
    if( picSeven == NULL )
    {
        return 1;
    }
    if( picEight == NULL )
    {
        return 1;
    }
    if( picNine == NULL )
    {
        return 1;
    }
    if( picZero == NULL )
    {
        return 1;
    }
    if( blueDot == NULL )
    {
        return 1;
    }
    if( blueDotBig == NULL )
    {
        return 1;
    }
    if( blueDotHuge == NULL )
    {
        return 1;
    }
    if( blueDotMassive == NULL )
    {
        return 1;
    }
    if( explosion == NULL )
    {
        return 1;
    }
    if( blackHole == NULL )
    {
        return 1;
    }
    if( shield == NULL )
    {
        return 1;
    }
    if( iceField == NULL )
    {
        return 1;
    }
    if( teleporterU == NULL )
    {
        return 1;
    }
    if( teleporterR == NULL )
    {
        return 1;
    }
    if( teleporterD == NULL )
    {
        return 1;
    }
    if( teleporterL == NULL )
    {
        return 1;
    }
    if( numPlayersScreen == NULL )
    {
        return 1;
    }
    if( onePlayer == NULL )
    {
        return 1;
    }
    if( outlineOnePlayer == NULL )
    {
        return 1;
    }
    if( twoPlayers == NULL )
    {
        return 1;
    }
    if( outlineTwoPlayers == NULL )
    {
        return 1;
    }
    if( ninjaStar == NULL )
    {
        return 1;
    }
    if( shot == NULL )
    {
        return 1;
    }
    if( evilDot == NULL )
    {
        return 1;
    }
    if( evilDotBig == NULL )
    {
        return 1;
    }
    if( evilDotHuge == NULL )
    {
        return 1;
    }
    if( evilDotMassive == NULL )
    {
        return 1;
    }
    if( barDot == NULL )
    {
        return 1;
    }
    if( teleportDot == NULL )
    {
        return 1;
    }
    if( ghostArrow == NULL )
    {
        return 1;
    }
    if( coop == NULL )
    {
        return 1;
    }
    if( outlineCoop == NULL )
    {
        return 1;
    }
    if( vs == NULL )
    {
        return 1;
    }
    if( outlineVs == NULL )
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
    SDL_FreeSurface( redDot );
    SDL_FreeSurface( redDotBig );
    SDL_FreeSurface( redDotHuge );
    SDL_FreeSurface( redDotMassive );
    SDL_FreeSurface( greyDot );
    SDL_FreeSurface( greyDotBig );
    SDL_FreeSurface( greyDotHuge );
    SDL_FreeSurface( greyDotMassive );
    SDL_FreeSurface( speedyPower );
    SDL_FreeSurface( pacmanPower );
    SDL_FreeSurface( freezePower );
    SDL_FreeSurface( nukePower );
    SDL_FreeSurface( vortexPower );
    SDL_FreeSurface( shieldPower );
    SDL_FreeSurface( teleportPower );
    SDL_FreeSurface( ninjaPower );
    SDL_FreeSurface( gravestonePower );
    SDL_FreeSurface( bulletPower );
    SDL_FreeSurface( loseScreen );
    SDL_FreeSurface( arrowPacman );
    SDL_FreeSurface( picOne );
    SDL_FreeSurface( picTwo );
    SDL_FreeSurface( picThree );
    SDL_FreeSurface( picFour );
    SDL_FreeSurface( picFive );
    SDL_FreeSurface( picSix );
    SDL_FreeSurface( picSeven );
    SDL_FreeSurface( picEight );
    SDL_FreeSurface( picNine );
    SDL_FreeSurface( picZero );
    SDL_FreeSurface( blueDot );
    SDL_FreeSurface( blueDotBig );
    SDL_FreeSurface( blueDotHuge );
    SDL_FreeSurface( blueDotMassive );
    SDL_FreeSurface( explosion );
    SDL_FreeSurface( blackHole );
    SDL_FreeSurface( shield );
    SDL_FreeSurface( iceField );
    SDL_FreeSurface( teleporterU );
    SDL_FreeSurface( teleporterR );
    SDL_FreeSurface( teleporterD );
    SDL_FreeSurface( teleporterL );
    SDL_FreeSurface( numPlayersScreen );
    SDL_FreeSurface( onePlayer );
    SDL_FreeSurface( outlineOnePlayer );
    SDL_FreeSurface( twoPlayers );
    SDL_FreeSurface( outlineTwoPlayers );
    SDL_FreeSurface( ninjaStar );
    SDL_FreeSurface( shot );
    SDL_FreeSurface( evilDot );
    SDL_FreeSurface( evilDotBig );
    SDL_FreeSurface( evilDotHuge );
    SDL_FreeSurface( evilDotMassive );
    SDL_FreeSurface( barDot );
    SDL_FreeSurface( teleportDot );
    SDL_FreeSurface( ghostArrow );
    SDL_FreeSurface( coop );
    SDL_FreeSurface( outlineCoop );
    SDL_FreeSurface( vs );
    SDL_FreeSurface( outlineVs );
    SDL_FreeSurface( roto );
    SDL_FreeSurface( ghostRoto );

    //Quit SDL
    SDL_Quit();
}

double distance( double x1, double y1, double x2, double y2 )//Distance Formula function
{
    //Return the distance between the two points
    return sqrt( pow( x2 - x1, 2 ) + pow( y2 - y1, 2 ) );
}

bool CircleCollision( double x1, double y1, double x2, double y2, double r1, double r2 )//collison between 2 circles
{
    //If the distance between the centers of the circles is less than the sum of their radii
    if( distance( x1, y1, x2, y2 ) < ( r1 + r2 ) )
    {
        //The circles have collided
        return true;
    }

    //If not
    return false;
}

void ScoreConversion(int score,double x,double y)
{
    int ary[4] = {0,0,0,0};

    int i=0;

    ary[3]=score%10;
    score=score/10;
    ary[2]=score%10;
    score=score/10;
    ary[1]=score%10;
    score=score/10;
    ary[0]=score%10;

    while (i!=4)
    {
        if(ary[i]==0)
            apply_surface(x+i*30,y,picZero,screen);
        else if(ary[i]==1)
            apply_surface(x+i*30,y,picOne,screen);
        else if(ary[i]==2)
            apply_surface(x+i*30,y,picTwo,screen);
        else if(ary[i]==3)
            apply_surface(x+i*30,y,picThree,screen);
        else if(ary[i]==4)
            apply_surface(x+i*30,y,picFour,screen);
        else if(ary[i]==5)
            apply_surface(x+i*30,y,picFive,screen);
        else if(ary[i]==6)
            apply_surface(x+i*30,y,picSix,screen);
        else if(ary[i]==7)
            apply_surface(x+i*30,y,picSeven,screen);
        else if(ary[i]==8)
            apply_surface(x+i*30,y,picEight,screen);
        else
            apply_surface(x+i*30,y,picNine,screen);
        i++;
    }
    return;
}

int PowerUpAssign()
{
    int randomnumber;

    randomnumber = rand() % 100 + 1; //1 is speed, 2 is pacman, 3 is shield, 4 is freeze, 5 is vortex, 6 is nuke, 7 is teleport, 8 is ninja, 9 is gravestone, 10 is bullet.
    if (randomnumber < 4)
        return 6;
    if (randomnumber >= 4 && randomnumber < 11)
        return 8;
    if (randomnumber >= 11 && randomnumber < 31)
        return 2;
    if (randomnumber >= 31 && randomnumber < 41)
        return 4;
    if (randomnumber >= 41 && randomnumber < 51)
        return 9;
    if (randomnumber >= 51 && randomnumber < 56)
        return 3;
    if (randomnumber >= 56 && randomnumber < 61)
        return 10;
    if (randomnumber >= 61 && randomnumber < 66)
        return 7;
    if (randomnumber >= 66 && randomnumber < 71)
        return 1;
    if (randomnumber >= 71)
        return 5;
}
double BoundaryCheckX(double x, int left, int right)
{
    if (x > right)
        return right;
    if (x < left)
        return left;
    return x;
}
double BoundaryCheckY(double y, int up, int down)
{

    if (y < up)
        return up;
    if (y > down)
        return down;
    return y;
}
