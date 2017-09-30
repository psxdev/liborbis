#include <stdint.h>

#include <types/kernel.h>
#include <types/videoout.h>


#define ORBIS2D_DISPLAY_BUFFER_NUM 2 
#define ORBIS2D_FLIP_RATE 0 // 0 none 1 30fps 2 20fps
#define ORBIS2D_FLIP_MODE SCE_VIDEO_OUT_FLIP_MODE_VSYNC 


#define ATTR_WIDTH 1280 
#define ATTR_HEIGHT 720




typedef struct Orbis2dConfig
{
	uint32_t bgColor;
	uint64_t videoMemStackAddr;
	uint32_t videoMemStackSize;
	uint64_t videoMemStackTopAddr;
	uint64_t videoMemStackBaseAddr;
	off_t videoMemOffset;
	int width;
	int pitch; 
	int height;
	int pixelFormat;
	int bytesPerPixel;
	int tilingMode;
	void *surfaceAddr[ORBIS2D_DISPLAY_BUFFER_NUM];
	int64_t flipArgLog[ORBIS2D_DISPLAY_BUFFER_NUM];
	int flipMode;
	int flipRate;
	SceKernelEqueue flipQueue;
	int videoHandle;
	int currentBuffer;
	int orbis2d_initialized;
	
}Orbis2dConfig;

int orbis2dInit();
void orbis2dFinish();
int orbis2dInitWithConf(Orbis2dConfig *conf);
int orbis2dSetConf(Orbis2dConfig *conf);
int orbis2dCreateConf();
Orbis2dConfig *orbis2dGetConf();
void orbis2dSwapBuffers();
void orbis2dClearBuffer();
void orbis2dDrawRectColor(int x, int w, int y, int h, uint32_t color);
void orbis2dWritePixelColor(int x, int y, uint32_t pixelColor);
void orbis2dStartDrawing();
void orbis2dFinishDrawing(int64_t flipArg);
