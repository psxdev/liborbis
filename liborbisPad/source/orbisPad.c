#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <userservice.h>
#include <pad.h>
#include "orbisPad.h"
#include "logdebug.h"

OrbisPadConfig *orbisPadConf=NULL;

int orbispad_external_conf=-1;

void orbisPadFinish()
{
	int ret;
	if(orbispad_external_conf!=1)
	{
		if(orbisPadConf->orbispad_initialized==1)
		{		
			ret=scePadClose(orbisPadConf->padHandle);
		
			sys_log("liborbis2d scePadClose return 0x%8x\n",ret);
		}
		orbisPadConf->orbispad_initialized=-1;
	
		sys_log("liborbisPad finished");
	}
}

OrbisPadConfig *orbisPadGetConf()
{
	if(orbisPadConf)
	{
		return orbisPadConf;
	}
	
	return NULL; 
}
int orbisPadCreateConf()
{	
	if(!orbisPadConf)
	{
		orbisPadConf=(OrbisPadConfig *)malloc(sizeof(OrbisPadConfig));
		
		orbisPadConf->userId=0;
		orbisPadConf->padHandle=-1;
		orbisPadConf->padData=(ScePadData *)malloc(sizeof(ScePadData));
		orbisPadConf->orbispad_initialized=-1;
		
		return 0;
	}
	
	if(orbisPadConf->orbispad_initialized==1)
	{
		return orbisPadConf->orbispad_initialized;
	}
	//something weird happened
	return -1;			
}
int orbisPadSetConf(OrbisPadConfig *conf)
{
	if(conf)
	{
		orbisPadConf=conf;
		orbispad_external_conf=1;
		return orbisPadConf->orbispad_initialized;
	}
	
	return 0; 
}
int orbisPadInitWithConf(OrbisPadConfig *conf)
{
	int ret;
	ret=orbisPadSetConf(conf);
	if(ret)
	{
		sys_log("liborbisPad already initialized using configuration external\n");
		sys_log("orbispad_initialized=%d\n",orbisPadConf->orbispad_initialized);
		sys_log("ready to have a lot of fun...\n");
		return orbisPadConf->orbispad_initialized;
	}
	else
	{
		return 0;
	}
}
bool orbisPadGetButton(unsigned int filter)
{
	return (orbisPadConf->padData->buttons&filter ? 1 : 0);
}

int orbisPadUpdate()
{
	int ret;
	ret=scePadReadState(orbisPadConf->padHandle,orbisPadConf->padData);
	if(ret==0 && orbisPadConf->padData->connected==1)
	{
		return 0;
		
	}
	else
	{
		return -1;
	}
}
int orbisPadInit()
{
    int ret;
   
    int param=700;
	
	if(orbisPadCreateConf()==1)
	{
			return orbisPadConf->orbispad_initialized;
	}
	if (orbisPadConf->orbispad_initialized==1) 
	{
		sys_log("liborbispad is already initialized!\n");
		return orbisPadConf->orbispad_initialized;
	}
	ret=sceUserServiceInitialize(&param);
	
       
    if(ret==0)
    {
        ret=scePadInit();
        if(ret<0)
        {
            sys_log("liborbispad scePadInit return error 0x%8x\n",ret);
            return -1;
        }
        sys_log("liborbispad scePadInit return %d\n",ret);
        if(ret==0)
        {
           
            ret=sceUserServiceGetInitialUser(&orbisPadConf->userId);
			if(ret<0)
			{
	            sys_log("liborbispad sceUserServiceGetInitialUser return error 0x%8x\n",ret);
	            return -1;
				
			}
            orbisPadConf->padHandle=scePadOpen(orbisPadConf->userId, 0, 0, NULL);
            if(orbisPadConf->padHandle<0)
            {
                sys_log("liborbispad scePadOpen return error 0x%8x\n",orbisPadConf->padHandle);
                return -1;
            }
            sys_log("liborbispad scePadOpen return handle 0x%8x\n",orbisPadConf->padHandle);
            if(orbisPadConf->padHandle>0)
            {
                    orbisPadConf->orbispad_initialized=1;
                
            }

        }
    }
   
    return orbisPadConf->orbispad_initialized;
}
