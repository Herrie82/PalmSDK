/*
*   PDL
*   Copyright 2010-2011 Hewlett-Packard Development Company, L.P. All rights reserved.
*/

#ifndef PDL_TYPES_H_
#define PDL_TYPES_H_

#include <SDL_keysym.h>
#include <SDL_stdinc.h>

typedef SDLKey      PDL_key;
#define PDLKey      PDL_key

typedef SDL_bool    PDL_bool;

#define PDL_FALSE   SDL_FALSE
#define PDL_TRUE    SDL_TRUE

#define CALLBACK_PAYLOAD_MAX 1024

/* Hardware Types */
#define HARDWARE_UNKNOWN  		-1
#define HARDWARE_PRE 			101
#define HARDWARE_PRE_PLUS 		102
#define HARDWARE_PIXI 			201
#define HARDWARE_VEER 			301
#define HARDWARE_PRE_2			401
#define HARDWARE_PRE_3      	501
#define HARDWARE_TOUCHPAD   	601
#define HARDWARE_TOUCHPAD_GO   	701

/*
 * The user event code for a GPS event. You can identify a
 * GPS event with this. The event type will be SDL_USEREVENT.
 * The event union's user.code value will be PDL_GPS_UPDATE. 
 * The event union's user.data1 will be a pointer to a 
 * PDL_Location structure. 
 */
#define PDL_GPS_UPDATE 0xE100  

/* This event is sent when there has been a failure in the GPS system
 * In this case, The event union's user.data1 will be a PDL_Err * 
 * Cast, then dereference to find out the PDL_Err 
 * ex: 
 * PDL_Err err = *(PDL_Err *)Event.user.data; 
 */
#define PDL_GPS_FAILURE 0xE101  

/*
 * The user event code for a compass event. You can identify a
 * compass event with this. The event type will be SDL_USEREVENT.
 * The event union's user.code value will be PDL_COMPASS. 
 * The event union's user.data1 will be a pointer to a 
 * PDL_Compass structure. 
 */
#define PDL_COMPASS 0xE110

/*
 * The user event code that means there are queued downstream
 * JS calls pending. The client should respond to this by calling
 * PDL_HandleJSCalls(). 
 * The event type will be SDL_USEREVENT.
 * The event union's user.code value will be PDL_PENDING_JS.
 * There is no data associated with this event.
 */
#define PDL_PENDING_JS 0xE120


/* The maximum length of the version string in the PDL_OSVersion structure */
#define PDL_VERSION_STR_SIZE 256


/* Remember: Any time you get an error, there will be a plaintext description of what happened
 * available with PDL_GetError()
 */
typedef enum PDL_Err
{
    PDL_NOERROR         = 0,
    PDL_EMEMORY,        /* Memory failure */
    PDL_ECONNECTION,    /* Connection error */
    PDL_INVALIDINPUT,   /* Problem with the sent-in params */
    PDL_EOTHER,         /* Other error */
    PDL_UNINIT,         /* The function relies on a client-called init function that has never been called */
    PDL_NOTALLOWED,     /* A configurable option prevents the operation from succeeding (Ex: Location Services disabled by user = GPS won't work) */
    PDL_LICENSEFAILURE, /* The license of the application is invalid or missing. */
    PDL_STRINGTOOSMALL, /* A sent-in string that was to be filled was too small for the data needed */
    PDL_SYSTEMERROR_FILE, /* A OS data file or database external to PDL is missing or malformed*/
    PDL_SYSTEMERROR_NET,  /* Networking is disabled or unavailable*/
    PDL_APPINFO,      	 /* PDL was unable to find the appinfo.json file, and thus does know know any of 
    					 the information therein, nor does it know the top level of the app's directory tree. */
    PDL_ESTATE			/* The operation you attempted can not be done due to the runtime state of the system you are using */
} PDL_Err;

