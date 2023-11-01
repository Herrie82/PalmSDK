rem @echo off

@rem You can use the form "buildit.cmd pre" or "buildit emulator debug" to 
@rem turn on options without modifying the script
set PRE=0
set PIXI=0
set EMULATOR=0
set DEBUG=0

if "%1"=="emulator" ( 
  set EMULATOR=1 
) else (
  if "%1"=="pre" ( 
    set PRE=1 
  ) else (
    if "%1"=="pixi" ( 
      set PIXI=1 
    ) else (
      @rem default is Pixi
      set PIXI=1
    )
  )
)

if "%2"=="debug" ( 
  set DEBUG=1 
)

@rem List your source files here
set SRC=badfont.cpp

@rem List the libraries needed
set LIBS=-lSDL -lSDL_image -lSDL_ttf -lpdl

@rem Name your output executable
set OUTFILE=badfont

if %PRE% equ 0 if %PIXI% equ 0 if %EMULATOR% equ 0 goto :END

if %DEBUG% equ 1 (
   set DEVICEOPTS=-g
) else (
   set DEVICEOPTS=
)

rem defaults for device
set CC=arm-none-linux-gnueabi-gcc
set DEVICELIB="-L%PALMPDK%\device\lib"

if %PRE% equ 1 (
   set DEVICEOPTS=%DEVICEOPTS% -mcpu=cortex-a8 -mfpu=neon -mfloat-abi=softfp
)

if %PIXI% equ 1 (
   set DEVICEOPTS=%DEVICEOPTS% -mcpu=arm1136jf-s -mfpu=vfp -mfloat-abi=softfp
)

if %EMULATOR% equ 1 (
   set CC=i686-pc-linux-gnu-gcc
   set DEVICELIB="-L%PALMPDK%\emulator\lib" -Lemulator\lib
)

echo %CC% %DEVICEOPTS%

%CC% %DEVICEOPTS% -o %OUTFILE% %SRC% "-I%PALMPDK%\include" ^
  "-I%PALMPDK%\include\SDL" %DEVICELIB% -Wl,--allow-shlib-undefined ^
  %LIBS%

goto :EOF

