#include "Engine/include/3D/3D.H"


void initGameObjects(void);
void update(void);
void draw(void);
void controls(void);
void updatePhysics(void);

unsigned long* data[10];
Mesh Grid,Cube;
MESHData meshdata[10];

int CameraSpeed       = 50 ;
int RunSpeed          = 100;
int CameraSensitivity = 40 ;
int CubeSpeed         = 25 ;
int CubeRotationSpeed = 15 ;
int tf                = 0  ;
int Gravity           = 150 ;
int JumpHeight        = 8000;
int JumpSpeed         = 300 ;
int isJump            = 0;
int FloorPos          = 2000;
int onFloor           = 1;

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
        updatePhysics();
        controls();
        update();
        clearDisplay();
        draw();
        display();
    }

}





void initGameObjects()
{
    cd_open();

    cd_read_file("GRID.TMD",&data[0]);
    cd_read_file("CUBE.TMD",&data[1]);
    cd_read_file("CRASH.TIM",&data[2]);

    cd_close();

    LoadTMD(data[0],&meshdata[0],1);
    LoadTMD(data[1],&meshdata[1],1);

    loadTexture((u_char *)data[2]);


    Camera.position.vx = -150;
    Camera.position.vy = 2000;
    Camera.position.vz = 4600;

    Grid.position.vx = 0;
    Grid.position.vy = 0;
    Grid.position.vz = 0;

    Cube.position.vx = 0;
    Cube.position.vy = 1500;
    Cube.position.vz = 0;
}





void update()
{
    Cube.rotation.vx += CubeRotationSpeed;
    Cube.rotation.vy += CubeRotationSpeed;

    if(!tf)
    {
        Cube.position.vy += CubeSpeed;
    }
    if(Cube.pos.vy > 3500)
    {
        tf = 1;
    }
    if(tf)
    {
        Cube.position.vy -= CubeSpeed;
    }
    if(Cube.position.vy < 1501)
    {
        tf = 0;
    }

    if(isJump && Camera.position.vy < JumpHeight)
    {
        Camera.position.vy += JumpSpeed;
    }

    if(Camera.position.vy >= JumpHeight)
    {
        isJump = 0;
    }

    if(Camera.position.vy > 2000)
    {
        onFloor = 0;
    }

    if(!onFloor)
    {
        Camera.pos.vy -= Gravity;
    }

    if(Camera.position.vy <= FloorPos)
    {
        onFloor = 1;
    }

    CalculateCamera();
}


void updatePhysics()
{
    if(Camera.pos.vy > 2000)
    {
        onFloor = 0;
    }
    if(Camera.position.vx > 10500)
    {
        Camera.pos.vy -= Gravity;
    }
    if(Camera.position.vx < -10200)
    {
        Camera.pos.vy -= Gravity;
    }
    if(Camera.position.vz < -10600)
    {
        Camera.pos.vy -= Gravity;
    }
    if(Camera.position.vz > 10200)
    {
        Camera.pos.vy -= Gravity;
    }


    if(Camera.position.vy < -20000)
    {
        Camera.position.vx = -150;
        Camera.position.vy = 1500;
        Camera.position.vz = 4600;
    }

}



void draw()
{

    RenderMesh(Grid,&meshdata[0]);
    RenderMesh(Cube,&meshdata[1]);

}




void controls()
{
    padUpdate();


    if(padCheck(Pad1Up))
    {
        Camera.position.vz -= CameraSpeed;
        if(padCheck(Pad1L1))
        {
            Camera.position.vz -= RunSpeed;
        }
    }
    if(padCheck(Pad1Down))
    {
        Camera.position.vz += CameraSpeed;
        if(padCheck(Pad1L1))
        {
            Camera.position.vz += RunSpeed;
        }
    }
    if(padCheck(Pad1Left))
    {
        Camera.position.vx += CameraSpeed;
        if(padCheck(Pad1L1))
        {
            Camera.position.vx += RunSpeed;
        }
    }
    if(padCheck(Pad1Right))
    {
        Camera.position.vx -= CameraSpeed;
        if(padCheck(Pad1L1))
        {
            Camera.position.vx -= RunSpeed;
        }
    }


    if(padCheck(Pad1Triangle) && Camera.rot.vx > -1000)
    {
        Camera.rotation.vx -= CameraSensitivity;
    }
    if(padCheck(Pad1Cross)&& Camera.rot.vx < 1000)
    {
        Camera.rotation.vx += CameraSensitivity;
    }
    if(padCheck(Pad1Square))
    {
        Camera.rotation.vy += CameraSensitivity;
    }
    if(padCheck(Pad1Circle))
    {
        Camera.rotation.vy -= CameraSensitivity;
    }


    if(padCheck(Pad1R2) && onFloor && !isJump)
    {
        isJump = 1;
    }
}