typedef enum PDL_Orientation
{
	PDL_ORIENTATION_0 = 0,   /* the action button below the screen */
	PDL_ORIENTATION_90,      /* the action button to the left of the screen */
	PDL_ORIENTATION_180,     /* the action button above the screen */
	PDL_ORIENTATION_270     /* the action button to the right of the screen */
} PDL_Orientation;

enum 
{
    PDLK_GESTURE_AREA       = 231,
    PDLK_GESTURE_BACK       = 27,
    PDLK_GESTURE_FORWARD    = 229,
    PDLK_GESTURE_DISMISS_KEYBOARD    = 24
};

typedef enum PDL_TouchAggression
{
	PDL_AGGRESSION_LESSTOUCHES = 0,  /* LESS likely to see multi-touches. MORE likely to see single-touches. This is the default. */
	PDL_AGGRESSION_MORETOUCHES       /* MORE likely to see multi-touches. LESS likely to see single-touches. */
} PDL_TouchAggression;

typedef enum PDL_OGLVersion
{
	PDL_OGL_1_1 = 0,
	PDL_OGL_2_0
} PDL_OGLVersion;

/* Struct used to store information about a particular interface
 * On Windows, only IP address is populated 
 */
struct _PDL_NetInfo
{
	Uint32 ipaddress;
	Uint32 netmask;
	Uint32 broadcast;
};
typedef struct _PDL_NetInfo PDL_NetInfo;

/* For backward compatibility. */
#define _NETinfo	_PDL_NetInfo
#define NETinfo 	PDL_NetInfo

/* struct used for latitude and longitude information */
typedef struct PDL_Location
{
	/* location on the Earth. -1,-1 if it is unknown. */ 
	double latitude;
	double longitude;
	
	/* the altitude in meters. -1 if unknown */
	double altitude; 
	
	/* the accuracy of the location, in meters. will be -1 if it is unknown */
	double horizontalAccuracy;
	double verticalAccuracy;
	
	/* the compass direction of travel. -1 if it is unknown */
	double heading;
	
	/* the travel velocity in meters per second -1 if it is unknown */
	double velocity;
} PDL_Location;

typedef struct PDL_Compass
{
	int confidence;
	double magneticBearing;
	double trueBearing;
} PDL_Compass;

typedef struct PDL_ScreenMetrics
{
	int horizontalPixels; /* number of pixels horizontally */
	int verticalPixels; /* number of pixels, vertically */
	int horizontalDPI; /* number pf horizontal pixels in one inch */
	int verticalDPI; /* number of vertical pixels in one inch */
	double aspectRatio; /* the aspect ratio (horizontalDPI/verticalDPI) */
} PDL_ScreenMetrics;

typedef struct PDL_OSVersion
{
	int majorVersion; /* the major version. If the version is "1.4.5", this value will be 1. */
	int minorVersion; /* the minor version. If the version is "1.4.5", this value will be 4. */
	int revision;     /* the revision. If the version is "1.4.5", this value will be 5. */
	char versionStr[PDL_VERSION_STR_SIZE];
} PDL_OSVersion;

/* the internally managed PDK_Parameters type, which */
/* will be sent along to all service callbacks */
typedef struct PDL_ServiceParameters PDL_ServiceParameters;

/* the internally managed PDL_JSParameters type, which */
/* will be sent along to all JS callbacks */
typedef struct PDL_JSParameters PDL_JSParameters;

/* the PDL_ServiceCallbackFunc calling convention. Any callbacks sent to PDL_ServiceCall */
/* will be in this format. reply will be the reply string. user will be the */
/* user-defined pointer that was sent to PDL_ServiceCall */
typedef PDL_bool (*PDL_ServiceCallbackFunc) (PDL_ServiceParameters *params, void *user);

/* the function prototype for JS handlers.  */
typedef PDL_bool (*PDL_JSHandlerFunc) (PDL_JSParameters *params);

/* used by purchases */
typedef struct PDL_ItemInfo PDL_ItemInfo;
typedef struct PDL_ItemReceipt PDL_ItemReceipt;
typedef struct PDL_ItemCollection PDL_ItemCollection;

#endif /* PDL_TYPES_H_ */
