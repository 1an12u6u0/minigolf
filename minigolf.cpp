#include <raylib.h>
#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

ifstream fin ("data.in");

const int cellcountx=25;
const int cellcounty=30;
const int cellsize=30;

int level=1;
int points=0;

enum GameState{Menu,Gameplay};

class Ball{
    public:
        int radius=15;
        float speedx=0;
        float speedy=0;
        float x=cellcountx*cellsize/2;
        float y=cellcounty*cellsize/2;
        double friction=0.95f;

        void Draw(){
            DrawCircle(x,y,radius,WHITE);
        }

        void Read(){
            speedx=0;
            speedy=0;
            fin>>x;
            fin>>y;
        }
        /*
        void level(int level_number){
            switch(level_number){
                case 1:{
                    speedx=0;
                    speedy=0;
                    break;
                }
                case 2:{
                    speedx=0;
                    speedy=0;
                    x=GetScreenWidth()/2;
                    y=740;
                    break;
                }
                case 3:{
                    speedx=0;
                    speedy=0;
                    x=GetScreenWidth()/5*4;
                    y=740;
                    break;
                }
                case 4:{
                    speedx=0;
                    speedy=0;
                    x=GetScreenWidth()/5;
                    y=800;
                    break;
                }
                case 5:{
                    break;
                }
            }
        }*/

        void Update(){
            speedx*=friction;
            speedy*=friction;

            x+=speedx;
            y+=speedy;

            if(abs(speedx)<0.05) speedx=0;
            if(abs(speedy)<0.05) speedy=0;
        }

        void Check(Sound ball_hit){
            if(x-radius<cellsize||x+radius>cellcountx*cellsize-cellsize){
                PlaySound(ball_hit);
                speedx*=-1;
            }
            if(y-radius<cellsize||y+radius>cellcounty*cellsize-cellsize){
                PlaySound(ball_hit);
                speedy*=-1;
            }
            if(x-radius<cellsize-15){
                x+=cellsize;
            }
            if(x+radius>cellcountx*cellsize+15){
                x-=cellsize;
            }
            if(y-radius<cellsize-15){
                y+=cellsize;
            }
            if(y+radius>cellcounty*cellsize+15){
                y-=cellsize;
            }
        }void CheckRec(Rectangle rec1,Rectangle rec2,Sound ball_hit){
            if(CheckCollisionCircleRec(Vector2{x,y},radius,rec1)){
                float cx=(x<rec1.x)?rec1.x:((x>rec1.x+rec1.width)?rec1.x+rec1.width:x);
                float cy=(y<rec1.y)?rec1.y:((y>rec1.y+rec1.height)?rec1.y+rec1.height:y);
                float ox=abs(x-cx);
                float oy=abs(y-cy);
                if(ox>oy){
                    speedx*=-1;
                    x=(x<rec1.x)?rec1.x-radius:rec1.x+rec1.width+radius;
                }else{
                    speedy*=-1;
                    y=(y<rec1.y)?rec1.y-radius:rec1.y+rec1.height+radius;
                }
                PlaySound(ball_hit);
            }
            if(CheckCollisionCircleRec(Vector2{x,y},radius,rec2)){
                float cx=(x<rec2.x)?rec2.x:((x>rec2.x+rec2.width)?rec2.x+rec2.width:x);
                float cy=(y<rec2.y)?rec2.y:((y>rec2.y+rec2.height)?rec2.y+rec2.height:y);
                float ox=abs(x-cx);
                float oy=abs(y-cy);
                if(ox>oy){
                    speedx*=-1;
                    x=(x<rec2.x)?rec2.x-radius:rec2.x+rec2.width+radius;
                }else{
                    speedy*=-1;
                    y=(y<rec2.y)?rec2.y-radius:rec2.y+rec2.height+radius;
                }
                PlaySound(ball_hit);
            }
        }
};

void DrawBlocks(){

    Color edges={61, 107, 43, 255};

    for(int i=0;i<cellcountx;i++){
        DrawRectangle(cellsize*i,0,cellsize,cellsize,edges);
        DrawRectangle(cellsize*i,(cellcounty-1)*cellsize,cellsize,cellsize,edges);
    }
    for(int i=0;i<cellcounty;i++){
        DrawRectangle(0,cellsize*i,cellsize,cellsize,edges);
        DrawRectangle((cellcountx-1)*cellsize,cellsize*i,cellsize,cellsize,edges);
    }
}

Ball ball;

class Hole{
    public:
        float x=0;
        float y=0;
        float radius=15;

