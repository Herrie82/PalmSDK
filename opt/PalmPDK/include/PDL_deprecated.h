/*
*   PDL
*   Copyright 2010-2011 Hewlett-Packard Development Company, L.P. All rights reserved.
*/

#ifndef PDL_DEPRECATED_H_
#define PDL_DEPRECATED_H_

/* Use PDL_GetNetInfo instead. */
PDL_Err PDLNet_Get_Info(const char * _interface, NETinfo * interfaceInfo);

/* Deprecated!  Use cplite.h instead. */
PDL_Err PDL_CheckLicense(void);

/* Deprecated!  Screen timeout is now disabled by default. */
PDL_Err PDL_ScreenTimeoutEnable(PDL_bool Enable);

/* deprecated. Used to return the full path to the current executable
 * now just returns "./" */
PDL_Err PDL_GetCallingPath(char *buffer, int bufferLen);

/* DEPRECATED (Use PDL_GetHardwareID)
 * Returns a string for the current hardware. Will return "pre", "pixi"
 * or "unknown". Included for backward compatibility only. Can't identify
 * devices released after the Palm Pixi. */
const char *PDL_GetHardware(void);

#endif /* PDL_DEPRECATED_H_ */
