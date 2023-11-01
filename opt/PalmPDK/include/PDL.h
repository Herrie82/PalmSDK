/*
*   PDL
*   Copyright 2010-2011 Hewlett-Packard Development Company, L.P. All rights reserved.
*/

#ifndef PDL_H_
#define PDL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "PDL_types.h"
#include "PDL_Services.h"
#include "PDL_JS.h"
#include "PDL_Purchase.h"
#include "PDL_Sensors.h"
#include "PDL_deprecated.h"


/* Initializes the PDL subsystem. Currently no flags are defined. */
PDL_Err PDL_Init(unsigned int flags);

/* Launches the email browser with a new message ready. It will be
* pre-filled out with the sent-in subject and body. */
PDL_Err PDL_LaunchEmail(const char* Subject, const char* Body);

/* Same as PDL_LaunchEmail, but you can send a recipient list. */
PDL_Err PDL_LaunchEmailTo(const char* Subject, const char* Body, int numRecipients, const char **recipients);

/* Launches the web browser and opens the sent-in URL. */
PDL_Err PDL_LaunchBrowser(const char* Url);

/* Returns the key name of the sent-in key. This is the
 same as SDL_GetKeyName, but includes support for gesture
 area actions. */
const char*   PDL_GetKeyName(PDL_key Key);

/* Returns the language string, as stored in the Luna prefs database.
 Fills the sent-in buffer with the string. Will not write more than
 bufferLen. */
PDL_Err PDL_GetLanguage(char *buffer, int bufferLen);

/* returns the name of the country from the region information
* ex.: "United States" */
PDL_Err PDL_GetRegionCountryName(char *buffer, int bufferLen);

/* returns the country code of the country from the region information
* ex.: "us" */
PDL_Err PDL_GetRegionCountryCode(char *buffer, int bufferLen);

/* Returns information about an interface such as 'eth0'.
* On success, fills in the PDL_NetInfo struct pointed to by
* interfaceInfo. The ipaddress, netmask and broadcast
* will be in network byte order.
* Note: In Windows builds, only interfaceInfo->ipaddress is populated. */
PDL_Err PDL_GetNetInfo(const char *interfaceName, PDL_NetInfo * interfaceInfo);

/* Sets the official orientation of the device. Notifications and banner
* messages displayed by the system will be drawn in this orientation. It is
* recommended that apps call this to let the system know what orientation
* they are drawing themselves in, to make for a more pleasant user experience.
* (otherwise, notifications could come in at a different orientation than the
* app is currently displaying) */
PDL_Err PDL_SetOrientation(PDL_Orientation orientation);

/* Sets the current state of whether banner messages will be displayed
* on top of your app.  Call this function with PDL_FALSE to disable them.
* The default is for them to be enabled. */
PDL_Err PDL_BannerMessagesEnable(PDL_bool Enable);

/* Sets the current state of the quick launch gesture. The default enabled. */
PDL_Err PDL_GesturesEnable(PDL_bool Enable);

/* By default, the system assumes the app provides a custom pause UI.  If disabled,
* the system will display a scrim over your app and require the user to tap a play
* button to resume the application. */
PDL_Err PDL_CustomPauseUiEnable(PDL_bool Enable);

/* Notifies all OS components that you are playing streamed music.
* These components may respond by pausing/resuming their own streams
* (depending on whether you announced that you were
* playing or not playing streamed music). At *present* (with no guarantee
* of this being permanent or honored indefinitely), this will stop any
* media player playback in progress (if you send PDL_TRUE). */
PDL_Err PDL_NotifyMusicPlaying(PDL_bool MusicPlaying);

/* Enables or disabled automatic sound pausing when carded. 
 * When set to PDL_TRUE, sounds will be paused at a system level 
 * (not affecting the pause state apparent to the app) when the app
 * is carded. When set to PDL_FALSE, no special sound related actions
 * are taken when carded. The default value is PDL_TRUE. */
PDL_Err PDL_SetAutomaticSoundPausing(PDL_bool AutomaticallyPause);