        /*void Change(int level_number){
            switch (level_number){
                case 1:{
                    x=GetScreenWidth()/2;
                    y=200;
                    break;
                }
                case 2:{
                    x=GetScreenWidth()/2;
                    y=200;
                    break;
                }
                case 3:{
                    x=GetScreenWidth()/5-radius;
                    y=100;
                    break;
                }
                case 4:{
                    x=GetScreenWidth()-100-radius;
                    y=100;
                    break;
                }
                case 5:{
                    break;
                }
            }
        }*/

        void Draw(){
            DrawCircle(x,y,radius,BLACK);
        }

        void Read(){
            fin>>x;
            fin>>y;
        }

        bool Check(Sound hit){
            if(CheckCollisionCircles(Vector2{x,y},radius,Vector2{ball.x,ball.y},ball.radius)){
                PlaySound(hit);
                return 1;
            }
            return 0;
        }
};

class Levels{
    public:
        float x1,y1,x2,y2;

        void Read(){
            fin>>x1>>y1;
            fin>>x2>>y2;
        }

        void Draw(Sound ball_hit){
            Rectangle rec1={x1,y1,400,100};
            Rectangle rec2={x2,y2,400,100};
            
            DrawRectangleRec(rec1,WHITE);
            DrawRectangleRec(rec2,WHITE);

            ball.CheckRec(rec1,rec2,ball_hit);
            /*switch(level_number){
                case 1:{
                    Rectangle rec1={GetScreenWidth()/3-400,GetScreenHeight()/2,400,100};
                    Rectangle rec2={GetScreenWidth()/3*2,GetScreenHeight()/2,400,100};
                    
                    DrawRectangleRec(rec1,WHITE);
                    DrawRectangleRec(rec2,WHITE);

                    ball.CheckRec(rec1,rec2,ball_hit);
                    break;
                }
                case 2:{
                    Rectangle rec1={GetScreenWidth()/3-200,GetScreenHeight()/3,400,100};
                    Rectangle rec2={GetScreenWidth()/3*2-200,GetScreenHeight()/3*2-50,400,100};
                    
                    DrawRectangleRec(rec1,WHITE);
                    DrawRectangleRec(rec2,WHITE);

                    ball.CheckRec(rec1,rec2,ball_hit);
                    break;
                }
                case 3:{
                    Rectangle rec1={GetScreenWidth()/3*2-150,GetScreenHeight()/3-200,300,300};
                    Rectangle rec2={GetScreenWidth()/3-150,GetScreenHeight()/5*4-200,300,300};
                    
                    DrawRectangleRec(rec1,WHITE);
                    DrawRectangleRec(rec2,WHITE);

                    ball.CheckRec(rec1,rec2,ball_hit);
                    break;
                }
                case 4:{
                    Rectangle rec1={GetScreenWidth()/3*2-350,GetScreenHeight()/3-150,700,200};
                    Rectangle rec2={GetScreenWidth()/7-200,GetScreenHeight()/5*2+150,700,200};
                    
                    DrawRectangleRec(rec1,WHITE);
                    DrawRectangleRec(rec2,WHITE);

                    ball.CheckRec(rec1,rec2,ball_hit);
                    break;
                }
                case 5:{
                    break;
                }
            }*/
        }
};

Hole hole;
Levels Level;

/*void ChangeLevel(int level_number,Sound ball_hit){
    //hole.Change(level_number);
    Level.Draw(ball_hit);
    //ball.level(level_number);
}*/

void ReadLevel(){
    ball.Read();
    hole.Read();
    Level.Read();
}

