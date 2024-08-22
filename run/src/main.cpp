#include "raylib.h"
#include "raylib-cpp.hpp"


struct AnimData 
{
    Rectangle rec;
    Vector2 pos;
    int frame;
    float updateTime;
    float runningTime;
};

//ground check function
bool isOnGround(AnimData data, int windowHeight)
{
    return data.pos.y>=windowHeight-data.rec.height;
    
}
//counts the score
bool ScoreCounter(AnimData character, AnimData hazard[],int size)
{
    bool pass=false;
    for(int i=0;i<size;i++)
    {
        if(character.pos.x>=hazard[i].pos.x-10&&character.pos.x<=hazard[i].pos.x)
            pass=true;
    }
    return pass;
}

//updates animation for sprites
AnimData updateAnimData(AnimData data, float deltaTime, int maxFrame)//maxFrame used for the variable frame values
{
    data.runningTime+=deltaTime;
        if(data.runningTime>=data.updateTime)
        {
            data.runningTime=0;
            data.rec.x = data.frame*(data.rec.width);
            data.frame++;
            if(data.frame>maxFrame)
                data.frame=0;
        }
    return data;

}



int main()
{
    //array of window dimensions
    int windowDimension[2]={512,384};
    /*Refactored***********************************
    const int windowWidth{512};
    const int windowHeight{384};
    **************************************************/
    // initialize the window
    InitWindow(windowDimension[0], windowDimension[1], "Royalty Free I Swear");
    
// acceleration due to gravity (pixels/sec)/sec
    const int gravity{1'000};
//num of nebula
    const int sizeOfNebulea=100;
//jump velocity
    const int jumpVel{-650};
//nebula speed
    int nebVel{-200};
//score
    int score{};


//SCARFY VARIABLES
    //loading textures
    Texture2D scarfy = LoadTexture("textures/scarfy.png");
/*Refactored**************************************************
    Rectangle scarfyRec;
    scarfyRec.width = scarfy.width/6;
    scarfyRec.height = scarfy.height;
    scarfyRec.x = 0;
    scarfyRec.y = 0;
    //initialize pos on screen
    Vector2 scarfyPos;
//Scarfy starting pos
    scarfyPos.x = windowWidth/2 - scarfyRec.width/2;
    scarfyPos.y = windowHeight - scarfyRec.height;
//Scarfy ANIMATION TIME
    const float updateTime= 1.0/12.0;
    float runningTime=0;
    //animation frame
    int frame{};
************************************************************************/

//SCARFY ANIM DATA
    AnimData scarfyData{
        {0,0,scarfy.width/6,scarfy.height}//Rectangle Rec
        ,{windowDimension[0]/3 - scarfyData.rec.width/2,windowDimension[1] - scarfyData.rec.height}//Vector2 Pos
        ,0,//frame
        1.0/12.0,//updateTime
        0};//runningTime


    // is in air
    bool isInAir{};

    //scarfy velocity
    int velocity{0};
    //move speed
    int moveSpeed{200};
    //Audio Timer variable
    int timer{};

    //BACKROUND
    Texture2D backround =LoadTexture("textures/far-buildings.png");
    float bgX{};
    //MIDGROUND
    Texture2D midground =LoadTexture("textures/back-buildings.png");  
    float mdX{};  
    //FOREGROUND
    Texture2D foreground =LoadTexture("textures/foreground.png");
    float frX{};

//NEBULA HAZARD VARIABLES

//REFACTORED****************************************************************
    //nebula ANIM Time
    //int nebFrame{};
    //const float nebUpdateTime{1.0/12.0};
    //float nebRunningTime{};
//REFACTORED****************************************************************

 


//NEBULA loading texture
    Texture2D nebula = LoadTexture("textures/12_nebula_spritesheet.png");

/*REFACTORED********************************************************
//ANIME DATA FOR NEBULA
    AnimData nebData{
        {(0.0),(0.0),nebula.width/8,nebula.height/8},//rectangle Rec
        {windowDimension[0],windowDimension[1]-nebula.height/8}//Vector2 pos
        ,0,//frame
        (1.0/12.0),//updateTime
        0//runningTime
        };


//ANIME DATA FOR NEBULA2
    AnimData neb2Data{
        {(0.0),(0.0),nebula.width/8,nebula.height/8},//rectangle Rec
        {windowDimension[0]+300,windowDimension[1]-nebula.height/8}//Vector2 pos
        ,0,//frameTime
        (1.0/16.0),//updateTime
        0//runningTime
        };
******************************************************************************/

//ARRAY ON NEBULA ANIMATION DATA
    //changes size of array

    AnimData nebulea[sizeOfNebulea]{};
    //initializing all similar variables
    for(int i=0; i < sizeOfNebulea; i++)
    {
        nebulea[i].rec.x=0.0;
        nebulea[i].rec.y=0.0;
        nebulea[i].rec.width=nebula.width/8;
        nebulea[i].rec.height=nebula.height/8;
        //not initializing pos.x since it is different between nebulea
        nebulea[i].pos.y= GetRandomValue(-((nebula.height/8)/2),windowDimension[1]-nebula.height/8);//windowDimension[1]-nebula.height/8;
        nebulea[i].frame=0;
        nebulea[i].updateTime=1.0/16.0;
        nebulea[i].runningTime=0;
        nebulea[i].pos.x=windowDimension[0]+i*400;

    }
    //FINISH LINE
    float finish{nebulea[sizeOfNebulea-1].pos.x};

/*REFACTORED*************************************************
    //initializing idffering varaibles
    nebulea[0].pos.x=windowDimension[0];
    nebulea[1].pos.x=windowDimension[0]+300;
    nebulea[2].pos.x=windowDimension[0]+600;
***************************************************/

//REFACTORED****************************************************************
    //Rectangle nebRec{(0.0),(0.0),nebula.width/8,nebula.height/8};
    //initialize position
    //Vector2 nebPos{windowWidth,windowHeight-nebRec.height};

    //nebula hazard 2
    //Rectangle neb2Rec{(0.0),(0.0),nebula.width/8,nebula.height/8};
    //Vector2 neb2Pos{windowWidth+300,windowHeight-nebRec.height};
//**************************************************************************
//collision detection
    bool collision{};

//Init audio device and loading sounds
    InitAudioDevice();
    Wave jumpWave = LoadWave("sounds/Jump sound.wav");
    Wave scoreWave = LoadWave("sounds/score sound.wav"); 
    Wave deadWave = LoadWave("sounds/fail-sound-effect.wav");
    Wave winWave = LoadWave("sounds/win.wav");
    Sound jumpSound =LoadSoundFromWave(jumpWave);
    Sound scoreSound =LoadSoundFromWave(scoreWave); 
    Sound deadSound =LoadSoundFromWave(deadWave);
   Sound winSound =LoadSoundFromWave(winWave);
    Music LinkinPark= LoadMusicStream("sounds/linkin-park.mp3");

    PlayMusicStream(LinkinPark);
//START OF GAME LOOP

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        UpdateMusicStream(LinkinPark);
        //delta time (time since last frame)
        const float DT{GetFrameTime()};
        // start drawing
        BeginDrawing();
        ClearBackground(WHITE);

        //score board


        //UPDATE BACKROUND************************************************************************
        bgX-=20*DT;
        mdX-=40*DT;
        frX-=80*DT;
        if(bgX<=-backround.width*2)
        {
            bgX=0.0;
        }
        if(mdX<=-midground.width*2)
        {
            mdX=0.0;
        }
        if(frX<=-foreground.width*2)
        {
            frX=0.0;
        }
        
        //DRAW BACKROUND
        Vector2 bg1Pos{bgX,0.0};
        Vector2 bg2Pos{bgX+backround.width*2,0.0};//width multiplied by 2 since scale is 2x
        DrawTextureEx(backround,bg1Pos,0.0,2.0,WHITE);
        DrawTextureEx(backround,bg2Pos,0.0,2.0,WHITE);
        //DRAW MIDGROUND
        Vector2 mid1Pos{mdX,0.0};
        Vector2 mid2Pos{mdX+midground.width*2,0.0};  
        DrawTextureEx(midground,mid1Pos,0.0,2.0,WHITE);
        DrawTextureEx(midground,mid2Pos,0.0,2.0,WHITE);      
        //DRAW FOREGROUND
        Vector2 for1Pos{frX,0.0};
        Vector2 for2Pos{frX+foreground.width*2,0.0};  
        DrawTextureEx(foreground,for1Pos,0.0,2.0,WHITE);
        DrawTextureEx(foreground,for2Pos,0.0,2.0,WHITE);   
    //BEGIN GAME LOGIC


    //score board
    const char *scoreI=TextFormat("Score: %d",score);
    DrawText(scoreI,windowDimension[0]/3+20,windowDimension[1]/8,30,RED);



    //SCARFY CHECKS AND UPDATES
        if (isOnGround(scarfyData,windowDimension[1]))
        {
            // rectangle is on the ground
            velocity = 0;
            isInAir = false;
        }
        else
        {
            // rectangle is in the air
            velocity += gravity*DT;
            isInAir = true;
        }

        // jump check
        if (IsKeyPressed(KEY_SPACE) && !isInAir)
        {
            velocity += jumpVel;
            PlaySound(jumpSound);
        }

        if(IsKeyDown(KEY_D))
        {
            scarfyData.pos.x+=moveSpeed*DT;
        }
        if(IsKeyDown(KEY_A))
        {
            scarfyData.pos.x+=-(moveSpeed*DT);
        }

//for loop for updating nebula pos
        for(int i =0;i<sizeOfNebulea;i++)
        {
            nebulea[i].pos.x+=nebVel*DT;
        }

//UPDATE FINISH LINE**************************************
        finish+=nebVel*DT;
//********************************************

      /*REFACTORED**********************************************************  
        //update nebula pos
        nebulea[0].pos.x+=nebVel*DT;
        //update nebula2 pos
        nebulea[1].pos.x+=nebVel*DT;
        //update nebula3 pos
        nebulea[2].pos.x+=nebVel*DT;        
        *************************************************************************/
        // update SCARFY position
        scarfyData.pos.y += velocity*DT;

        if(!isInAir)
        {
            scarfyData=updateAnimData(scarfyData,DT,5);
        }
        else if(isInAir)
        {
            if(velocity<0)
            {
                scarfyData.rec.x=3*(scarfy.width/6);
            }
            else
                scarfyData.rec.x=4*(scarfy.width/6);

        }
/*REFACTORED************************************************************************
        //update Scarfy animation frame
        scarfyData.runningTime+=DT;
        if(scarfyData.runningTime>=scarfyData.updateTime&&!isInAir)
        {
            scarfyData.runningTime=0;
            scarfyData.rec.x = scarfyData.frame*(scarfy.width/6);
            scarfyData.frame++;
            if(scarfyData.frame>5)
                scarfyData.frame=0;
        }
        //Gets specific jump sprite and stop animation on jump
        else if(scarfyData.runningTime>=scarfyData.updateTime&&isInAir)
        {
            scarfyData.rec.x = 0;            
        }
************************************************************************************/

//update nebula animation
for(int i=0;i<sizeOfNebulea;i++)
{
    nebulea[i]=updateAnimData(nebulea[i],DT,7);
}

/*REFACTORED*************************************************************************
    //UPDATE loop for NEBULA HAZARD ANIMATION
        for(int i =0; i<sizeOfNebulea; i++)
        {
            nebulea[i].runningTime+=DT;
            if(nebulea[i].runningTime>=nebulea[i].updateTime)
            {
                //goes through row of sprite sheet
                nebulea[i].runningTime=0;
                nebulea[i].rec.x=nebulea[i].frame*(nebula.width/8);
                nebulea[i].frame++;
                //checks for final row of sprite sheet and resets the y pos 
                if(nebulea[i].frame>7)
                {
                    nebulea[i].frame=0;
                }
            }

        }
******************************************************************************************/
      /*REFACTORED**********************************************************  
        }
        nebulea[0].runningTime+=DT;
        if(nebulea[0].runningTime>=nebulea[0].updateTime)
        {
            //goes through row of sprite sheet
            nebulea[0].runningTime=0;
            nebulea[0].rec.x=nebulea[0].frame*(nebula.width/8);
            nebulea[0].frame++;
            //checks for final row of sprite sheet and resets the y pos 
            if(nebulea[0].frame>7)
            {
                nebulea[0].frame=0;
            }

        }
    //UPDATE NEBULA2 HAZARD ANIMATION
        nebulea[1].runningTime+=DT;
        if(nebulea[1].runningTime>=nebulea[1].updateTime)
        {
            //goes through row of sprite sheet
            nebulea[1].runningTime=0;
            nebulea[1].rec.x=nebulea[1].frame*(nebula.width/8);
            nebulea[1].frame++;
            //checks for final row of sprite sheet and resets the y pos 
            if(nebulea[1].frame>7)
            {
                nebulea[1].frame=0;
            }

        }
//udate nebula 3
        nebulea[2].runningTime+=DT;
        if(nebulea[2].runningTime>=nebulea[2].updateTime)
        {
            //goes through row of sprite sheet
            nebulea[2].runningTime=0;
            nebulea[2].rec.x=nebulea[2].frame*(nebula.width/8);
            nebulea[2].frame++;
            //checks for final row of sprite sheet and resets the y pos 
            if(nebulea[2].frame>7)
            {
                nebulea[2].frame=0;
            }

        }
    

     ************************************************************************************/   
    
//COLLISION DETECTION

    for(AnimData nebula:nebulea)
    {
        float pad{50};
        Rectangle nebRec{
            nebula.pos.x +pad,
            nebula.pos.y +pad,
            nebula.rec.width -2 *pad,
            nebula.rec.height -2 * pad
        };
        Rectangle scarfyRec{
            scarfyData.pos.x,
            scarfyData.pos.y,
            scarfyData.rec.width,
            scarfyData.rec.height
        };
        if(CheckCollisionRecs(nebRec,scarfyRec))
            collision=true;
    }

        

    if(collision)
    {
      DrawText("YOU HAVE DIED",windowDimension[0]/4,windowDimension[1]/2,30,LIGHTGRAY);

      if(timer==0)
      {
        PlaySound(deadSound);
        StopMusicStream(LinkinPark);
      }

        timer++;

    }

    else if(scarfyData.pos.x>finish)
    {
      DrawText("YOU HAVE WON",windowDimension[0]/4,windowDimension[1]/2,30,LIGHTGRAY);
      if(timer==0)
      {
        PlaySound(winSound);
        StopMusicStream(LinkinPark);
      }
        timer++;     
    }
    else
    {

        //draw nebula loop
        for(int i=0;i<sizeOfNebulea;i++)
        {
            DrawTextureRec(nebula, nebulea[i].rec, nebulea[i].pos, WHITE);        
        }
    /*REFACTORED**************************************************************************
        //DRAW NEBULA
            DrawTextureRec(nebula, nebulea[0].rec, nebulea[0].pos, WHITE);
        //DRAW NEBULA2
            DrawTextureRec(nebula, nebulea[1].rec, nebulea[1].pos, RED);
        //DRAW NEBULA3
            DrawTextureRec(nebula, nebulea[2].rec, nebulea[2].pos, BLUE);
    ************************************************************************************/
        //draw scarfy
        DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE);
        //SCORE COUNTER
        if(ScoreCounter(scarfyData,nebulea,sizeOfNebulea))
        {
            score++;
            PlaySound(scoreSound);
        }

        //increase the speed of the hazard every 5 points
        if(score%10==0&&score>0)
            nebVel-=1;
    
    }

            // stop drawing
            EndDrawing();
    

    }

    UnloadTexture(scarfy);
    UnloadTexture(nebula);
    UnloadTexture(backround);
    UnloadTexture(midground);
    UnloadTexture(foreground);
    CloseWindow();
}    







