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
#define GPointDensity 1000


// ** Controls ** //
float CameraPanSpeed = 1;
bool CameraPanSpeedUp = false;
float CameraPanSpeedUpRate = .1;
float CameraPanMaxSpeed = 20; // Pan Will be changed to flight later, Once the movement actually resembles such
float HorizontalSensitivity = .5;
float VerticalSensitivity = .5; // Smoothed DEGREE shifts of 360 per Mouse dot scan, Or come up with a conversion rate that feels good.
float MovementSpeed = .06;
bool Paused = false;
bool InvertLookHorizontal = false;
bool InvertLookVertical = false;
float prevMouseX = 0;
float prevMouseY = 0;
float CamLateralMax = 360;
float CamLateralMin = 0; // JUST the number of whole number segment GridDensity for camera movement, Still wraps around doesnt limit swivel mobility
float CamLongitudalMax = 180;
float CamLongitudalMin = 0; 
float TEMPGLOBALHORIZONTALLOOKDEGREE = 0;
float TEMPGLOBALVERTICALLOOKDEGREE = 0;
//NEXT stick the dot to the mouse point and SHIFT the Current X, Y and Z coords instead of X and Y on the Screen, Calculative effect
// dot goes where mouse goes
// ** Controls ** //

// ** GLOBAL PHYSICS VARIABLES ** //
float ScreenWidth = 1280;
float ScreenHeight = 720;
float ScreenCenterX = 0;// ScreenWidth / 2;
float ScreenCenterY = 0;
int ChunkBuffer = 0;
float FOV = 1; // Likely only need one, Not sure, 
float ZFOV = 1; // Likely only need one, Not sure, 
float StandardFOVinc = 0;// ONLY CHANGES when renderdistance does // Standard Increment per row for FOV, Leaves half a squares worth over at the end for the other half of the square given we gen from the squars center, and the center of their cneters, 
float Pi = 3.14159265359;
float HorizonLine = 500;
float HorizonLineFactorial = .8;
int PointOfConvergenceSize = 3; // visual size of the center dot the 3d map for the sky and Land eminates from (no calculative effect, purely visual)
float StandardRenderDistance = 32;
SDL_FRect Pointer = {0,0,10,10}; // Temporary Placeholder for testing camera Movement controls, Will WASD, 
//Look to fly in direction with W, S reverse, Left and right are PAN, Mousepoint to turn,

// -- PLAYER PHYSICS -- //
typedef struct {
float x;
float y;
float z;
float LookVerticalAxis; // of 180 degress
float LookHorizontalAxis; // of 360 degrees
float EyeViewHeight[3];
float PlayerHeight;
float CameraFocalLength; // THe GridAxis is only 90 degrees at a specific camera focal length, Find that out, Ai cant be trusted.
// Body animation / Pose Info here too in a seperate struct brought here, a array of diff types of bodies, 
// Height is relative to your location, Duh.. not really a duh moment. 
}  EntityState; // maybe seperate the base essentials of the Look direction, From other state variables Like items and attachments, Even mobs will need this much
// Will need a ENUM and a fully dynamic setup for this later
EntityState GameMaster;
float AngleOfApproach = 0;

int EntityEmbodied = 0; // The entity ID for the entity whos perspective is being viewed
// -- PLAYER PHYSICS -- //

// -- WORLD PHYSICS -- //
float CollisionDampening = 0; // For Bouncy objects
SDL_FRect SkyConvergencePoint = {0,0,0,0};
SDL_FRect LandConvergencePoint = {0,0,0,0};

typedef struct {
float TrueX;
float TrueY;
int Row;
int Placement;
bool Onscreen;
int MaxInnerX;
int MaxInnerY; // Initialize all these to zero, darn it, this will have to be updated constantly
// Height is relative to your location, Duh.. not really a duh moment. 
}  GenesisPoint;

typedef struct {
GenesisPoint GenPoints[GPointDensity];
}  WorldSquare;
// -- WORLD PHYSICS -- //

// ** GLOBAL PHYSICS VARIABLES ** //


//Probably need Enums for the rects


// ? ? ?  NETWORKING   ? ? ? //
// ? ? ?  NETWORKING   ? ? ? //

// ? ? ?  NETWORKING   ? ? ? //
// # # #  BATTLEFIELD PROCESS   # # # //


