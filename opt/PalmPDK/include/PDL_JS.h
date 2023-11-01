/*
*   PDL
*   Copyright 2010-2011 Hewlett-Packard Development Company, L.P. All rights reserved.
*/

#ifndef __PDL_JS__
#define __PDL_JS__

#ifdef __cplusplus
extern "C" {
#endif

#include "SDL.h"
#include "PDL_types.h"

/************************* JS COMMUNICATION *******************************/
/************************* JS COMMUNICATION *******************************/
/************************* JS COMMUNICATION *******************************/

/* Register a callback to be a handler for a JS function. For instance, you could 
* call PDL_RegisterJSHandler("foo", MyFunc). In the JS script, you could
* get the native element in the list and call element.foo(), which would lead to the registered
* function being called. */
PDL_Err PDL_RegisterJSHandler(const char *functionName, PDL_JSHandlerFunc function);

/* Same as PDL_RegisterJSHandler, but instead of the js handler being called immediately,
 * the request will be put in a queue, and an event will be posted to SDL to alert you. You 
 * must call PDL_HandleJSCalls to make the actual handlers get called. This approach
 * ensures the handlers are called in your thread (as opposed to PDL_RegisterJSHandler, which
 * calls them immediately from an internal PDK thread). 
 * The event you get is SDL_USEREVENT, The event's user.code value will be PDL_PENDING_JS.
 */
PDL_Err PDL_RegisterPollingJSHandler(const char *functionName, PDL_JSHandlerFunc function);

/* call this after all JS handlers have been registered. Until this function is
* called, NONE of the handlers will work. After this function is called, you may
* not specify any further handlers. */
PDL_Err PDL_JSRegistrationComplete(void);

/* This will call the handlers for any pending downstream js calls
 * that are in the queue. See PDL_RegisterPollingJSHandler's comments
 * for more information. 
 * Returns the number of calls handled, or -1 if there was an error. 
 */
int PDL_HandleJSCalls(void);

/* some helper functions for the JS parameters */
const char *PDL_GetJSFunctionName(PDL_JSParameters *parms); /* returns the function name that was called from the JS side which caused this handler to be called */
PDL_bool PDL_IsPoller(PDL_JSParameters *parms); /* returns true if the params are being handled by a polling handler. False if they are being handled immediately.*/
int PDL_GetNumJSParams(PDL_JSParameters *parms); /* returns the number of parameters sent */
const char *PDL_GetJSParamString(PDL_JSParameters *parms, int paramNum); /* no allocations. Just returns an internal pointer. Returns "" on failure */
int PDL_GetJSParamInt(PDL_JSParameters *parms, int paramNum); /* returns 0 on failure */
double PDL_GetJSParamDouble(PDL_JSParameters *parms, int paramNum); /* returns 0.0 on failure */

/* call this function to specify your reply to the JS call. It makes a copy
* of the sent-in string, so you don't have to maintain it. */
PDL_Err PDL_JSReply(PDL_JSParameters *parms, const char *reply);

/* same as a reply, but for an exception. This will cause the JavaScript
* function that is being called to throw a JavaScript exception with the
* reply string  */
PDL_Err PDL_JSException(PDL_JSParameters *parms, const char *reply);

/* call this function to proactively call JS functionality from the native side.
* functionName: the name of the function to be called on the JS side. Register this in your JS
*               app by calling: $('MyPlugin').funcName = this.funcName.bind(this); from within the setup
*               function of whatever class you want to handle the call. Replace "MyPlugin" with the name
*               you gave your plugin, and "funcName" with the function name you want to use when calling
*               from the app. 
* params: An array of const char * at least numParams long, containing the parameters to send along as c-strings
* numParams: The number of parameters to be sent to the function */
PDL_Err PDL_CallJS(const char *functionName, const char **params, int numParams);

/* Call this to go fullscreen from beign a plugin. It will set up a direct rendering channel
 * for optimal performance, and you will have total control over the screen. In addition
 * to managing internals, this function will call SDL_SetVideoMode with the parameters you sent
 * in, and will return the surface generated. That will be your new screen, the previous one
 * will be invalid. */
SDL_Surface *PDL_SetFullscreen(int width, int height, int bpp, Uint32 flags);

/* Call this any time after you have called PDL_SetFullscreen. It will return the plugin
 * from fullscreen mode back to beign a component on the web page. It will cause a SDL_VIDEORESIZE
 * event to be added to the queue. You will need to handle that resize like you would any other. 
 * You will not be able to draw to the plugin until you have called SDL_SetVideoMode ion response to the
 * resize. */
PDL_Err PDL_DismissFullscreen(void);

/* returns PDL_TRUE if it is a plugin that has fullscreened (not if it's jsut a fullscreen native app). */
PDL_bool PDL_IsFullscreenPlugin(void); 


#ifdef __cplusplus
}
#endif

#endif 