int main(){
    
    InitWindow(cellcountx*cellsize,cellcounty*cellsize,"Mini Golf Game");
    InitAudioDevice();
    SetTargetFPS(60);

    Rectangle playButton={(float)GetScreenWidth()/2-100,(float)GetScreenHeight()/2-40,200,50};
    Rectangle quitButton={(float)GetScreenWidth()/2-100,(float)GetScreenHeight()/2+30,200,50};
    Rectangle volume_down={(float)GetScreenWidth()/2-250,(float)GetScreenHeight()/2+100,200,50};
    Rectangle volume_up={(float)GetScreenWidth()/2+50,(float)GetScreenHeight()/2+100,200,50};

    Color terrain={99, 191, 69, 255};

    GameState CurrentState=Menu;

    Sound ball_hit=LoadSound("ball-sound.mp3");
    Sound hole_hit=LoadSound("hole_sound.mp3");
    Music BackgroundMusic=LoadMusicStream("background.mp3");

    SetSoundVolume(ball_hit,0.3f);
    SetSoundVolume(hole_hit,0.3f);

    PlayMusicStream(BackgroundMusic);

    bool drag=0;
    Vector2 start={0,0};
    Vector2 end={0,0};
    float multiplier=0.7f;
    int maxdist=170;
    float volume=0.08;

    Color edges={61, 107, 43, 255};

    int change=0;
    
    ReadLevel();

    while(!WindowShouldClose()){

        UpdateMusicStream(BackgroundMusic);

        Vector2 MousePos=GetMousePosition();

        if(IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)){
            cout<<"X: "<<MousePos.x<<" Y: "<<MousePos.y<<'\n';
        }

        switch(CurrentState){
            case Menu:{
                if(CheckCollisionPointRec(MousePos,playButton)&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    CurrentState=Gameplay;
                }
                if(CheckCollisionPointRec(MousePos,volume_down)&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    if(volume>0.01){
                        volume-=0.01;
                    }
                }
                if(CheckCollisionPointRec(MousePos,volume_up)&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    if(volume<0.2){
                        volume+=0.01;
                    }
                }
                if(CheckCollisionPointRec(MousePos,quitButton)&&IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                    goto cleanup;
                }

                SetMusicVolume(BackgroundMusic,volume);

                //cerr<<volume<<'\n';
                

                break;
            }
            case Gameplay:{

                if(hole.Check(hole_hit)==1){
                    level++;
                    ReadLevel();
                }

                if(ball.speedx==0&&ball.speedy==0){
                    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                        start=GetMousePosition();
                        drag=1;
                    }
                }
                
                if(drag){
                    end=GetMousePosition();
                    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
                        drag=0;
                        
                        float dx=start.x-end.x;
                        float dy=start.y-end.y;
                        float distance=hypot(dx,dy);

                        if(distance>maxdist){
                            dx=(dx/distance)*maxdist;
                            dy=(dy/distance)*maxdist;
                        }
                        ball.speedx=dx*multiplier;
                        ball.speedy=dy*multiplier;

                        //cerr<<dx<<" "<<dy<<'\n';

                    }
                }

                Vector2 length={abs(start.x-end.x),abs(start.y-end.y)};

                ball.Check(ball_hit);

                ball.Update();

                break;
            }
        }
        

        BeginDrawing();
        
        ClearBackground(terrain);
        switch(CurrentState){
            case Menu:{
                DrawText("MINI GOLF", GetScreenWidth() / 2 - MeasureText("MINI GOLF", 50) / 2, GetScreenHeight() / 4, 50, GREEN);

                bool hoverPlay = CheckCollisionPointRec(MousePos, playButton);
                DrawRectangleRec(playButton, hoverPlay ? DARKGREEN : LIME);
                DrawText("PLAY", playButton.x + (playButton.width - MeasureText("PLAY", 20)) / 2, playButton.y + 15, 20, BLACK);

                bool hoverVolume_down = CheckCollisionPointRec(MousePos, volume_down);
                DrawRectangleRec(volume_down, hoverVolume_down ? DARKGREEN : LIME);
                DrawText("Volume Down", volume_down.x + (volume_down.width - MeasureText("Volume Down", 20)) / 2, volume_down.y + 15, 20, BLACK);

                bool hoverVolume_up = CheckCollisionPointRec(MousePos, volume_up);
                DrawRectangleRec(volume_up, hoverVolume_up ? DARKGREEN : LIME);
                DrawText("Volume Up", volume_up.x + (volume_up.width - MeasureText("Volume Up", 20)) / 2, volume_up.y + 15, 20, BLACK);

                bool hoverQuit = CheckCollisionPointRec(MousePos, quitButton);
                DrawRectangleRec(quitButton, hoverQuit ? MAROON : RED);
                DrawText("QUIT", quitButton.x + (quitButton.width - MeasureText("QUIT", 20)) / 2, quitButton.y + 15, 20, WHITE);

                break;
            }
            case Gameplay:{

                DrawBlocks();
                Level.Draw(ball_hit);
                hole.Draw();
                DrawText(TextFormat("Current level: %i",level),GetScreenWidth()/2-50*3-60,70,50,edges);
                DrawText(TextFormat("Current points: %i",points),GetScreenWidth()/2-50*3-60,GetScreenHeight()-120,50,edges);

                if(drag){
                    float dx=end.x-start.x;
                    float dy=end.y-start.y;

                    float distance=hypot(dx,dy);
                    float display_dist=distance;

                    if(display_dist>maxdist)display_dist=(float)maxdist;
                    float ratio=display_dist/maxdist;

                    if(distance>maxdist){
                        dx=(dx/distance)*maxdist;
                        dy=(dy/distance)*maxdist;
                    }

                    Color var={(unsigned char)(255*ratio),(unsigned char)(255*(1.0f-ratio)),3,255};
                    DrawLineEx(Vector2{ball.x,ball.y},Vector2{ball.x+dx,ball.y+dy},3.5f,var);
                }
                ball.Draw();
            }
        }

        EndDrawing();

    }
    cleanup:
        UnloadSound(ball_hit);
        UnloadMusicStream(BackgroundMusic);
        CloseAudioDevice();
        CloseWindow();

    return 0;
}