//with a standardize tick rate, and a near turn based processing system, determine how off screen battles take place ETC Out of render distance  SNipe shots etc
//Render a small circle of that is seen through the scope from far far away, Just the players True movement, Cut everything else
// Or enemy combatants who arent being rendered, say one revives the other behind your back, Even if you cant see em they are real
// NPC can do COVER FIRE war cry, Sends a Signal to a Public ledge of all Mobs of that type in the area, A shared Single Variable
// I should be able to name Chunks, And render them We have a near to the player constant Render buffer, And a Ghost Memory method Planned FOr when New chunks are to be loaded and the old
// dropped off. Create a function that can dynamically expand the render limit, But in one particular direction, to save memory


// # # #  BATTLEFIELD PROCESS   # # # //


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



void DrawRotatedSquare(
    SDL_Renderer *renderer,
    float x,
    float y,
    float size,
    float angle
)
{
    float radians = angle * Pi / 180.0f;

    float half = size / 2.0f;

    float px[4] = {
        -half,
         half,
         half,
        -half
    };

    float py[4] = {
        -half,
        -half,
         half,
         half
    };

    for (int i = 0; i < 4; i++)
    {
        int next = (i + 1) % 4;

        float x1 = px[i] * Cosf(radians) - py[i] * Sinf(radians);
        float y1 = px[i] * Sinf(radians) + py[i] * Cosf(radians);

        float x2 = px[next] * Cosf(radians) - py[next] * Sinf(radians);
        float y2 = px[next] * Sinf(radians) + py[next] * Cosf(radians);

        SDL_RenderLine(
            renderer,
            x + x1,
            y + y1,
            x + x2,
            y + y2
        );
    }
}


void DrawSquare(SDL_Renderer *renderer, float x1, float y1,float x2, float y2, float size,int side) {
    // Direction of the starting line
    float dx = x2 - x1;
    float dy = y2 - y1;

    // Length of the starting line
    float length = sqrtf(dx * dx + dy * dy);

    // Normalize direction
    dx /= length;
    dy /= length;

    // Perpendicular vector
    float px = -dy;
    float py = dx;

    // Choose left or right
    if (side == 1)
    {
        px = -px;
        py = -py;
    }

    // Move perpendicular by the square's side length
    px *= size;
    py *= size;

    // The other two corners
    float x3 = x2 + px;
    float y3 = y2 + py;

    float x4 = x1 + px;
    float y4 = y1 + py;

    // Draw all four sides
    SDL_RenderLine(renderer, x1, y1, x2, y2);
    SDL_RenderLine(renderer, x2, y2, x3, y3);
    SDL_RenderLine(renderer, x3, y3, x4, y4);
    SDL_RenderLine(renderer, x4, y4, x1, y1);

    // Ai  is dog shite The lines wont be askew, Must be Modified to account for FOV
}


void DrawCone(SDL_Renderer *renderer, float x, float y, float size)
{
    float halfWidth = size / 2.0f;

    float apexX = x,              apexY = y;
    float baseLeftX = x - halfWidth,  baseLeftY = y + size;
    float baseRightX = x + halfWidth, baseRightY = y + size;

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderLine(renderer, apexX, apexY, baseLeftX, baseLeftY);
    SDL_RenderLine(renderer, apexX, apexY, baseRightX, baseRightY);
    SDL_RenderLine(renderer, baseLeftX, baseLeftY, baseRightX, baseRightY);
}

void DrawRotatingCross(SDL_Renderer *renderer,int centerX, int centerY,int armLength,float degrees) {
    float rad1 = degrees * (Pi / 180.0);
    float rad2 = rad1 + (Pi / 2.0); /* perpendicular line */
 
    float dx1 = cos(rad1);
    float dy1 = sin(rad1);
    float dx2 = cos(rad2);
    float dy2 = sin(rad2);
 
    int x1a = (int)lround(centerX - dx1 * armLength);
    int y1a = (int)lround(centerY - dy1 * armLength);
    int x1b = (int)lround(centerX + dx1 * armLength);
    int y1b = (int)lround(centerY + dy1 * armLength);
 
    int x2a = (int)lround(centerX - dx2 * armLength);
    int y2a = (int)lround(centerY - dy2 * armLength);
    int x2b = (int)lround(centerX + dx2 * armLength);
    int y2b = (int)lround(centerY + dy2 * armLength);
 
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); /* teal, matches earlier demos */
    SDL_RenderLine(renderer, x1a, y1a, x1b, y1b);
    SDL_RenderLine(renderer, x2a, y2a, x2b, y2b);
}