/* Opens/closes a port in the firewall. */
PDL_Err PDL_SetFirewallPortStatus(int port, PDL_bool Open);

/* Fills the sent-in buffer with a unique id of the handset (and will
* null-terminate it). The buffer should be at least 64 bytes long
* though this function will not necessarily use all of it.
* NOTE: This function opens and reads a file. You should call it 
* once to get the ID, then cache that yourself. You should not
* call this function in a performance-critical loop.  */
PDL_Err PDL_GetUniqueID(char *buffer, int bufferLen);

/* Fills the sent-in buffer with the name of the handset (and will
* null-terminate it). The buffer should be at least 64 bytes long
* though this function will not necessarily use all of it. */
PDL_Err PDL_GetDeviceName(char *buffer, int bufferLen);

/* Given a file name, get a path where that file can be safely
* stored, which will be segregated from the apps installed
* files. This allows the app to be replaced or removed without
* destroying the data files it generates. The dataFileName value
* can be a relative path, though it will be your responsibility to
* create any subdirectories you want to use (using PDL_GetDataFilePath
* to find out where the make them) */
PDL_Err PDL_GetDataFilePath(const char *dataFileName, char *buffer, int bufferLen);

/* Fills the sent-in buffer with a value from the appinfo.json file.
* This is a key-value system. */
PDL_Err PDL_GetAppinfoValue(const char *name, char *buffer, int bufferLen);

/* Activates/deactivates location tracking on the handset.
* You must call this function before PDL_GetLocation will
* return anything.   */
PDL_Err PDL_EnableLocationTracking(PDL_bool activate);

/* Returns information about the handset's current location.
* Fills the sent-in struct. In the event of an error,
* returns an error code and will leave the sent-in
* struct alone. Before calling this you must activate
* GPS tracking with the function PDL_EnableLocationTracking. */
PDL_Err PDL_GetLocation(PDL_Location *location);

/* Returns PDL_TRUE if this instance is a plugin/component.
* Returns PDL_FALSE if this instance is a standalone application. */
PDL_bool PDL_IsPlugin(void);

/* returns a constant for the current hardware the app is running
 * on. A HARDWARE_XXXX constant (found in PDL_types.h)
 */
int PDL_GetHardwareID(void);

/* fills the sent in PDL_ScreenMetrics struct with the metrics of
* the screen. Note that this is the metrics for the screen hardware,
* not the current application's card or component space. */
PDL_Err PDL_GetScreenMetrics(PDL_ScreenMetrics *outMetrics);

/* In the event of a PDL error, this will return a string with 
* information about the error in it. */
const char *PDL_GetError(void);

/* fills out the sent-in struct with the current version of HP webOS. */
PDL_Err PDL_GetOSVersion(PDL_OSVersion *osVersion);

/* returns an int that is the version of the PDK running on the device. */
int PDL_GetPDKVersion(void);

/* sets the touch aggression to the designated value. */ 
PDL_Err PDL_SetTouchAggression(PDL_TouchAggression aggression);

/* minimize the current app (fullscreen app only. Not for plugins). */ 
PDL_Err PDL_Minimize(void);

/* Vibrate at a given period for a given time */
PDL_Err PDL_Vibrate(int periodMS, int durationMS);

/* Activates/deactivates compass tracking. YOu must activate
 * compass tracking before any calls to PDL_GetCompass. */
PDL_Err PDL_EnableCompass(PDL_bool activate);

/* Get the current compass information */
PDL_Err PDL_GetCompass(PDL_Compass *compass);

/* summon or dismiss the on-screen keyboard for the HP TouchPad */
PDL_Err PDL_SetKeyboardState(PDL_bool bVisible);

/* dynamically open a specific OpenGL version. */
PDL_Err PDL_LoadOGL(PDL_OGLVersion version);

/* Logs to the file /media/internal/tracking.txt under the category "pdl_log" */
void PDL_Log(const char *format, ...);

/* shuts down PDL  */
void    PDL_Quit(void);

#ifdef __cplusplus
}
#endif

#endif /* PDL_H_ */
