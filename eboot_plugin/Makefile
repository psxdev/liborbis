ifndef Ps4Sdk
ifdef ps4sdk
Ps4Sdk := $(ps4sdk)
endif
ifdef PS4SDK
Ps4Sdk := $(PS4SDK)
endif
ifndef Ps4Sdk
$(error Neither PS4SDK, Ps4Sdk nor ps4sdk set)
endif
endif

keepelf = True
target ?= ps4_elf

#need to add this flags
#-Wl,-T script.ld

###################################

Libraries := -lorbis2d -lorbisPad -lSceVideoOut_stub -lSceSystemService_stub  -lSceUserService_stub -lSceGnmDriver_stub -lScePad_stub

include $(Ps4Sdk)/make/ps4sdk.mk

