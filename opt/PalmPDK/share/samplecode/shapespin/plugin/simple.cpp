/**
    This is sample code for the Palm webOS PDK.  See the LICENSE file for usage.
    Copyright (C) 2010 Palm, Inc.  All rights reserved.
**/

#include <stdio.h>
#include <math.h>

#include <GLES2/gl2.h>
#include "SDL.h"
#include "PDL.h"

#define LOGGING
#ifdef LOGGING
#include <syslog.h>
#define SYSLOG(...) syslog(__VA_ARGS__)
#else
#define SYSLOG(...) do { } while(0)
#endif

SDL_Surface *Surface;               // Screen surface to retrieve width/height information

int         Shader[2];              // We have a vertex & a fragment shader
int         Program;                // Totalling one program
float       Angle = 0.0;            // Rotation angle of our object
float       Proj[4][4];             // Projection matrix
int         iProj, iModel;          // Our 2 uniforms
int         lastTicks;              // Holds last time Display() was called so we can compute the time difference
int         id;

// because this variable is accessed from two threads, the main thread 
// and the JavaScript method handler thread, we mark it as volatile so the 
// compiler will always reread it from memory when its used. 
volatile float RotationSpeed = 0.001f; // Rotation speed of our object

// another cross-thread variable, used to stop movement when plugin is in card that's minimized
volatile bool Paused = false;

// this is used to intialize the GL ES emulation library on Windows, but not 
// compiled for device builds.


#ifdef WIN32
extern "C"  GL_API int GL_APIENTRY _dgles_load_library(void *, void *(*)(void *, const char *));
#endif

static void *proc_loader(void *h, const char *name)
{
    (void) h;
    return SDL_GL_GetProcAddress(name);
}

void doSetRotationSpeed(double val)
{
    // we'll make the slowest it goes 0.001, and 
    // the fastest 0.01
    double slowest = -0.005;
    double fastest = 0.005;
    double range = fastest - slowest;

    RotationSpeed = (float)(slowest + ((range*val)/100.0f));
}

// takes a value from 0 to 100 and make it 0 to 2*PI
PDL_bool setRotationSpeed(PDL_JSParameters *params)
{
    double val;
    if (PDL_GetNumJSParams(params) < 1 ||
        (val = PDL_GetJSParamDouble(params, 0), val < 0.0 || val > 100.0))
    {
        PDL_JSException(params, "You must send a value from 0 to 100");
        return PDL_TRUE;
    }
    
    char reply[32];
    snprintf(reply, 32, "%2.3f", val);
    PDL_JSReply(params, reply);
    
    doSetRotationSpeed(val);

    return PDL_TRUE;
}

// these methods take no parameters.  They're needed when running as a plugin in a hybrid
// application to pause the rendering loop because plugins to JS apps don't get application
// activation messages.
PDL_bool pause(PDL_JSParameters *params)
{
    Paused = true;
}

PDL_bool resume(PDL_JSParameters *params)
{
    Paused = false;

    // push a bogus event into the queue to wake up main thread
    SDL_Event Event;
    Event.active.type = SDL_ACTIVEEVENT;
    Event.active.gain = 1;
    Event.active.state = 0; /* no state makes this event meaningless */
    SDL_PushEvent(&Event);
}

// Standard GL perspective matrix creation
void Persp(float Proj[4][4], const float FOV, const float ZNear, const float ZFar)
{
    const float Delta   = ZFar - ZNear;

    memset(Proj, 0, sizeof(Proj));

    Proj[0][0] = 1.0f / tanf(FOV * 3.1415926535f / 360.0f);
    Proj[1][1] = Proj[0][0] / ((float)Surface->h / Surface->w);

    Proj[2][2] = -(ZFar + ZNear) / Delta;
    Proj[2][3] = -1.0f;
    Proj[3][2] = -2.0f * ZFar * ZNear / Delta;
}



