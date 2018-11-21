CC = $(NDK_PATH)/clang
LD = $(NDK_PATH)/clang

########## YOUR MAKEFILE BELOW ##########
INCLUDE = ./include
LDLIBS = ./lib
CFLAGS = -Wall -Werror -g -O2 -I$(INCLUDE) -fPIE
LDFLAGS = -L$(LDLIBS) -pie -fPIE

all: faceup facedown

faceup: faceup.o
	$(LD) $^ -o $@ $(LDFLAGS)


faceup.o: faceup.c face.h
	$(CC) -c $(CFLAGS) $<

facedown: facedown.o
	$(LD) $^ -o $@ $(LDFLAGS)


facedown.o: facedown.c face.h
	$(CC) -c $(CFLAGS) $<

.PHONY: clean

clean:
	rm -f *.o faceup facedown
