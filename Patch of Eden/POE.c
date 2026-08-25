#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>   
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>

// DEFAULT DEFINEMENT //

// ** Controls ** //
float CameraPanSpeed = 1;
bool CameraPanSpeedUp = false;
float CameraPanSpeedUpRate = .1;
float CameraPanMaxSpeed = 20; // Pan Will be changed to flight later, Once the movement actually resembles such
//NEXT stick the dot to the mouse point and SHIFT the Current X, Y and Z coords instead of X and Y on the Screen, Calculative effect
// dot goes where mouse goes
// ** Controls ** //

// ** GLOBAL PHYSICS VARIABLES ** //
float ScreenWidth = 812;
float ScreenHeight = 512;
float ScreenCenterX = 0;// ScreenWidth / 2;
float ScreenCenterY = 0;
SDL_FRect FreeCam = {0,0,10,10}; // Temporary Placeholder for testing camera Movement controls, Will WASD, 
//Look to fly in direction with W, S reverse, Left and right are PAN, Mousepoint to turn,

// -- PLAYER PHYSICS -- //
typedef struct {
int x;
int y;
int z;
// Height is relative to your location, Duh.. not really a duh moment. 
} * GameMaster;
// -- PLAYER PHYSICS -- //

// -- WORLD PHYSICS -- //
float CollisionDampening = 0; // For Bouncy objects
// -- WORLD PHYSICS -- //

// ** GLOBAL PHYSICS VARIABLES ** //

//Probably need Enums for the rects


// ? ? ?  NETWORKING   ? ? ? //
// ? ? ?  NETWORKING   ? ? ? //


int main(int argc, char* argv[]) {
SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); 
// INITIALIZATION STAGE //
ScreenCenterX = ScreenWidth / 2;
ScreenCenterY = ScreenHeight / 2;
FreeCam.x = ScreenCenterX;
FreeCam.y = ScreenCenterY;
FreeCam.w = 10;
FreeCam.h = 10;  

// INITIALIZATION STAGE //
    SDL_Window* window = SDL_CreateWindow("Patch of Eden", ScreenWidth, ScreenHeight, 0);
    SDL_Renderer* Firstrenderer = SDL_CreateRenderer(window, NULL);

    /// ## ///   AUDIO   /// ## ///


    /// ## ///   AUDIO   /// ## ///

bool quit = false;//, buttonClicked = false, switchWindow = false;
    SDL_Event e;
    while (!quit) {
        
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT){
                    quit = true;
            }

             

            else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
                int x = e.button.x, y = e.button.y;
             
                }
                else if (e.type == SDL_EVENT_KEY_DOWN){
                        
                            
                            
                            for (int k = 0; k < (int)CameraPanSpeed; k++)
                            {
                        switch (e.key.key) {    
                        case SDLK_UP:
                        case SDLK_W:
                        CameraPanSpeedUp = true;
                        if ( FreeCam.y > 0){FreeCam.y--;
                        printf("Pan Up, Cam Speed %f\n",CameraPanSpeed);} // Will need a control mode shift tree for different player states, game master, pLAYER, ENtity eyes aka no controls at all.
                        break;

                        case SDLK_S :
                        case SDLK_DOWN: // still have to register the press For audio feedback if its at the border
                        CameraPanSpeedUp = true;
                        if ( FreeCam.y < ScreenHeight - 10){FreeCam.y++;
                        printf("Pan Down, Cam Speed %f\n",CameraPanSpeed);}
                        break;

                        case SDLK_A:
                        case SDLK_LEFT:
                        CameraPanSpeedUp = true;
                        if ( FreeCam.x > 0){FreeCam.x--;
                         printf("Pan Left, Cam Speed %f\n",CameraPanSpeed);} // Now it can only move 1 pixel at a time to not go out of bounds, any more and it will,
                        break;

                        case SDLK_D:
                        case SDLK_RIGHT:
                        CameraPanSpeedUp = true;
                        if ( FreeCam.x < ScreenWidth -10){FreeCam.x++;
                        printf("Pan Right, Cam Speed %f\n",CameraPanSpeed);}
                        break;
                        }
                           
                    }
                }

                if (e.type == SDL_EVENT_KEY_UP){
                    CameraPanSpeedUp = false;
                   CameraPanSpeed = 1;
                }
            }
if (CameraPanSpeedUp == true && CameraPanSpeed < CameraPanMaxSpeed) { CameraPanSpeed+= CameraPanSpeedUpRate; }        



SDL_SetRenderDrawBlendMode(Firstrenderer, SDL_BLENDMODE_BLEND);
SDL_SetRenderDrawColor(Firstrenderer,255,180,180,255);
        SDL_RenderFillRect(Firstrenderer,&FreeCam);
         SDL_RenderPresent(Firstrenderer);
   
 SDL_Delay(16); // 16 is about 60 fps
SDL_SetRenderDrawColor(Firstrenderer, 0, 0, 0, 255);
SDL_RenderClear(Firstrenderer);
        }


    SDL_DestroyRenderer(Firstrenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

       
    }




   
    