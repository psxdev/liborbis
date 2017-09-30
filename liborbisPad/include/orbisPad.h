#include <stdint.h>
#include <stdbool.h>

#include <types/kernel.h>
#include <types/userservice.h>
#include <types/pad.h>

typedef struct OrbisPadConfig
{
	SceUserServiceUserId userId;
	ScePadData *padData;
	int padHandle;
	int orbispad_initialized;
	
}OrbisPadConfig;

int orbisPadInit();
void orbisPadFinish();
OrbisPadConfig *orbisPadGetConf();
int orbisPadCreateConf();
int orbisPadSetConf(OrbisPadConfig *conf);
int orbisPadInitWithConf(OrbisPadConfig *conf);
bool orbisPadGetButton(unsigned int filter);
int orbisPadUpdate();
