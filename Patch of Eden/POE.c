#include <SDL3/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>   
#include <string.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <math.h> // temporary will be replaced, The math will be ALL be
// DEFAULT DEFINEMENT //



// ** Controls ** //
float CameraPanSpeed = 1;
bool CameraPanSpeedUp = false;
float CameraPanSpeedUpRate = .1;
float CameraPanMaxSpeed = 20; // Pan Will be changed to flight later, Once the movement actually resembles such
float HorizontalSensitivity = .3;
float VerticalSensitivity = .3; // Smoothed DEGREE shifts of 360 per Mouse dot scan, Or come up with a conversion rate that feels good.
float MovementSpeed = .06;
bool Paused = false;
bool InvertLookHorizontal = false;
bool InvertLookVertical = false;
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
float ScreenWidth = 1280;
float ScreenHeight = 720;
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
float AngleOfApproach = 0;

int EntityEmbodied = 0; // The entity ID for the entity whos perspective is being viewed
// -- PLAYER PHYSICS -- //

// -- WORLD PHYSICS -- //
float CollisionDampening = 0; // For Bouncy objects
// -- WORLD PHYSICS -- //

// ** GLOBAL PHYSICS VARIABLES ** //


//Probably need Enums for the rects


// ? ? ?  NETWORKING   ? ? ? //
// ? ? ?  NETWORKING   ? ? ? //








float Cosf(float x)
{
    float xx = x * x;

    float result = 1.0f;

    float term = 1.0f;

    term = term * xx / 2.0f;
    result = result - term;

    term = term * xx / 12.0f;
    result = result + term;

    term = term * xx / 30.0f;
    result = result - term;

    term = term * xx / 56.0f;
    result = result + term;

    term = term * xx / 90.0f;
    result = result - term;

    term = term * xx / 132.0f;
    result = result + term;

    term = term * xx / 182.0f;
    result = result - term;

    term = term * xx / 240.0f;
    result = result + term;

    term = term * xx / 306.0f;
    result = result - term;

    return result;
}


float Sinf(float x)
{
    float xx = x * x;

    float result = x;
    float term = x;

    term = term * xx / 6.0f;
    result = result - term;

    term = term * xx / 20.0f;
    result = result + term;

    term = term * xx / 42.0f;
    result = result - term;

    term = term * xx / 72.0f;
    result = result + term;

    term = term * xx / 110.0f;
    result = result - term;

    term = term * xx / 156.0f;
    result = result + term;

    term = term * xx / 210.0f;
    result = result - term;

    term = term * xx / 272.0f;
    result = result + term;

    term = term * xx / 342.0f;
    result = result - term;

    return result;
}

