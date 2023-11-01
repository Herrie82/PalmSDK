/**
    Palm disclaimer
**/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "PDL.h"

SDL_Surface *gScreen = NULL;
SDL_Surface *gLogo = NULL;
TTF_Font *gFont = NULL;

PDL_Orientation orientation = PDL_ORIENTATION_0;
char *keyString = NULL;

#define PRELUDE_FONT_PATH "/usr/share/fonts/Prelude-Medium.ttf"

#define EXIT(n) do { fprintf(stderr, "Early exit at %s:%d\n", __FILE__, __LINE__); exit(n); } while (0)

static void freeLogo()
{
    SDL_FreeSurface(gLogo);
}

static void freeFont()
{
    TTF_CloseFont(gFont);
}

static void setup()
{
    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);
    
    PDL_Init(0);
    atexit(PDL_Quit);

    PDL_Err err;
    char appPath[256];
    err = PDL_GetCallingPath(appPath, sizeof(appPath));
    if (err) EXIT(1);
    chdir(appPath);

    char orientationStr[10] = "";
    err = PDL_GetAppinfoValue("badFontOrientation", orientationStr, 10);
    if (!err) {
        if (strcmp("0", orientationStr) == 0) orientation = PDL_ORIENTATION_0;
        else if (strcmp("90", orientationStr) == 0) orientation = PDL_ORIENTATION_90;
        else if (strcmp("180", orientationStr) == 0) orientation = PDL_ORIENTATION_180;
        else if (strcmp("270", orientationStr) == 0) orientation = PDL_ORIENTATION_270;
    }
    
    PDL_SetOrientation(orientation);

    SDL_EnableUNICODE(1);
    keyString = strdup("");

    gScreen = SDL_SetVideoMode(0, 0, 0, 0);
    // for 90/270 rotation modes, reverse original width/height.
	// This only works in SDL mode.  OpenGL mode on TouchPad fails at 768x1024.
    if (orientation == PDL_ORIENTATION_90 || orientation == PDL_ORIENTATION_270) {
        gScreen = SDL_SetVideoMode(gScreen->h, gScreen->w, 0, 0);
    }
    
    
    SDL_Surface *logoImage = IMG_Load("logo.png");
    if (!logoImage) EXIT(1);
    gLogo = SDL_DisplayFormat(logoImage);
    if (!gLogo) EXIT(1);
    SDL_FreeSurface(logoImage);
    atexit(freeLogo);

    
    TTF_Init();
    atexit(TTF_Quit);

    gFont = TTF_OpenFont(PRELUDE_FONT_PATH, 20);
    if (!gFont) EXIT(1);
    atexit(freeFont);
}

static void display()
{
    int black = SDL_MapRGB(gScreen->format, 0, 0, 0);
    SDL_Rect screenRect = { 0, 0, gScreen->w, gScreen->h };
    SDL_FillRect(gScreen, &screenRect, black);
    
    SDL_Rect logoDestRect;
    logoDestRect.x = (gScreen->w - gLogo->w) / 2;
    logoDestRect.y = ((gScreen->h - gLogo->h) / 2) - 8;
    logoDestRect.w = gLogo->w;
    logoDestRect.h = gLogo->h;
    SDL_BlitSurface(gLogo, NULL, gScreen, &logoDestRect);

    SDL_Color yellow = { 255, 255, 0 };
    SDL_Surface *textSurface = TTF_RenderText_Blended(gFont, "Comic Sans Forever", yellow);
    if (textSurface)
    {
        // now center it horizontally and position it just below the logo
        SDL_Rect textDestRect;
        textDestRect.x = (gScreen->w - textSurface->w) / 2;
        textDestRect.y = (logoDestRect.y + logoDestRect.h + 8);
        textDestRect.w = textSurface->w;
        textDestRect.h = textSurface->h;
        SDL_BlitSurface(textSurface, NULL, gScreen, &textDestRect);
        SDL_FreeSurface(textSurface);
    }
    textSurface = TTF_RenderText_Blended(gFont, keyString, yellow);
    if (textSurface)
    {
        // now center it horizontally and position it just below the logo
        SDL_Rect textDestRect;
        textDestRect.x = (gScreen->w - textSurface->w) / 2;
        textDestRect.y = (logoDestRect.y - 30);
        textDestRect.w = textSurface->w;
        textDestRect.h = textSurface->h;
        SDL_BlitSurface(textSurface, NULL, gScreen, &textDestRect);
        SDL_FreeSurface(textSurface);
    }
    
    SDL_Flip(gScreen);
}

bool gKeyboardVisible = false;

static void loop()
{
    SDL_Event event;
    if (SDL_WaitEvent(&event)) {
        if (event.type == SDL_QUIT)
            exit(0);
        else if (event.type == SDL_VIDEOEXPOSE)
            display();
        else if (event.type == SDL_MOUSEBUTTONDOWN) {
           if (PDL_GetPDKVersion() >= 300) {
                gKeyboardVisible = !gKeyboardVisible;
                PDL_SetKeyboardState(gKeyboardVisible ? PDL_TRUE : PDL_FALSE);
           }
        }
        else if (event.type == SDL_KEYUP) {
            if (event.key.keysym.unicode == PDLK_GESTURE_DISMISS_KEYBOARD) {
               if (PDL_GetPDKVersion() >= 300) {
                    gKeyboardVisible = false;
                    PDL_SetKeyboardState(PDL_FALSE);
               }
            }
            free(keyString);
            asprintf(&keyString, "SDL_KEYDOWN: '%s' (U+%.4X)", 
				PDL_GetKeyName(event.key.keysym.sym), event.key.keysym.unicode);
            printf("%s\n", keyString);
            display();
        }
    }
}

int main(int argc, char **argv)
{
    if (argc > 1) {
        if (strcmp("-0", argv[1]) == 0) orientation = PDL_ORIENTATION_0;
        else if (strcmp("-90", argv[1]) == 0) orientation = PDL_ORIENTATION_90;
        else if (strcmp("-180", argv[1]) == 0) orientation = PDL_ORIENTATION_180;
        else if (strcmp("-270", argv[1]) == 0) orientation = PDL_ORIENTATION_270;
    }
    
    setup();
    display();
    while (1)
        loop();
}
