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
float HorizontalSensitivity = 1;
float VerticalSensitivity = 1; // Smoothed DEGREE shifts of 360 per Mouse dot scan, Or come up with a conversion rate that feels good.
bool Paused = false;
float prevMouseX = 0;
float prevMouseY = 0;
float CamLateralMax = 360;
float CamLateralMin = 0; // JUST the number of whole number segment divisions for camera movement, Still wraps around doesnt limit swivel mobility
float CamLongitudalMax = 180;
float CamLongitudalMin = 0; 

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
float x;
float y;
float z;
float LookVerticalAxis; // of 180 degress
float LookHorizontalAxis; // of 360 degrees
// Height is relative to your location, Duh.. not really a duh moment. 
}  EntityState; // maybe seperate the base essentials of the Look direction, From other state variables Like items and attachments, Even mobs will need this much
// Will need a ENUM and a fully dynamic setup for this later
EntityState GameMaster;


int EntityEmbodied = 0; // The entity ID for the entity whos perspective is being viewed
// -- PLAYER PHYSICS -- //

// -- WORLD PHYSICS -- //
float CollisionDampening = 0; // For Bouncy objects
// -- WORLD PHYSICS -- //

// ** GLOBAL PHYSICS VARIABLES ** //


//Probably need Enums for the rects


// ? ? ?  NETWORKING   ? ? ? //
// ? ? ?  NETWORKING   ? ? ? //













    typedef struct {
    char Text[512];
    char LastMessage[512];
    int Textint[512];
    int Size;
    int LastLetter;
    int Step;
    float StartingX;
    float StartingY;
    SDL_FRect srcrect[512];
    SDL_FRect destrect[512];
    } message;




message Coordinates;