// Simple function to create a shader
void LoadShader(char *Code, int ID)
{
    // Compile the shader code
    glShaderSource  (ID, 1, (const char **)&Code, NULL); 
    glCompileShader (ID);

    // Verify that it worked
    int ShaderStatus;
    glGetShaderiv(ID, GL_COMPILE_STATUS, &ShaderStatus); 

    // Check the compile status
    if (ShaderStatus != GL_TRUE) {
        printf("Error: Failed to compile GLSL program\n");
        int Len = 1024;
        char Error[1024];
        glGetShaderInfoLog(ID, 1024, &Len, Error);
        printf(Error);
        exit (-1);
    }
}

// Initializes the application data
int Init(void) 
{
    // Very basic ambient+diffusion model
    const char VertexShader[] = "                   \
        attribute vec3 Position;                    \
        attribute vec3 Normal;                      \
                                                    \
        uniform mat4 Proj;                          \
        uniform mat4 Model;                         \
                                                    \
        varying vec3 NormVec;                       \
        varying vec3 LighVec;                       \
                                                    \
        void main(void)                             \
        {                                           \
            vec4 Pos = Model * vec4(Position, 1.0); \
                                                    \
            gl_Position = Proj * Pos;               \
                                                    \
            NormVec     = (Model * vec4(Normal,0.0)).xyz;     \
            LighVec     = -Pos.xyz;                 \
        }                                           \
    ";

    const char FragmentShaderRed[] = "                                             \
        varying highp vec3 NormVec;                                             \
        varying highp vec3 LighVec;                                             \
                                                                                \
        void main(void)                                                         \
        {                                                                       \
            lowp vec3 Color = vec3(1.0, 0.0, 0.0);                              \
                                                                                \
            mediump vec3 Norm  = normalize(NormVec);                            \
            mediump vec3 Light = normalize(LighVec);                            \
                                                                                \
            mediump float Diffuse = dot(Norm, Light);                           \
                                                                                \
            gl_FragColor = vec4(Color * (max(Diffuse, 0.0) * 0.6 + 0.4), 1.0);  \
        }                                                                       \
    ";
    
    const char FragmentShaderBlue[] = "                                             \
        varying highp vec3 NormVec;                                             \
        varying highp vec3 LighVec;                                             \
                                                                                \
        void main(void)                                                         \
        {                                                                       \
            lowp vec3 Color = vec3(0.0, 0.0, 1.0);                              \
                                                                                \
            mediump vec3 Norm  = normalize(NormVec);                            \
            mediump vec3 Light = normalize(LighVec);                            \
                                                                                \
            mediump float Diffuse = dot(Norm, Light);                           \
                                                                                \
            gl_FragColor = vec4(Color * (max(Diffuse, 0.0) * 0.6 + 0.4), 1.0);  \
        }                                                                       \
    ";

    // Create 2 shader programs
    Shader[0] = glCreateShader(GL_VERTEX_SHADER);
    Shader[1] = glCreateShader(GL_FRAGMENT_SHADER);

    LoadShader((char *)VertexShader, Shader[0]);
    
    if ( id == 2 )
    {
        LoadShader((char *)FragmentShaderBlue, Shader[1]);
    }
    else
    {
        LoadShader((char *)FragmentShaderRed, Shader[1]);
    }

    // Create the prorgam and attach the shaders & attributes
    Program   = glCreateProgram();

    glAttachShader(Program, Shader[0]);
    glAttachShader(Program, Shader[1]);

    glBindAttribLocation(Program, 0, "Position");
    glBindAttribLocation(Program, 1, "Normal");

    // Link
    glLinkProgram(Program);

    // Validate our work thus far
    int ShaderStatus;
    glGetProgramiv(Program, GL_LINK_STATUS, &ShaderStatus); 

    if (ShaderStatus != GL_TRUE) {
        printf("Error: Failed to link GLSL program\n");
        int Len = 1024;
        char Error[1024];
        glGetProgramInfoLog(Program, 1024, &Len, Error);
        printf(Error);
        exit(-1);
    }

    glValidateProgram(Program);

    glGetProgramiv(Program, GL_VALIDATE_STATUS, &ShaderStatus); 

    if (ShaderStatus != GL_TRUE) {
        printf("Error: Failed to validate GLSL program\n");
        exit(-1);
    }

    // Enable the program
    glUseProgram                (Program);
    glEnableVertexAttribArray   (0);
    glEnableVertexAttribArray   (1);

    // Setup the Projection matrix
    Persp(Proj, 70.0f, 0.1f, 200.0f);

    // Retrieve our uniforms
    iProj   = glGetUniformLocation(Program, "Proj");
    iModel  = glGetUniformLocation(Program, "Model");

    // Basic GL setup
    glClearColor    (0.0, 0.0, 0.0, 1.0);
    glEnable        (GL_CULL_FACE);
    glCullFace      (GL_BACK);

    return GL_TRUE;
}



