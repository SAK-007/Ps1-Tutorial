#include "Engine/include/3D/3D.H"

void initGameObjects(void);
void update(void);
void draw(void);
void controls(void);


unsigned long* data[10];
Mesh Grid,Cube;
MESHData meshdata[10];


int CameraSpeed       = 20;
int CameraSensitivity = 25;

void main()
{
    StartAmbientLight();
    initializeScreen();
    initializeFont();
    initializePad();
    initializeMemory();

    initGameObjects();

    SetBGColor(0,0,255);



    while(1)
    {

        controls();
        update();
        clearDisplay();
        draw();
        display();
    }
    return 0;
}






void initGameObjects()
{
    cd_open();

    cd_read_file("GRID.TMD" ,&data[0]);
    cd_read_file("CUBE.TMD" ,&data[1]);
    cd_read_file("CRASH.TIM",&data[2]);

    cd_close();

    LoadTMD(data[0],&meshdata[0],1);
    LoadTMD(data[1],&meshdata[1],1);



    Camera.position.vx = 0;
    Camera.position.vy = 300;
    Camera.position.vz = 0;

    Grid.position.vx = 0;
    Grid.position.vy = 0;
    Grid.position.vz = 0;

    Cube.position.vx = 0;
    Cube.position.vy = 500;
    Cube.position.vz = 0;
}









void update()
{
    CalculateCamera();
}






void draw()
{
    RenderObject(Grid.pos,Grid.rot,&meshdata[0]);
    RenderObject(Cube.pos,Cube.rot,&meshdata[1]);
}






void controls()
{
    padUpdate();


    if(padCheck(Pad1Up))      {Camera.position.vz -= CameraSpeed;}
    if(padCheck(Pad1Down))    {Camera.position.vz += CameraSpeed;}
    if(padCheck(Pad1Left))    {Camera.position.vx += CameraSpeed;}
    if(padCheck(Pad1Right))   {Camera.position.vx -= CameraSpeed;}


    if(padCheck(Pad1Triangle)){Camera.rotation.vx -= CameraSensitivity;}
    if(padCheck(Pad1Cross))   {Camera.rotation.vx += CameraSensitivity;}
    if(padCheck(Pad1Square))  {Camera.rotation.vy += CameraSensitivity;}
    if(padCheck(Pad1Circle))  {Camera.rotation.vy -= CameraSensitivity;}
}
