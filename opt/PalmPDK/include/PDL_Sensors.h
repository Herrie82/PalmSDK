/*
*   PDL
*   Copyright 2011-2012 Hewlett-Packard Development Company, L.P. All rights reserved.
*/

#ifndef __PDLSENSORS_
#define __PDLSENSORS_

#include <SDL_stdinc.h>
#include "PDL_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************** CONSTANTS *********************/
/***************** CONSTANTS *********************/
/***************** CONSTANTS *********************/
/* Sensor types */
typedef Sint32 PDL_SensorType; 

#define PDL_SENSOR_NONE 				(-1)
#define PDL_SENSOR_ACCELEROMETER		0
#define PDL_SENSOR_COMPASS				1
#define PDL_SENSOR_ALS					2
#define PDL_SENSOR_ANGULAR_VELOCITY		3
#define PDL_SENSOR_GRAVITY				4
#define PDL_SENSOR_LINEAR_ACCELERATION	5
#define PDL_SENSOR_MAGNETIC_FIELD		6
#define PDL_SENSOR_ORIENTATION			7
#define PDL_SENSOR_PROXIMITY			8
#define PDL_SENSOR_ROTATION				9
#define PDL_SENSOR_SHAKE				10

// return values for PDL_SENSOR_ORIENTATION
#define PDL_SENSOR_ORIENTATION_UNKNOWN          0
#define PDL_SENSOR_ORIENTATION_FACE_UP          1
#define PDL_SENSOR_ORIENTATION_FACE_DOWN        2
#define PDL_SENSOR_ORIENTATION_NORMAL           3
#define PDL_SENSOR_ORIENTATION_UP_SIDE_DOWN     4
#define PDL_SENSOR_ORIENTATION_LEFT_SIDE_DOWN   5
#define PDL_SENSOR_ORIENTATION_RIGHT_SIDE_DOWN  6

// return values for PDL_SENSOR_SHAKE
#define PDL_SENSOR_SHAKE_NOT_SHAKING    0
#define PDL_SENSOR_SHAKE_STARTED        1
#define PDL_SENSOR_SHAKE_CONTINUING     2
#define PDL_SENSOR_SHAKE_STOPPED        3

/***************** INDIVIDUAL EVENTS USED BY PDL_SENSORS *********************/
/***************** INDIVIDUAL EVENTS USED BY PDL_SENSORS *********************/
/***************** INDIVIDUAL EVENTS USED BY PDL_SENSORS *********************/
/* for use with PDL_SENSOR_ACCELEROMETER */
typedef struct
{
	PDL_SensorType type;
	float32 x;
	float32 y;
	float32 z;
} PDL_AccelerometerEvent;

/* for use with PDL_SENSOR_COMPASS */
typedef struct
{
	PDL_SensorType type;
	float32 magnetic;
	float32 trueBearing;
	float32 confidence;
} PDL_CompassEvent;

/* for use with PDL_SENSOR_ALS */
typedef struct
{
	PDL_SensorType type;
	Sint32 lux;
} PDL_ALSEvent;

/* for use with PDL_SENSOR_ANGULAR_VELOCITY */
typedef struct
{
	PDL_SensorType type;
	float32 x;
	float32 y;
	float32 z;
} PDL_AngularVelocityEvent;

/* for use with PDL_SENSOR_GRAVITY */
typedef struct
{
	PDL_SensorType type;
	float32 x;
	float32 y;
	float32 z;
} PDL_GravityEvent;

/* for use with PDL_SENSOR_LINEAR_ACCELERATION */
typedef struct
{
	PDL_SensorType type;
	float32 x;
	float32 y;
	float32 z;
	float32 worldX;
	float32 worldY;
	float32 worldZ;
} PDL_LinearAccelerationEvent;

/* for use with PDL_SENSOR_MAGNETIC_FIELD */
typedef struct
{
	PDL_SensorType type;
	float32 x;
	float32 y;
	float32 z;
	float32 rawX;
	float32 rawY;
	float32 rawZ;
} PDL_MagneticFieldEvent;

/* for use with PDL_SENSOR_ORIENTATION */
typedef struct
{
	PDL_SensorType type;
	Sint32 orientation;
} PDL_OrientationEvent;

/* for use with PDL_SENSOR_PROXIMITY */
typedef struct
{
	PDL_SensorType type;
	Sint32 presence;
} PDL_ProximityEvent;

/* for use with PDL_SENSOR_ROTATION */
typedef struct
{
	PDL_SensorType type;
	float32 matrix[9];
	float32 quatW;
	float32 quatX;
	float32 quatY;
	float32 quatZ;
	float32 yaw;
	float32 pitch;
	float32 roll;
} PDL_RotationEvent;

/* for use with PDL_SENSOR_SHAKE */
typedef struct
{
	PDL_SensorType type;
	Sint32 state;
	float32 magnitude;
} PDL_ShakeEvent;


/***************** MAIN SENSOR EVENT *********************/
/***************** MAIN SENSOR EVENT *********************/
/***************** MAIN SENSOR EVENT *********************/
typedef union PDL_SensorEvent 
{
	PDL_SensorType 					type;
	PDL_AccelerometerEvent 			accelerometer;
	PDL_CompassEvent 				compass;
	PDL_ALSEvent 					als;
	PDL_AngularVelocityEvent		angularVelocity;
	PDL_GravityEvent				gravity;
	PDL_LinearAccelerationEvent 	linearAcceleration;
	PDL_MagneticFieldEvent			magneticField;
	PDL_OrientationEvent			orientation;
	PDL_ProximityEvent				proximity;
	PDL_RotationEvent				rotation;
	PDL_ShakeEvent					shake;
	unsigned char 					reserved[128]; 
} PDL_SensorEvent;


/***************** SENSOR FUNCTIONS *********************/
/***************** SENSOR FUNCTIONS *********************/
/***************** SENSOR FUNCTIONS *********************/

/* returns PDL_TRUE if the sensor exists on this device. PDL_FALSE if not. */
PDL_bool PDL_SensorExists(PDL_SensorType sensor);

/* Turn on or off a sensor. Once it's enabled, you will be able 
 * to receive events from it. Once off, the events will stop. */
PDL_Err PDL_EnableSensor(PDL_SensorType sensor, PDL_bool bEnable);

/* Get events from a specific sensor. If the sensor is active and ready 
 * for polling, this will return PDL_NOERROR, even if there are no events
 * available. If there are no events available, the type field in the event
 * structure will be set to PDL_SENSOR_NONE. If there is an event, the
 * relevant event type within the union will be filled out, and the type
 * set appropriately. Note that this is for a *specific* sensor type. It will
 * only return events for that type. */
PDL_Err PDL_PollSensor(PDL_SensorType sensor, PDL_SensorEvent *event);

/* Same as PDL_PollSensor, but it will return the first event available for 
 * all the currently active sensors. Gives back an event of type PDL_SENSOR_NONE
 * if no events are available. */
PDL_Err PDL_PollActiveSensors(PDL_SensorEvent *event);


#ifdef __cplusplus
}
#endif	


#endif