// Main-loop workhorse function for displaying the object
void Display(void)
{
    // Clear the screen
    glClear (GL_COLOR_BUFFER_BIT);

    float Model[4][4];

    memset(Model, 0, sizeof(Model));

    // Setup the Proj so that the object rotates around the Y axis
    // We'll also translate it appropriately to Display
    Model[0][0] = cosf(Angle);
    Model[1][1] = 1.0f;
    Model[2][0] = sinf(Angle);
    Model[0][2] = -sinf(Angle);
    Model[2][2] = cos(Angle);
    Model[3][2] = -1.0f;   
    Model[3][3] = 1.0f;

    // Constantly rotate the object as a function of time
    int ticks = SDL_GetTicks(); // note current time
    int thisTicks = ticks - lastTicks; // note delta time
    if ( thisTicks > 200 ) thisTicks = 200; // throttling
    Angle += ((float)thisTicks) * RotationSpeed; // apply animation
    lastTicks = ticks; // note for next loop

    // Vertex information
    float PtData[][3] = {
        {0.5f, 0.0380823f, 0.028521f},
        {0.182754f, 0.285237f, 0.370816f},
        {0.222318f, -0.2413f, 0.38028f},
        {0.263663f, -0.410832f, -0.118163f},
        {0.249651f, 0.0109279f, -0.435681f},
        {0.199647f, 0.441122f, -0.133476f},
        {-0.249651f, -0.0109279f, 0.435681f},
        {-0.263663f, 0.410832f, 0.118163f},
        {-0.199647f, -0.441122f, 0.133476f},
        {-0.182754f, -0.285237f, -0.370816f},
        {-0.222318f, 0.2413f, -0.38028f},
        {-0.5f, -0.0380823f, -0.028521f},
    };

    // Face information
    unsigned short FaceData[][3] = {
        {0,1,2,},
        {0,2,3,},
        {0,3,4,},
        {0,4,5,},
        {0,5,1,},
        {1,5,7,},
        {1,7,6,},
        {1,6,2,},
        {2,6,8,},
        {2,8,3,},
        {3,8,9,},
        {3,9,4,},
        {4,9,10,},
        {4,10,5,},
        {5,10,7,},
        {6,7,11,},
        {6,11,8,},
        {7,10,11,},
        {8,11,9,},
        {9,11,10,},
    };


    // Draw the icosahedron
    glUseProgram            (Program);
    glUniformMatrix4fv      (iProj, 1, false, (const float *)&Proj[0][0]);
    glUniformMatrix4fv      (iModel, 1, false, (const float *)&Model[0][0]);

    glVertexAttribPointer   (0, 3, GL_FLOAT, 0, 0, &PtData[0][0]);
    glVertexAttribPointer   (1, 3, GL_FLOAT, GL_TRUE, 0, &PtData[0][0]);

    glDrawElements          (GL_TRIANGLES, sizeof(FaceData) / sizeof(unsigned short), 
                            GL_UNSIGNED_SHORT, &FaceData[0][0]);
}

void statusCallback(const char *reply, void *context)
{
    printf("statusCallback: %s\n", reply);
}


