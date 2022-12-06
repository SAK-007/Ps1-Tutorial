#include <STDLIB.H>
#include <STDIO.H>
#include <LIBGTE.H>
#include <LIBGPU.H>
#include <LIBGS.H>
#include <LIBETC.H>
#include <LIBSPU.H>
#include <LIBDS.H>
#include <STRINGS.H>
#include <SYS/TYPES.H>

#define MAX_OBJECTS 100
#define pos position
#define rot rotation
#define NTSC 1
#define PAL 0
#define SCREEN_MODE_NTSC 0
#define SCREEN_MODE_PAL 1
#define CENTERX	SCREEN_WIDTH/2
#define CENTERY	SCREEN_HEIGHT/2
#define DITHER 1
#define OT_LENGTH	12
#define OT_ENTRIES	1<<OT_LENGTH
#define PACKETMAX	2048
#define MeshData GsDOBJ2
#define MESHData GsDOBJ2
#define TMDDATA GsDOBJ2
#define OBJECTDATA GsDOBJ2


int SCREEN_WIDTH, SCREEN_HEIGHT;
typedef struct {
	int r;
	int g;
	int b;
} Color;


typedef struct
{
VECTOR position;
SVECTOR  rotation;
} Mesh;


typedef struct
{
VECTOR  position;
SVECTOR rotation;
} MESH;


typedef struct
{
VECTOR position;
SVECTOR  rotation;
} OBJECT;


struct {
	VECTOR	position;
	SVECTOR rotation;
	GsCOORDINATE2 coord2;
} Camera;




GsOT		orderingTable[2];
GsOT_TAG	orderingTable_TAG[2][OT_ENTRIES];
int			myActiveBuff=0;
PACKET GPUOutputPacket[2][PACKETMAX*24];
Color BGColor;
short CurrentBuffer;
GsOT OrederingTable[2];
GsDOBJ2	Object[MAX_OBJECTS];
int ObjectCount=0;
GsF_LIGHT sunLight[1];
VECTOR sunDirection;
Color sunColor;
Color ambientLightColor;






Color createColor(int r, int g, int b) {
	Color color = {.r = r, .g = g, .b = b};
	return color;
}

void SetBGColor (int r, int g, int  b) {
	BGColor = createColor(r, g, b);
}

void initializeScreen() {

	ResetGraph(0);
	if (*(char *)0xbfc7ff52=='E') {
    	SCREEN_WIDTH = 320;
    	SCREEN_HEIGHT = 256;
    	printf("Setting the PlayStation Video Mode to (PAL %dx%d)\n",SCREEN_WIDTH,SCREEN_HEIGHT);
    	SetVideoMode(1);
    	printf("Video Mode is (%ld)\n",GetVideoMode());
   	} else {
     	SCREEN_WIDTH = 320;
     	SCREEN_HEIGHT = 240;
     	printf("Setting the PlayStation Video Mode to (NTSC %dx%d)\n",SCREEN_WIDTH,SCREEN_HEIGHT);
     	SetVideoMode(0);
     	printf("Video Mode is (%ld)\n",GetVideoMode());
   }
	GsInitGraph(SCREEN_WIDTH, SCREEN_HEIGHT, GsINTER|GsOFSGPU, 1, 0);
	GsDefDispBuff(0, 0, 0, SCREEN_HEIGHT);
	orderingTable[0].length	=OT_LENGTH;
	orderingTable[1].length	=OT_LENGTH;
	orderingTable[0].org		=orderingTable_TAG[0];
	orderingTable[1].org		=orderingTable_TAG[1];
	GsClearOt(0, 0, &orderingTable[0]);
	GsClearOt(0, 0, &orderingTable[1]);
	FntLoad(960, 0);
	FntOpen(-CENTERX + 7, -CENTERY + 15, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 512);
	GsInit3D();
	GsSetProjection(CENTERX);
	GsInitCoordinate2(WORLD, &Camera.coord2);
	GsSetLightMode(0);
}

void clear_vram() {
    RECT rectTL;
    setRECT(&rectTL, 0, 0, 1024, 512);
    ClearImage2(&rectTL, 0, 0, 0);
    DrawSync(0);
    return;
}

void clearDisplay() {
	myActiveBuff = GsGetActiveBuff();
	GsSetWorkBase((PACKET*)GPUOutputPacket[myActiveBuff]);
	GsClearOt(0, 0, &orderingTable[myActiveBuff]);
}

void display() {
	FntFlush(-1);
	DrawSync(0);
	VSync(0);
	GsSwapDispBuff();
	GsSortClear(BGColor.r, BGColor.g, BGColor.b, &orderingTable[myActiveBuff]);
	GsDrawOt(&orderingTable[myActiveBuff]);
}


