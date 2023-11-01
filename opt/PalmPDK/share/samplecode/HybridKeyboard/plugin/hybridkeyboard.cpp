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
TTF_Font *gFont = NULL;

char *keyString = NULL;

#define PRELUDE_FONT_PATH "/usr/share/fonts/Prelude-Medium.ttf"

#define EXIT(n) do { fprintf(stderr, "Early exit at %s:%d\n", __FILE__, __LINE__); exit(n); } while (0)

#define EVENT_LOGGING
#ifdef EVENT_LOGGING
#include <syslog.h>
#define SYSLOG(...) syslog(__VA_ARGS__)
#else
#define SYSLOG(...) do { } while(0)
#endif

static void freeFont()
{
    TTF_CloseFont(gFont);
}

static void setup()
{
	SYSLOG(LOG_ERR, "***** plugin setup");

    PDL_Init(0);
    atexit(PDL_Quit);

    SDL_Init(SDL_INIT_VIDEO);
    atexit(SDL_Quit);

    PDL_Err err;
    char appPath[256];
    err = PDL_GetCallingPath(appPath, sizeof(appPath));
    if (err) EXIT(1);
    chdir(appPath);
    
	SDL_EnableUNICODE(1);
	keyString = strdup("");

    gScreen = SDL_SetVideoMode(0, 0, 0, 0);
	
    TTF_Init();
    atexit(TTF_Quit);

    gFont = TTF_OpenFont(PRELUDE_FONT_PATH, 20);
    if (!gFont) EXIT(1);
    atexit(freeFont);
}

static void display()
{
	SYSLOG(LOG_ERR, "***** plugin display");

    int black = SDL_MapRGB(gScreen->format, 0, 0, 0);
    SDL_Rect screenRect = { 0, 0, gScreen->w, gScreen->h };
    SDL_FillRect(gScreen, &screenRect, black);
    
    SDL_Color yellow = { 255, 255, 0 };
    SDL_Surface *textSurface = TTF_RenderText_Blended(gFont, "Text Entry:", yellow);
    SDL_Rect textDestRect;
    if (textSurface)
    {
        textDestRect.x = (gScreen->w - textSurface->w) / 2;
        textDestRect.y = ((gScreen->h / 2) - textSurface->h - 2);
        textDestRect.w = textSurface->w;
        textDestRect.h = textSurface->h;
        SDL_BlitSurface(textSurface, NULL, gScreen, &textDestRect);
        SDL_FreeSurface(textSurface);
    }
    textSurface = TTF_RenderText_Blended(gFont, keyString, yellow);

    if (textSurface)
    {
        textDestRect.x = (gScreen->w - textSurface->w) / 2;
		textDestRect.y = textDestRect.y + textDestRect.h + 4;
        textDestRect.w = textSurface->w;
        textDestRect.h = textSurface->h;
        SDL_BlitSurface(textSurface, NULL, gScreen, &textDestRect);
        SDL_FreeSurface(textSurface);
    }
    
    SDL_Flip(gScreen);
}

static void loop()
{
    SDL_Event event;
    if (SDL_WaitEvent(&event)) {
		SYSLOG(LOG_ERR, "***** plugin event");
        if (event.type == SDL_QUIT) {
            exit(0);
		}
        else if (event.type == SDL_VIDEOEXPOSE) {
            display();
		}
        else if (event.type == SDL_VIDEORESIZE) {
//			gScreen = SDL_SetVideoMode(event.resize.w, event.resize.h, 0, 0);
            display();
		}
		else if (event.type == SDL_KEYUP) {
			free(keyString);
			asprintf(&keyString, "SDL_KEYDOWN: %s, %d", SDL_GetKeyName(event.key.keysym.sym), event.key.keysym.unicode);
			printf("%s\n", keyString);
			display();
        }
    }
}

int main(int argc, char **argv)
{
	setup();
    display();
    while (1)
        loop();
}
