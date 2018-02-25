TARGET = vrep
SRC_C = remoteApi/extApi.c remoteApi/extApiPlatform.c
SRC_CC = main.cc
LIBS = base stdcxx
INC_DIR += $(PRG_DIR)/include
INC_DIR += $(PRG_DIR)/remoteApi
CC_OPT = -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255 -D__linux