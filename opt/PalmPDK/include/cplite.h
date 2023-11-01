/* ============================================================
 * Date  : 2010-2-1
 * Author: Brian Hernacki (brian.hernacki@palm.com)
 * Copyright 2010 Palm, Inc. All rights reserved.
 * ============================================================ */

#ifndef CPLITE_HEADER_INCLUDED
#define CPLITE_HEADER_INCLUDED 1


#ifdef __cplusplus
extern "C" {
#endif

/*
*
*  PDL_isAppLicensedForDevice allows the caller to pass in an appID and verify if the
* local device is licensed for it. The intent is for the application to validate their
* own appID. The function returns 0 to indicate that the device is NOT licensed for the
* app or that it could not determine if it is was. It returns 1 if the device is licensed
* for the device. 
*
* Note that the device must be able to connect to Palm services at least the first time
* in order to determine license status. Subsequent calls do not require the service be
* reachable except in failure recovery scenarios. It is up to the caller to determine
* how to handle failures. Applications can query the service interface available at
* palm://com.palm.connectionmanager/getStatus to determine network avilability.
*
*/
extern inline int PDL_isAppLicensedForDevice(const char *appId) __attribute__((always_inline));

#ifdef __cplusplus
}
#endif

#endif