void TypeTextToScreen (message *Message, int Size, float StartingX, float StartingY){

//if (GlobalPTick == 0){
Message->StartingY = StartingY;
Message->StartingX = StartingX;
Message->Size = Size; 
//}

if (Message->LastMessage != Message->Text){
   //printf(" Message: %s\n", Message->Text);
char CapitalLEtters[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ. 1234567890*!$%";
char LowerCaseLetters[]="abcdefghijklmnopqrstuvwxyz. 1234567890*!$%";

for (int i = 0; i < (int)strlen(Message->Text); i++){
char Letter = Message->Text[i];

for (int j = 0; j < strlen(CapitalLEtters); j++){
if (CapitalLEtters[j] == Letter) {
    Message->Textint[i] = j;
   // printf(" Val %d\n", j);
    break;}

if (LowerCaseLetters[j] == Letter) {
    Message->Textint[i] = j;
   // printf(" Val %d\n", j);
    break;}
}
}

strcpy(Message->LastMessage,Message->Text);
}

int FontHeight[] = {12, 18, 28, 48};
int LetterWidth[] = {10, 12, 20, 48};
int FontStartY[] = {0, 12, 30, 48};
int FontWidth[168] = {
10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,0,0,10,10,
10,10,10,12,11,9,12,12,12,10,10,10,10,11,10,11,11,10,11,11,10,12,10,10,10,4,7,7,10,9,10,9,9,10,9,9,10,10,0,0,10,10,
14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,18,18,18,18,18,18,18,18,18,18,14,14,14,18,18,0,0,10,10,
10,10,10,10,10,10,1,10,1,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,0,0,10,10};
for (int s = 0; s < strlen(Message->Text); s++)
{
               if (Message->Textint[Message->Step] == 39){
                 Message->StartingY = StartingY;
                 Message->StartingX = StartingX;
                 Message->Step = 0;
                 Message->LastLetter = 0;
                return;
               }

                Message->srcrect[Message->Step].y = FontStartY[Message->Size];
                Message->srcrect[Message->Step].x = LetterWidth[Message->Size] * Message->Textint[Message->Step];
                Message->srcrect[Message->Step].w = FontWidth[Message->Textint[Message->Step] + (Message->Size *42) ];
                Message->srcrect[Message->Step].h = FontHeight[Message->Size];

                Message->destrect[Message->Step].y = Message->StartingY;
                Message->destrect[Message->Step].x = Message->StartingX += Message->LastLetter;
                Message->destrect[Message->Step].w = FontWidth[Message->Textint[Message->Step] + (Message->Size *42) ];
                Message->destrect[Message->Step].h = FontHeight[Message->Size];
                //printf("Val %f\n",  Message->destrect.y);
                if (Message->Textint[Message->Step] == 38){
                Message->StartingY += FontHeight[Message->Size] +4;
                Message->StartingX = StartingX;
                Message->LastLetter = 0;
                }

                if (Message->Textint[Message->Step] != 38){
                
                 Message->LastLetter = FontWidth[Message->Textint[Message->Step] + (Message->Size *42)];
                }
Message->Step++;
}
}
















int main(int argc, char* argv[]) {

SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO); 
SDL_Window* window = SDL_CreateWindow("Patch of Eden", ScreenWidth, ScreenHeight, 0);
SDL_Renderer* Firstrenderer = SDL_CreateRenderer(window, NULL);


// INITIALIZATION STAGE //

// ASSETS //
SDL_Surface *Text = SDL_LoadBMP("images/Text.bmp");
SDL_Texture *TextTexture = SDL_CreateTextureFromSurface(Firstrenderer, Text);
// ASSETS //

ScreenCenterX = ScreenWidth / 2;
ScreenCenterY = ScreenHeight / 2;
FreeCam.x = ScreenCenterX;
FreeCam.y = ScreenCenterY;
FreeCam.w = 10;
FreeCam.h = 10;  
EntityState* Entities[1] = {&GameMaster}; // maybe entity types? We will see.

for (int i = 0; i < sizeof(Entities) / sizeof(Entities[0]); i++)
{
    
    EntityState *EntityINIT = Entities[i];

    EntityINIT->x = 0;
    EntityINIT->y = 0;
    EntityINIT->z = 0;
    EntityINIT->LookHorizontalAxis = 0;
    EntityINIT->LookVerticalAxis = 0;
}
// INITIALIZATION STAGE //



    /// ## ///   AUDIO   /// ## ///


    /// ## ///   AUDIO   /// ## ///

bool quit = false;//, buttonClicked = false, switchWindow = false;
    SDL_Event e;
    while (!quit) {
   EntityState *Perspective = Entities[EntityEmbodied];      
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT){
                    quit = true;
            }
              float x = e.button.x, y = e.button.y;
             
             if (e.type == SDL_EVENT_MOUSE_MOTION && !Paused) // And if mouse is Focused IN RENDERER and Game is not Paused
             {
                if (prevMouseX > x)
                {   Perspective->LookHorizontalAxis -= (prevMouseX - x) * HorizontalSensitivity; // Might need a Motion Smoothing Function
                    if (Perspective->LookHorizontalAxis < CamLateralMin) { Perspective->LookHorizontalAxis += CamLateralMax; }
                    
                    prevMouseX = x;
                }
                if (prevMouseX < x)
                {   Perspective->LookHorizontalAxis += (x - prevMouseX) * HorizontalSensitivity; // Might need a Motion Smoothing Function
                    if (Perspective->LookHorizontalAxis > CamLateralMax) { Perspective->LookHorizontalAxis -= CamLateralMax; }
                    prevMouseX = x;
                }

                if (prevMouseY > y)
                {   Perspective->LookHorizontalAxis -= (prevMouseY - y) * VerticalSensitivity; // Might need a Motion Smoothing Function
                    if (Perspective->LookHorizontalAxis < CamLongitudalMin) { Perspective->LookHorizontalAxis = CamLongitudalMin; }
                    
                    prevMouseY = y;
                }
                if (prevMouseY < y)
                {   Perspective->LookVerticalAxis += (y - prevMouseY) * VerticalSensitivity; // Might need a Motion Smoothing Function
                    if (Perspective->LookVerticalAxis > CamLongitudalMax) { Perspective->LookVerticalAxis = CamLongitudalMax; } // if More stop, dont wrap back around, its nauseating
                    prevMouseY = y;
                }
              
                FreeCam.x = x; // Replaced by entityState struct, basic Camera Pan Flight style movement calc
                FreeCam.y = y; // Shows where you are looking tho with the dot, so its good to stay for a bit, REALLY cant leave too many "I need to get back to this style nonsense" or im cooked
             }
             

            else if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
               
             
                }

                else if (e.type == SDL_EVENT_KEY_DOWN){
                        
                             
                            
                            for (int k = 0; k < (int)CameraPanSpeed; k++)
                            {
                        switch (e.key.key) {    
                        case SDLK_UP:
                        case SDLK_W:
                        CameraPanSpeedUp = true;
                       // if ( Perspective->y > 0){
                            Perspective->y--; // watching through the eyes of AI and other players will be a feature
                        printf("Walk Up, Cam Speed %f\n",CameraPanSpeed);//} // Will need a control mode shift tree for different player states, game master, pLAYER, ENtity eyes aka no controls at all.
                        break;

                        case SDLK_S :
                        case SDLK_DOWN: // still have to register the press For audio feedback if its at the border
                        CameraPanSpeedUp = true;
                       // if ( Perspective->y < ScreenHeight - 10){
                            Perspective->y++;
                        printf("Walk Down, Cam Speed %f\n",CameraPanSpeed);//}
                        break;

                        case SDLK_A:
                        case SDLK_LEFT:
                        CameraPanSpeedUp = true;
                       // if ( Perspective->x > 0){
                            Perspective->x--;
                         printf("Walk Left, Cam Speed %f\n",CameraPanSpeed);//} // Now it can only move 1 pixel at a time to not go out of bounds, any more and it will,
                        break;

                        case SDLK_D:
                        case SDLK_RIGHT:
                        CameraPanSpeedUp = true;
                        //if ( Perspective->x < ScreenWidth -10){
                            Perspective->x++;
                        printf("Walk Right, Cam Speed %f\n",CameraPanSpeed);//}
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




char Cbuffer[512];
char Coordbuffer[512] = "Patch Of Eden*";
snprintf(Cbuffer, sizeof(Cbuffer), "X %f*Y %f*Z %f*Horizantal Facing Degree %f*Vertical Facing Degree %f*Gabriel Wynn 2026",Perspective->x, Perspective->y, Perspective->z, Perspective->LookHorizontalAxis, Perspective->LookVerticalAxis); 
strcat(Coordbuffer, Cbuffer);
strcat(Coordbuffer, "!");
strcpy(Coordinates.Text, Coordbuffer); 
TypeTextToScreen(&Coordinates, 1, 50, 30);

for (int d = 0; d < strlen(Coordinates.Text)  -1; d++)
{
  SDL_RenderTexture(Firstrenderer, TextTexture, &Coordinates.srcrect[d], &Coordinates.destrect[d]);
}



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




   
    