void DrawCircle(SDL_Renderer *renderer, float cx, float cy, float radius)
{
    const int segments = 30; //50 makes it round // 4 makes it a square // too high  a numbe makes it cap out at a unknown sum of maxusefulness
       
    for (int i = 0; i < segments; i++)
    {
        float a1 = (2.0f * 3.14 * i) / segments;
        float a2 = (2.0f * 3.14 * (i + 1)) / segments;

        float x1 = cx + Cosf(a1) * radius;
        float y1 = cy + Sinf(a1) * radius;

        float x2 = cx + Cosf(a2) * radius;
        float y2 = cy + Sinf(a2) * radius;

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
}

void DrawGrid(SDL_Renderer *renderer, float x, float y, float Width, int GridDivisions)
{
   GridDivisions += 1;

      float x1 = x - Width;
      float y1 = y - Width;
       float x2 = x1;
        float y2 = y + Width;
      SDL_RenderLine(renderer, x1, y1, x2, y2);

        x1 = x - Width;
     y1 = y - Width;
        x2 = x + Width;
        y2 = y1;
      SDL_RenderLine(renderer, x1, y1, x2, y2);

    x1 = x + Width;
      y1 = y + Width;
        x2 = x - Width;
        y2 = y1;
     SDL_RenderLine(renderer, x1, y1, x2, y2);

       x1 = x + Width;
      y1 = y - Width;
        x2 = x + Width;
        y2 = y + Width;
     SDL_RenderLine(renderer, x1, y1, x2, y2);


 for (int i = 1; i < GridDivisions; i++)
    {

        x1 = x - Width;// + (((Width * 2) / Divisions)* i);
        y1 = y- Width + (((Width * 2) / GridDivisions)* i);

        x2 = x + Width; //+ (((Width * 2) / GridDivisions)* i);
        y2 = y - Width + (((Width * 2) / GridDivisions)* i);

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    

        x1 = x - Width + (((Width * 2) /  GridDivisions)* i);
        y1 = y- Width; //+ (((Width * 2) / GridDivisions)* i);

        x2 = x - Width + (((Width * 2) / GridDivisions)* i);
        y2 = y + Width; //+ (((Width * 2) / GridDivisions)* i);

        SDL_RenderLine(renderer, x1, y1, x2, y2);
    
    
    }


}


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

                if (prevMouseY < y)
                {   Perspective->LookVerticalAxis -= ((y - prevMouseY) * VerticalSensitivity); // Might need a Motion Smoothing Function
                   if (Perspective->LookVerticalAxis < CamLongitudalMin) { Perspective->LookVerticalAxis = CamLongitudalMin; } // if More stop, dont wrap back around, its nauseating
                    prevMouseY = y;
                }
                if (prevMouseY > y)
                {   Perspective->LookVerticalAxis += (prevMouseY - y) * VerticalSensitivity; // Might need a Motion Smoothing Function
                    
                     if (Perspective->LookVerticalAxis > CamLongitudalMax) { Perspective->LookVerticalAxis = CamLongitudalMax;}
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
                        case SDLK_UP: // 8 is not a magic Number its a standar to diagnolly split this circle into diagonal quarters
                        case SDLK_W: // This is likely NEVER to be anything but 360 degrees, And the magic number method is MORE EFFICIENT, Clean Code != Efficient Code, But if my employer request They will recieve.
                        CameraPanSpeedUp = true; 
                            if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 7  || Perspective->LookHorizontalAxis < CamLateralMax /8) { printf("North  %f %f\n",Perspective->y,CameraPanSpeed); Perspective->y+=MovementSpeed; }
                       else if ( Perspective->LookHorizontalAxis >= CamLateralMax /8  && Perspective->LookHorizontalAxis < (CamLateralMax /8) * 3 ) {    AngleOfApproach = (Perspective->LookHorizontalAxis - (CamLateralMax /8)) * (100/((CamLateralMax /8) * 2)); if(AngleOfApproach >= 50){ AngleOfApproach = (AngleOfApproach - 50)  / 100; Perspective->x+=(MovementSpeed * (1 - AngleOfApproach)); Perspective->y-=(MovementSpeed *  AngleOfApproach); } else{ AngleOfApproach = (50 - AngleOfApproach) / 100; Perspective->x+=(MovementSpeed * (1 - AngleOfApproach)); Perspective->y+=(MovementSpeed * AngleOfApproach);  } printf("East  %f X %f %f\n",AngleOfApproach,Perspective->x,CameraPanSpeed);  }
                       else if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 3  && Perspective->LookHorizontalAxis < (CamLateralMax /8) * 5 ) { AngleOfApproach = (Perspective->LookHorizontalAxis - ((CamLateralMax /8)* 3) ) * (100 /((CamLateralMax /8) * 2)); if(AngleOfApproach >= 50){ AngleOfApproach = (AngleOfApproach - 50)  / 100; Perspective->y-=(MovementSpeed * (1 - AngleOfApproach)); Perspective->x-=(MovementSpeed * AngleOfApproach); } else{ AngleOfApproach = (50 - AngleOfApproach) / 100; Perspective->y-=(MovementSpeed * (1 - AngleOfApproach)); Perspective->x+=(MovementSpeed * AngleOfApproach);  } printf("South  %f  Y %f %f\n",AngleOfApproach,Perspective->y,CameraPanSpeed);}// watching through the eyes of AI and other players will be a feature
                       else if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 5 && Perspective->LookHorizontalAxis < (CamLateralMax /8) * 7 ) { AngleOfApproach = (Perspective->LookHorizontalAxis - ((CamLateralMax /8)* 5) ) * (100/((CamLateralMax /8) * 2)); if(AngleOfApproach >= 50){ AngleOfApproach = (AngleOfApproach - 50) / 100; Perspective->x-=(MovementSpeed * (1 - AngleOfApproach)); Perspective->y+=(MovementSpeed * AngleOfApproach); } else{ AngleOfApproach = (50 - AngleOfApproach) / 100; Perspective->x-=(MovementSpeed * (1 - AngleOfApproach)); Perspective->y-=(MovementSpeed *  AngleOfApproach);  } printf("West %f X %f %f\n",AngleOfApproach,Perspective->x,CameraPanSpeed); } // Increase or decrease X and Y by fractions of a whole number,
    
                        break;

                        case SDLK_S :
                        case SDLK_DOWN: // still have to register the press For audio feedback if its at the border
                        CameraPanSpeedUp = true;
                        if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 7  || Perspective->LookHorizontalAxis < (CamLateralMax /8) * 1 ) { printf("North  %f %f\n",Perspective->y,CameraPanSpeed); Perspective->y-=MovementSpeed; }
                       else if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 1  && Perspective->LookHorizontalAxis < (CamLateralMax /8) * 3 ) { printf("East  %f %f\n",Perspective->x,CameraPanSpeed); Perspective->x-=MovementSpeed; }
                       else if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 3  && Perspective->LookHorizontalAxis < (CamLateralMax /8) * 5 ) { printf("South  %f %f\n",Perspective->y,CameraPanSpeed); Perspective->y+=MovementSpeed; }// watching through the eyes of AI and other players will be a feature
                       else if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 5 && Perspective->LookHorizontalAxis < (CamLateralMax /8) * 7 ) { printf("West  %f %f\n",Perspective->x,CameraPanSpeed) ; Perspective->x+=MovementSpeed; } // Increase or decrease X and Y by fractions of a whole number,
    
                        break;

                        case SDLK_A:
                        case SDLK_LEFT:
                        CameraPanSpeedUp = true;
                             if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 7  || Perspective->LookHorizontalAxis < (CamLateralMax /8) * 1 ) { printf("North  %f %f\n",Perspective->y,CameraPanSpeed); Perspective->x-=MovementSpeed; }
                       else if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 1  && Perspective->LookHorizontalAxis < (CamLateralMax /8) * 3 ) { printf("East  %f %f\n",Perspective->x,CameraPanSpeed); Perspective->y+=MovementSpeed; }
                       else if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 3  && Perspective->LookHorizontalAxis < (CamLateralMax /8) * 5 ) { printf("South  %f %f\n",Perspective->y,CameraPanSpeed); Perspective->x+=MovementSpeed; }// watching through the eyes of AI and other players will be a feature
                       else if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 5 && Perspective->LookHorizontalAxis < (CamLateralMax /8) * 7 ) { printf("West  %f %f\n",Perspective->x,CameraPanSpeed) ; Perspective->y-=MovementSpeed; } // Increase or decrease X and Y by fractions of a whole number,
    
                        break;

                        case SDLK_D:
                        case SDLK_RIGHT:
                        CameraPanSpeedUp = true;
                  if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 7  || Perspective->LookHorizontalAxis < (CamLateralMax /8) * 1 ) { printf("North  %f %f\n",Perspective->y,CameraPanSpeed); Perspective->x+=MovementSpeed; }
                       else if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 1  && Perspective->LookHorizontalAxis < (CamLateralMax /8) * 3 ) { printf("East  %f %f\n",Perspective->x,CameraPanSpeed); Perspective->y-=MovementSpeed; }
                       else if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 3  && Perspective->LookHorizontalAxis < (CamLateralMax /8) * 5 ) { printf("South  %f %f\n",Perspective->y,CameraPanSpeed); Perspective->x-=MovementSpeed; }// watching through the eyes of AI and other players will be a feature
                       else if ( Perspective->LookHorizontalAxis >= (CamLateralMax /8) * 5 && Perspective->LookHorizontalAxis < (CamLateralMax /8) * 7 ) { printf("West  %f %f\n",Perspective->x,CameraPanSpeed) ; Perspective->y+=MovementSpeed; } // Increase or decrease X and Y by fractions of a whole number,
    
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
snprintf(Cbuffer, sizeof(Cbuffer), "X %.3f*Y % .3f*Z %.3f*Horizantal Facing Degree %.3f*Vertical Facing Degree %.3f*Gabriel Wynn 2026",Perspective->x, Perspective->y, Perspective->z, Perspective->LookHorizontalAxis, Perspective->LookVerticalAxis); 
strcat(Coordbuffer, Cbuffer);
strcat(Coordbuffer, "!");
strcpy(Coordinates.Text, Coordbuffer); 
TypeTextToScreen(&Coordinates, 1, 10, 10);

for (int d = 0; d < strlen(Coordinates.Text)  -1; d++)
{
  SDL_RenderTexture(Firstrenderer, TextTexture, &Coordinates.srcrect[d], &Coordinates.destrect[d]);
}



SDL_SetRenderDrawBlendMode(Firstrenderer, SDL_BLENDMODE_BLEND);
SDL_SetRenderDrawColor(Firstrenderer,255,180,180,255);
        SDL_RenderFillRect(Firstrenderer,&FreeCam);


SDL_SetRenderDrawColor(Firstrenderer, 255, 255, 255, 255);

        DrawCircle(Firstrenderer, ScreenCenterX, ScreenCenterY, 100);
          DrawGrid(Firstrenderer, ScreenCenterX, ScreenCenterY, 100, 6); // Even numbers only
         SDL_RenderPresent(Firstrenderer);
   



 SDL_Delay(16); // 16 is about 60 fps
SDL_SetRenderDrawColor(Firstrenderer, 0, 0, 0, 255);
SDL_RenderClear(Firstrenderer);
        }


    SDL_DestroyRenderer(Firstrenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

       
    }




   
    