void setVideoMode(int mode){
	if(mode == 0){SetVideoMode(1);
	SCREEN_WIDTH = 320;
    SCREEN_HEIGHT = 256;
	}
	else{SetVideoMode(0);
	SCREEN_WIDTH = 320;
    SCREEN_HEIGHT = 240;
	}
}

void SetFogEnabled(int answer){
if(answer == 1){GsSetLightMode(1);}
else           {GsSetLightMode(0);}
}

void SetSunColor (int r, int g, int  b) {
	sunColor.r = r;
	sunColor.g = g;
	sunColor.b = b;
}


void SetSunDirection (int x, int y, int z) {
	sunDirection.vx = x;
	sunDirection.vy = y;
	sunDirection.vz = z;
}

void SetAmbientLight (int r, int g, int b) {
	GsSetAmbient(
r * (ONE/255),
g * (ONE/255),
b * (ONE/255)
	);
}

void CalculateCamera() {
	VECTOR	vec;
	GsVIEW2 view;

	view.view = Camera.coord2.coord;
	view.super = WORLD;

	RotMatrix(&Camera.rotation, &view.view);
	ApplyMatrixLV(&view.view, &Camera.position, &vec);
	TransMatrix(&view.view, &vec);
	GsSetView2(&view);
	sunLight[0].vx = sunDirection.vx;
	sunLight[0].vy = -sunDirection.vy;
	sunLight[0].vz = sunDirection.vz;
	sunLight[0].r =	sunColor.r;	sunLight[0].g = sunColor.g;	sunLight[0].b = sunColor.b;
	GsSetFlatLight(0, &sunLight[0]);
}

void RenderObject(VECTOR pos, SVECTOR rot, GsDOBJ2 *obj) {
	MATRIX lmtx,omtx;
	GsCOORDINATE2 coord;
	coord = Camera.coord2;
	pos.vy *= -1;
	RotMatrix(&rot, &omtx);
	TransMatrix(&omtx, &pos);
	CompMatrixLV(&Camera.coord2.coord, &omtx, &coord.coord);
	coord.flg = 0;
	obj->coord2 = &coord;
	GsGetLws(obj->coord2, &lmtx, &omtx);
	GsSetLightMatrix(&lmtx);
	GsSetLsMatrix(&omtx);
	GsSortObject4(obj, &orderingTable[myActiveBuff], 14-OT_LENGTH, getScratchAddr(0));
}


int LoadTMD(u_long *tmd, GsDOBJ2 *obj, int enableLighting) {
	u_long *dop;
	int i,NumObj;
	dop = tmd;
	dop++;
	GsMapModelingData(dop);
	dop++;
	NumObj = *dop;
	dop++;
	for(i=0; i<NumObj; i++) {
		GsLinkObject4((u_long)dop, &obj[i], i);
		if (enableLighting == 0) {
			obj[i].attribute = (1<<6);
		}
	}
	return(NumObj);
}

void loadTexture(unsigned char imageData[]) {
	GsIMAGE* tim_data;
	RECT* rect;
	RECT* crect;
	tim_data = malloc3(sizeof(GsIMAGE));
	GsGetTimInfo ((u_long *)(imageData+4),tim_data);
	rect = malloc3(sizeof(RECT));
	crect = malloc3(sizeof(RECT));
	rect->x = tim_data->px;
	rect->y = tim_data->py;
	rect->w = tim_data->pw;
	rect->h = tim_data->ph;
	printf("Framebuffer info {x=%d, y=%d, w=%d, h=%d}\n", rect->x, rect->y, rect->w, rect->h);
	LoadImage(rect, tim_data->pixel);
	crect->x = tim_data->cx;
	crect->y = tim_data->cy;
	crect->w = tim_data->cw;
	crect->h = tim_data->ch;
	printf("CLUT info {x=%d, y=%d, w=%d, h=%d}\n", crect->x, crect->y, crect->w, crect->h);
	LoadImage(crect, tim_data->clut);
	free3(rect);
	free3(crect);
	free3(tim_data);
}

VECTOR pos;
MATRIX tempMatrixPos;
VECTOR returnPos;
VECTOR Translate (SVECTOR rot, int x, int y, int z) {

	pos.vx = x;
	pos.vy = y;
	pos.vz = z;

    RotMatrix(&rot, &tempMatrixPos);

	ApplyMatrixLV(&tempMatrixPos, &pos, &returnPos);

	return returnPos;

}

int getPositionX(Mesh object){
	return object.position.vx;
}

int getPositionY(Mesh object){
	return object.position.vy;
}

int getPositionZ(Mesh object){
	return object.position.vz;
}

void StartAmbientLight(){
    SetAmbientLight(67, 67, 67);
    SetSunColor(255, 255, 255);
    SetSunDirection(0, -1, 1);
}
