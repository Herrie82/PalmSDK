/*
*   PDL
*   Copyright 2010-2011 Hewlett-Packard Development Company, L.P. All rights reserved.
*/

#ifndef __PDLSERVICES__
#define __PDLSERVICES__

#ifdef __cplusplus
extern "C" {
#endif

#include "PDL_types.h"

/************************* SERVICE CALLS *******************************/
/************************* SERVICE CALLS *******************************/
/************************* SERVICE CALLS *******************************/
	
/* sends a services call through the public bus
* uri - the component to sent the payload to
* payload - the payload to send
* callback - the function to be called with a response (NULL if you don't care about a response)
* user - a pointer that will be sent to the callback (can be NULL, ignored if callback is NULL)
* removeAfterResponse - when true the system will remove the callback from memory after 1 response. 
*                       When false, it will keep it around until PDL_UnregisterCallback is called */   
PDL_Err PDL_ServiceCall(const char *uri, const char* payload);
PDL_Err PDL_ServiceCallWithCallback(const char *uri, const char *payload, PDL_ServiceCallbackFunc callback, void *user, PDL_bool removeAfterResponse);
PDL_Err PDL_UnregisterServiceCallback(PDL_ServiceCallbackFunc callback); /* call to stop recieving messages to this callback */

/* when your callback gets called, you will have a PDL_ServiceParameters
* pointer. These functions extract data from it. Each will return a default
* value if the parameter is not there. */ 
PDL_bool 	PDL_ParamExists(PDL_ServiceParameters *parms, const char *name); /* returns true if that parameter is in the list */
void 		PDL_GetParamString(PDL_ServiceParameters *parms, const char *name, char *buffer, int bufferLen); /* fills the buffer. fills with "" if the parameter doesn't exist */
int         PDL_GetParamInt(PDL_ServiceParameters *parms, const char *name); /* returns 0 of the parameter doesn't exist */
double      PDL_GetParamDouble(PDL_ServiceParameters *parms, const char *name); /* returns 0.0 if the parameter doesn't exist */
PDL_bool 	PDL_GetParamBool(PDL_ServiceParameters *parms, const char *name); /* returns PDL_FALSE if the parameter doesn't exist */
const char *PDL_GetParamJson(PDL_ServiceParameters *parms); /* returns the entire json formatted string that the service call responded with. String is only valid during the callback.*/


#ifdef __cplusplus
}
#endif

#endif 