float GenesisPointFOVcalc(GenesisPoint Genisis, float x)

{  // pre calc square FOV using the static SPiral ssytem you wrote out, Adjusted for Angle Ignore if not renderable as per bool, If project coords exist outside the window
    // Then inner from the TRUE available pixels within that square,
    float GPointRelativity = 0;
      float innerGPointRelativity = 0; // Merely goes into the first val, This is a placeholder, to remind me, is ALSO the sizing of any objects
      // rendered at that location.
      int TrueX, TrueY; // the TRUE on screen pixel that ths best fit for the starting point of this objects rendering
    return GPointRelativity, TrueX, TrueY; // Relative FOV of the square itself // T
}
// INSIDE the square, Coords From the center of the square
// Will most likely Call the first function and pass that Into this, But what if i only want the... I never would  
// direction codes 1234 or positive cardinal, 5678  for negative cardinal, switch statement, OR see 
// if C processes the negative number properly when the time comes








void DrawEarthNSky(SDL_Renderer *renderer) {
StandardFOVinc = ((FOV / (StandardRenderDistance * 2)) * ((StandardRenderDistance * 2) -1) / StandardRenderDistance); // DOES EXACTLY WHAT WE NEED IT TO, even distibution, .5 left over,  wont break
// ObjFOV = FOV / (RenderDistance + .5)  =/- the area within the square. the space your on being a whole 1, or 99.999 etc,
// Down a static amount each row,  Then a division of that difference from one row the the next divided amoung the row number AKA # of squares on a side / 2
// and it gets reverse partially, X + is - FOV, and so is y- because of it being closer to the player
// ACTUALLY divide  FOV by renderdistance + .5 So we get that added buff on that last block as the Coord starts from the center of the block, 
// Simplifies many other things complicates little things like this
SDL_SetRenderDrawColor(renderer, 111, 111, 222, 222);
     float GroundCenterX = ScreenCenterX;
     float GroundCenterY =  ScreenHeight / CamLongitudalMax;

     HorizonLine  = (ScreenHeight / CamLongitudalMax) * TEMPGLOBALVERTICALLOOKDEGREE;
    float x1 = 0;
    float y1 =  HorizonLine;

    float x2 = ScreenWidth; // may change if we add the ROLL AXIS
    float y2 =  HorizonLine; // Invert Cam Function would go here
    if (InvertLookVertical == true)
    {
      y1 = ScreenHeight - HorizonLine;
      y2 =ScreenHeight -  HorizonLine;
    }
    
    SDL_RenderLine(renderer, x1, y1, x2, y2);

SkyConvergencePoint.w = PointOfConvergenceSize;
SkyConvergencePoint.h = PointOfConvergenceSize;  
SkyConvergencePoint.x = ScreenCenterX-(SkyConvergencePoint.w /2);
SkyConvergencePoint.y = (HorizonLine -(ScreenHeight/2))  -(SkyConvergencePoint.h /2);

LandConvergencePoint.w = PointOfConvergenceSize;
LandConvergencePoint.h = PointOfConvergenceSize;
LandConvergencePoint.x = ScreenCenterX-(LandConvergencePoint.w /2);
LandConvergencePoint.y = ((ScreenHeight /2 ) + HorizonLine) -(LandConvergencePoint.h /2);


//LandConvergencePoint = {ScreenCenterX-1, ((ScreenHeight - HorizonLine) /2) -1, 3, 3 };


}
 


/* size (in pixels) of the little filled square used to mark each
   center point -- a single SDL_RenderDrawPoint pixel is nearly
   invisible on screen, so each center is drawn as a small filled
   square instead, still just one red "point" per cell */
#define CENTER_MARKER_SIZE 4
 
/* ---------------------------------------------------------
 * GridPoint / GridPointArray
 * --------------------------------------------------------- */
typedef struct {
    int id;        /* assigned in discovery order; equals its own array index */
    int Placement;
    int Row;
    float x;
    float y;
    bool visible;  /* true if inside the SECOND (visibility) box */
} GridPoint;
 
typedef struct {
 GridPoint points[GPointDensity];
 float startingX;
 float startingY;
 int count;
} GridPointArray;
 
GridPointArray LandGrid;
GridPointArray SkyGrid;
GridPointArray *LandandSky[2] = {&LandGrid,&SkyGrid};

 