int main(int argc, char** argv)
{
    // defaults value is 1 which uses the red shader
    id = 1;
    
    // when used as a plugin, the "Param1" argument is the first item on the command line.  We 
    // look for that here and parse it as a new id number.
    if ( argc >= 2 )
    {
        id = atoi(argv[1]);
    }
    
    // Initialize the SDL library with the Video subsystem
    int result = SDL_Init(SDL_INIT_VIDEO);
    
    if ( result != 0 )
    {
        printf("Could not init SDL: %s\n", SDL_GetError());
        exit(1);
    }

    PDL_Init(0);
    
    // register the js callback
    PDL_RegisterJSHandler("setRotationSpeed", setRotationSpeed);
    PDL_RegisterJSHandler("setAngle", setRotationSpeed);
    PDL_RegisterJSHandler("pause", pause);
    PDL_RegisterJSHandler("resume", resume);
    PDL_JSRegistrationComplete();
    PDL_CallJS("ready", NULL, 0);

    // Tell it to use OpenGL version 2.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    
    // Set the video mode to full screen (or the full size of the plugin area)
    // with OpenGL-ES support
    Surface = SDL_SetVideoMode(0, 0, 0, SDL_OPENGL);

#if WIN32
    // Load the desktop OpenGL-ES emulation library
    _dgles_load_library(NULL, proc_loader);
#endif

    // Application specific Initialize of data structures & GL states
    if (Init() == false)
    {
        return -1;
    }
    
    // init the time stuff
    lastTicks = SDL_GetTicks();
    doSetRotationSpeed(10); // start at 10, where the slider starts

    bool didIt = false;
    Uint32 fireTime = SDL_GetTicks() + 1000;

    // Event descriptor
    SDL_Event Event;
    Event.type = SDL_NOEVENT;

    do 
    {
        if ( !didIt && (SDL_GetTicks() > fireTime) )
        {
            // fire off the js
            const char *params[2];
            params[0] = "foo";
            params[1] = "bar";
            PDL_Err mjErr = PDL_CallJS("testFunc", params, 2);
            if ( mjErr != PDL_NOERROR )
            {
                printf("error\n");
            }
            didIt = true;
        }

        if (Paused)
        {
            SDL_WaitEvent(&Event);
            if (Event.type == SDL_ACTIVEEVENT)
            {
                if ((Event.active.state & SDL_APPACTIVE) &&    (Event.active.gain == 1))
                {
                    Paused = false;
                    lastTicks = SDL_GetTicks();
                    Display();
                }
            }
            else if (Event.type == SDL_VIDEORESIZE)
            {
                SYSLOG(LOG_ERR, "Resizing plugin to %dx%d", Event.resize.w, Event.resize.h);
                Surface = SDL_SetVideoMode(Event.resize.w,Event.resize.h, 0, SDL_OPENGL);
                if (Init() == false)
                {
                    exit(-1);
                }
                Display();
            }            
            else if (Event.type == SDL_VIDEOEXPOSE)
            {
                // Rerender the display if system asks us too
                Display();
            }
        }
        else
        {
            // Render our scene
            Display();

            // Make it visible on the screen
            SDL_GL_SwapBuffers();
            
            // Process the events
            while (SDL_PollEvent(&Event)) 
            {
                switch (Event.type) 
                {
                    // List of keys that have been pressed
                    case SDL_KEYDOWN:
                        switch (Event.key.keysym.sym) 
                        {
                            // Escape forces us to quit the app
                            case SDLK_ESCAPE:
                                Event.type = SDL_QUIT;
                                break;
                        }
                        break;
                    
                    case SDL_VIDEORESIZE:
                        SYSLOG(LOG_ERR, "Resizing plugin to %dx%d", Event.resize.w, Event.resize.h);
                        Surface = SDL_SetVideoMode(Event.resize.w, Event.resize.h, 0, SDL_OPENGL);
                        if (Init() == false)
                        {
                            exit(-1);
                        }
                        Display();
                        break;

                    // handle deactivation by pausing our animation
                    case SDL_ACTIVEEVENT:
                        if ((Event.active.state & SDL_APPACTIVE) &&    (Event.active.gain == 0))
                        {
                            Paused = true;
                        }
                        break;

                    default:
                        break;
                }
            }
        }
    } while (Event.type != SDL_QUIT);
    // We exit anytime we get a request to quit the app

    // Cleanup
    PDL_Quit();
    SDL_Quit();

    return 0;
}
