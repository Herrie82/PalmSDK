# This is a Windows makefile for use with Microsoft NMake
PRE=0
PIXI=1
DESKTOP=0
DEBUG=0

!if $(DEBUG)
DEVICEOPTS=-g
!else
DEVICEOPTS=
!endif

!if $(PRE)
DEVICEOPTS=$(DEVICEOPTS) -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp
!elseif $(PIXI)
DEVICEOPTS=$(DEVICEOPTS) -mcpu=arm1136jf-s -mfpu=vfp -mfloat-abi=softfp
!else
!error Must set either PRE or PIXI variable to 1 to build
!endif

INC="-I%PalmPDK%\include" "-I%PalmPDK%\include\SDL" 

CC=arm-none-linux-gnueabi-gcc
CFLAGS=$(INC) $(DEVICEOPTS)
CPP=arm-none-linux-gnueabi-g++
CPPFLAGS=$(CFLAGS)
LIBS=-lSDL -lSDL_image -lSDL_ttf -lpdl
LDFLAGS="-L%PalmPDK%\device\lib" $(LIBS) -Wl,--allow-shlib-undefined

.cpp.o::
    $(CPP) $(CPPFLAGS) -c $<

.c.o::
    $(CC) $(CFLAGS) -c $<

badfont: badfont.o
    $(CC) $(LDFLAGS) -o $@ $**

package: badfont appinfo.json logo.png
    -rd /S /Q STAGING
    mkdir STAGING
    copy badfont STAGING
    copy appinfo.json STAGING
    copy logo.png STAGING
    echo filemode.755=badfont > STAGING\package.properties
    palm-package STAGING

clean:
    -rd /S /Q STAGING
    -del badfont
    -del badfont.o
    
all: badfont package
