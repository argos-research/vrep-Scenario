CFLAGS = -I./remoteApi -I./include -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255 -D__linux

all: 
	g++ $(CFLAGS) -c client.cpp -o client.o
	gcc $(CFLAGS) -c ./remoteApi/extApi.c -o extApi.o
	gcc $(CFLAGS) -c ./remoteApi/extApiPlatform.c -o extApiPlatform.o
	g++ extApi.o extApiPlatform.o client.o -o ../../client -lpthread