void DrawRotatingGrid(SDL_Renderer *renderer,int centerX, int centerY,int armLength, float degrees, int GridDensity)
{
    float rad = degrees * (Pi / 180.0);
    float d1x = cos(rad),  d1y = sin(rad);
    float d2x = -sin(rad), d2y = cos(rad);
    float spacing = (GridDensity > 0) ? ((float)armLength / (float)GridDensity) : 0.0;
 
    SDL_Color mainColor = { 255, 255, 255, 255 };
    SDL_Color gridColor = { 77, 77, 77, 80  };
 
    for (int k = -GridDensity; k <= GridDensity; k++) {
        float shift = k * spacing;
        float ox = centerX + d2x * shift;
        float oy = centerY + d2y * shift;
        int x1 = (int)lround(ox - d1x * armLength);
        int y1 = (int)lround(oy - d1y * armLength);
        int x2 = (int)lround(ox + d1x * armLength);
        int y2 = (int)lround(oy + d1y * armLength);
        SDL_Color c = (k == 0) ? mainColor : gridColor;
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
 
    for (int k = -GridDensity; k <= GridDensity; k++) {
        float shift = k * spacing;
        float ox = centerX + d1x * shift;
        float oy = centerY + d1y * shift;
        int x1 = (int)lround(ox - d2x * armLength);
        int y1 = (int)lround(oy - d2y * armLength);
        int x2 = (int)lround(ox + d2x * armLength);
        int y2 = (int)lround(oy + d2y * armLength);
        SDL_Color c = (k == 0) ? mainColor : gridColor;
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        SDL_RenderLine(renderer, x1, y1, x2, y2);
    }
}

/*
 * ComputeGridCenters
 * --------------------
 * Recomputes every cell-center point for the current rotation,
 * runs each one through the exclusion box first (points outside
 * are dropped, never stored), then runs each SURVIVING point
 * through the visibility box to set its `visible` flag.
 *
 * xMin/xMax/yMin/yMax   : primary box -- determines what gets
 *                          stored at all.
 * xMin2/xMax2/yMin2/yMax2: secondary box -- determines the
 *                          `visible` flag on whatever was stored.
 *
 * Returns a heap-allocated GridPointArray; caller must eventually
 * pass it to FreeGridPointArray(). Nothing is cached between
 * calls -- call this again (e.g. next frame, after `degrees`
 * changes) and it recomputes everything from scratch.
 */
void ComputeGridCenters( int armLength, float degrees, int GridDensity,int type) {
  GridPointArray *GpointStruct = LandandSky[type];
 GpointStruct->count = 0;
  
    float rad = degrees * (Pi / 180.0);
    float d1x = cos(rad),  d1y = sin(rad);
    float d2x = -sin(rad), d2y = cos(rad);
    float spacing = (float)armLength / (float)GridDensity;
 
    int maxPoints = (2 * GridDensity) * (2 * GridDensity); /* upper bound, before exclusion */
 
    for (int i = -GridDensity; i <= GridDensity - 1; i++) {
        float u = (i + 0.5) * spacing;
        for (int j = -GridDensity; j <= GridDensity - 1; j++) {
            float v = (j + 0.5) * spacing;

            if (GpointStruct->count >= GPointDensity) {
                return; // hard ceiling hit -- stop writing, never overrun points[]
            }

            float px = LandandSky[type]->startingX + d1x * u + d2x * v;
            float py = LandandSky[type]->startingY + d1y * u + d2y * v; // startingY, not startingX

            LandandSky[type]->points[GpointStruct->count].id = GpointStruct->count;
            LandandSky[type]->points[GpointStruct->count].x = px;
            LandandSky[type]->points[GpointStruct->count].y = py;
            // printf("STEP %d  X: %f Y: %f\n",GpointStruct->count,  LandandSky[type]->points[GpointStruct->count].x,LandandSky[type]->points[GpointStruct->count].y);
            GpointStruct->count++;
        }
        
    }
 
}
 
/* ---------------------------------------------------------
 * Drawing (arms + grid lines -- unchanged from v2/v3)
 * --------------------------------------------------------- */

 
/* draws every stored point: bright red if visible, dim red if not,
   just so the two-box logic is visually checkable */
void DrawGridPoints(SDL_Renderer *renderer,  GridPointArray *GPoints, float xMin, float xMax, float yMin, float yMax,
                                    float xMin2, float xMax2, float yMin2, float yMax2)
{
    for (int i = 0; i < GPoints->count; i++) {
        GridPointArray *gp = GPoints;
      


            /* ---- STEP 1: exclusion against the primary box ---- */
            if (gp->points[i].x  < xMin || gp->points[i].x  > xMax || gp->points[i].y < yMin || gp->points[i].y > yMax) {
               // continue; /* dropped entirely -- no id, no storage */// Bufferzone, This code may have to be better structured later
            }


     if (gp->points[i].x >= xMin2 && gp->points[i].x <= xMax2 && gp->points[i].y >= yMin2 && gp->points[i].y <= yMax2){
                gp->points[i].visible = true;
            }
            else {
                gp->points[i].visible = false;
            }
 
          
        if (gp->points[i].visible) {
               SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  
            if (gp->points[i].id % 100 ==0)
            {
                  SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);  
            }
            
        SDL_FRect marker;
           printf("%d X: %f Y: %f\n",i, gp->points[i].x,gp->points[i].y);
        marker.x = (int)lround(gp->points[i].x) - CENTER_MARKER_SIZE / 2;
        marker.y = (int)lround(gp->points[i].y) - CENTER_MARKER_SIZE / 2;
        marker.w = CENTER_MARKER_SIZE;
        marker.h = CENTER_MARKER_SIZE;
        SDL_RenderFillRect(renderer, &marker);
        }
 
   
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


Message->StartingY = StartingY;
Message->StartingX = StartingX;
Message->Size = Size; 

if (Message->LastMessage != Message->Text){
   
char CapitalLEtters[]="ABCDEFGHIJKLMNOPQRSTUVWXYZ. 1234567890*!$%";
char LowerCaseLetters[]="abcdefghijklmnopqrstuvwxyz. 1234567890*!$%";

for (int i = 0; i < (int)strlen(Message->Text); i++){
char Letter = Message->Text[i];

for (int j = 0; j < strlen(CapitalLEtters); j++){
if (CapitalLEtters[j] == Letter) {
    Message->Textint[i] = j;
   
    break;}

if (LowerCaseLetters[j] == Letter) {
    Message->Textint[i] = j;
   
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
SDL_SetRenderDrawColor(Firstrenderer, 0, 0, 0, 255);
SDL_RenderClear(Firstrenderer);
// ASSETS //
SDL_Surface *Text = SDL_LoadBMP("images/Text.bmp");
SDL_Texture *TextTexture = SDL_CreateTextureFromSurface(Firstrenderer, Text);
// ASSETS //



for (int i = 0; i < 2; i++)
{
    GridPointArray *gp = LandandSky[i];

    gp->count= 0;
    gp->startingX = 0;
    gp->startingY = 0;
for (int f = 0; f < GPointDensity; f++)
{
    gp->points->id = 0;
    gp->points->Placement = 0;
    gp->points->Row = 0;
    gp->points->visible = false;
    gp->points->x = 0;
}   gp->points->y = 0;

}




ScreenCenterX = ScreenWidth / 2;
ScreenCenterY = ScreenHeight / 2;
HorizonLine = ScreenCenterY; //* HorizonLineFactorial; // standard starting point of the horizon, 60% to the top 

Pointer.x = ScreenCenterX; 
Pointer.y = ScreenCenterY;
Pointer.w = 10;
Pointer.h = 10;  
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
TEMPGLOBALHORIZONTALLOOKDEGREE = Perspective->LookHorizontalAxis;
TEMPGLOBALVERTICALLOOKDEGREE = Perspective->LookVerticalAxis;
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
              
                Pointer.x = x; // Replaced by entityState struct, basic Camera Pan Flight style movement calc
                Pointer.y = y; // Shows where you are looking tho with the dot, so its good to stay for a bit, REALLY cant leave too many "I need to get back to this style nonsense" or im cooked
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


 SDL_SetRenderDrawColor(Firstrenderer,0,0,0,255);
 SDL_FRect Sky = { 0, 0 ,ScreenWidth ,HorizonLine};
 SDL_RenderFillRect(Firstrenderer,&Sky);
 SDL_SetRenderDrawColor(Firstrenderer,100,100,100,255);
 SDL_FRect Land = { 0, HorizonLine ,ScreenWidth ,ScreenHeight};
 SDL_RenderFillRect(Firstrenderer,&Land);


char Cbuffer[512];
char Coordbuffer[512] = "Patch Of Eden*";
snprintf(Cbuffer, sizeof(Cbuffer), "X %.3f*Y % .3f*Z %.3f*Horizantal Facing Degree %.3f*Vertical Facing Degree %.3f*Mac Daddy 2026",Perspective->x, Perspective->y, Perspective->z, Perspective->LookHorizontalAxis, Perspective->LookVerticalAxis); 
strcat(Coordbuffer, Cbuffer);
strcat(Coordbuffer, "!");
strcpy(Coordinates.Text, Coordbuffer); 
TypeTextToScreen(&Coordinates, 1, 10, 10);

for (int d = 0; d < strlen(Coordinates.Text)  -1; d++)
{
  SDL_RenderTexture(Firstrenderer, TextTexture, &Coordinates.srcrect[d], &Coordinates.destrect[d]);
}



SDL_SetRenderDrawBlendMode(Firstrenderer, SDL_BLENDMODE_BLEND);
SDL_SetRenderDrawColor(Firstrenderer,255,40,40,255);


        SDL_RenderFillRect(Firstrenderer,&Pointer);


//SDL_SetRenderDrawColor(Firstrenderer, 255, 255, 255, 255);




       // DrawCircle(Firstrenderer, ScreenCenterX, ScreenCenterY, 100);
        //  DrawGrid(Firstrenderer, ScreenCenterX, ScreenHeight, ScreenWidth/8, GridRowCT); // Even numbers only
        DrawEarthNSky(Firstrenderer);


SDL_SetRenderDrawColor(Firstrenderer, 255,255,255,255);
SDL_RenderFillRect(Firstrenderer, &SkyConvergencePoint);

SDL_RenderFillRect(Firstrenderer, &LandConvergencePoint);

//DrawRotatingCross(Firstrenderer,SkyConvergencePoint.x + (PointOfConvergenceSize /2), SkyConvergencePoint.y + (PointOfConvergenceSize /2), ScreenHeight*.8, TEMPGLOBALHORIZONTALLOOKDEGREE);
//DrawRotatingCross(Firstrenderer,LandConvergencePoint.x + (PointOfConvergenceSize /2), LandConvergencePoint.y + (PointOfConvergenceSize /2), ScreenHeight*.8, TEMPGLOBALHORIZONTALLOOKDEGREE);



LandandSky[0]->startingX = LandConvergencePoint.x + (PointOfConvergenceSize /2);
LandandSky[1]->startingX = SkyConvergencePoint.x + (PointOfConvergenceSize /2);
LandandSky[0]->startingY = LandConvergencePoint.y + (PointOfConvergenceSize /2);
LandandSky[1]->startingY = SkyConvergencePoint.y + (PointOfConvergenceSize /2);

//DrawRotatingGrid(Firstrenderer,LandConvergencePoint.x + (PointOfConvergenceSize /2), LandConvergencePoint.y + (PointOfConvergenceSize /2), ScreenHeight - HorizonLine,TEMPGLOBALHORIZONTALLOOKDEGREE,32);


ComputeGridCenters(  ScreenWidth, TEMPGLOBALHORIZONTALLOOKDEGREE, 14, 0 );
      
        DrawGridPoints(Firstrenderer, &LandGrid,
            0 - ChunkBuffer, ScreenWidth +ChunkBuffer, HorizonLine + ChunkBuffer, ScreenHeight + ChunkBuffer,
            0, ScreenWidth, HorizonLine, ScreenHeight);
      

//DrawRotatingGrid(Firstrenderer,SkyConvergencePoint.x + (PointOfConvergenceSize /2), SkyConvergencePoint.y + (PointOfConvergenceSize /2),HorizonLine,TEMPGLOBALHORIZONTALLOOKDEGREE,32);


ComputeGridCenters(  ScreenWidth, TEMPGLOBALHORIZONTALLOOKDEGREE, 14, 1 );
      
        DrawGridPoints(Firstrenderer, &SkyGrid, 
            0 - ChunkBuffer, ScreenWidth +ChunkBuffer, 0 -ChunkBuffer, HorizonLine+ ChunkBuffer,
            0, ScreenWidth, 0, HorizonLine);
            int TestConeSize = 32;
            for (int p = 0; p < 10; p++)
            { 
                if (LandGrid.points[p*100].visible)
                {
                      DrawCone(Firstrenderer, LandGrid.points[p*100].x, LandGrid.points[p*100].y - TestConeSize, TestConeSize);
                }
                
             
            }
            
           


         SDL_RenderPresent(Firstrenderer);
   



 SDL_Delay(16); // 16 is about 60 fps

        }


    SDL_DestroyRenderer(Firstrenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

       
    }




   
    