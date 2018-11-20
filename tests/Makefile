CC = $(NDK_PATH)/clang
LD = $(NDK_PATH)/clang

########## YOUR MAKEFILE BELOW ##########
INCLUDE = ./include
LDLIBS = ./lib
CFLAGS = -Wall -Werror -g -O2 -I$(INCLUDE)
LDFLAGS = -L$(LDLIBS)

all: face

face: face.o
	$(LD) $^ -fPIE -o $@ $(LDFLAGS)


face.o: face.c face.h
	$(CC) -fPIE -c $(CFLAGS) $<

.PHONY: clean

clean:
	rm -f *.o faceup facedown
