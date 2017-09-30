#include <stdio.h>
#include <stdlib.h>

#include <kernel.h>
#include <systemservice.h>
#include <logdebug.h>
#include <orbis2d.h>
#include <orbisPad.h>

int x=1280/2;
int y=720/2;
int w=1280/64;
int h=1280/64;
int step=10;


int64_t flipArg=0;
int R,G,B;
uint32_t color=0x80ff0000;
int flag=1;

Orbis2dConfig *conf;

void updateController()
{
	int ret;
	ret=orbisPadUpdate();
	if(ret==0)
	{
		
		if(orbisPadGetButton(SCE_PAD_BUTTON_UP))
		{
			if(y-step>=0)
			{
				y=y-step;
			}
			else
			{
				y=0;
			}
		}
		if(orbisPadGetButton(SCE_PAD_BUTTON_DOWN))
		{
			if(y+step<conf->height-1)
			{
				y=y+step;
			}
			else
			{
				y=conf->height-1-step;
			}
		}						
		if(orbisPadGetButton(SCE_PAD_BUTTON_RIGHT))
		{
			if(x+step<conf->width-1)
			{
				x=x+step;
			}
			else
			{
				x=conf->width-1-step;
			}
		}
		if(orbisPadGetButton(SCE_PAD_BUTTON_LEFT))
		{
			if(x-step>=0)
			{
				x=x-step;
			}
			else
			{
				x=0;
			}
		}
		if(orbisPadGetButton(SCE_PAD_BUTTON_TRIANGLE))
		{
			sys_log("Triangle pressed exit\n");
			
			flag=0;
				
		}
		if(orbisPadGetButton(SCE_PAD_BUTTON_CIRCLE))
		{
			sys_log("Circle pressed reset position and color red\n");
			x=1280/2;
			y=720/2;
			color=0x80ff0000;	
		}
		if(orbisPadGetButton(SCE_PAD_BUTTON_CROSS))
		{
			sys_log("Cross pressed rand color\n");
			R=rand()%256;
			G=rand()%256;
			B=rand()%256;
			color=0x80000000|R<<16|G<<8|B;
			
		}
		if(orbisPadGetButton(SCE_PAD_BUTTON_SQUARE))
		{
			sys_log("Square pressed\n");
			
		}
			
	}
}
int main(uint64_t stackbase, uint64_t othervalue) 
{
	int ret;

	
	//hide playroom splash
	sceSystemServiceHideSplashScreen();
	//init pad
	ret=orbisPadInit();
	
	if(ret==1)
	{
		
		ret=orbis2dInit();
	
		if(ret==1)
		{
			conf=orbis2dGetConf();
			while(flag)
			{
				//capture pad data and populate positions
				// X random color
				// O reset to center position and red color
				// /\ to exit
				// dpad move rectangle
				updateController();
				
				
				//wait for current display buffer
				orbis2dStartDrawing();

				// clear with background (default white) to the current display buffer 
				orbis2dClearBuffer();
				
				//default red is here press X to random color
				orbis2dDrawRectColor(x,w,y,h,color);
				
				//flush and flip
				orbis2dFinishDrawing(flipArg);
				
				//swap buffers
				orbis2dSwapBuffers();
				flipArg++;
			}
			
			orbis2dFinish();
			orbisPadFinish();
		}
		
	}	




	exit(0);

	return 0;
}
