/**
    Palm disclaimer
**/
#include <stdio.h>
#include <math.h>


#include <GLES2/gl2.h>
#include "SDL.h"
#include "PDL.h"

SDL_Surface *Surface;               // Screen surface to retrieve width/height information

int         Shader[2];              // We have a vertex & a fragment shader
int         Program;                // Totalling one program
float       Angle = 0.0;            // Rotation angle of our object
float       Proj[4][4];             // Projection matrix
int         iProj, iModel;          // Our 2 uniforms



#ifdef WIN32
extern "C" 
#endif
GL_API int GL_APIENTRY _dgles_load_library(void *, void *(*)(void *, const char *));

static void *proc_loader(void *h, const char *name)
{
    (void) h;
    return SDL_GL_GetProcAddress(name);
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
        printf("%s",Error);
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

    const char FragmentShader[] = "                                             \
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
            gl_FragColor = vec4(Color * (max(Diffuse, 0.0) * 0.6 + 0.4), 0.5);  \
        }                                                                       \
    ";

    // Create 2 shader programs
    Shader[0] = glCreateShader(GL_VERTEX_SHADER);
    Shader[1] = glCreateShader(GL_FRAGMENT_SHADER);

    LoadShader((char *)VertexShader, Shader[0]);
    LoadShader((char *)FragmentShader, Shader[1]);

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
        printf("%s",Error);
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
    Angle = SDL_GetTicks() * 0.001f;

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

void DumpSystemInfo()
{
    PDL_Err err;
    char buffer[256];
    
    int pdkVersion = PDL_GetPDKVersion();
    printf("PDL_GetPDKVersion: %d\n", pdkVersion);

    if (pdkVersion >= 200)
        printf("PDL_GetHardwareID: %d\n", PDL_GetHardwareID());

    PDL_OSVersion osVersion;
    if (PDL_NOERROR == PDL_GetOSVersion(&osVersion)) {
        printf("PDL_GetOSVersion: %s (%d.%d.%d)\n",
            osVersion.versionStr,
            osVersion.majorVersion,
            osVersion.minorVersion,
            osVersion.revision);
    }

    PDL_ScreenMetrics metrics;
    if (PDL_NOERROR == PDL_GetScreenMetrics(&metrics)) {
        printf("PDL_GetScreenMetrics: %d x %d, DPI %d x %d, Aspect Ratio: %f\n",
            metrics.horizontalPixels,
            metrics.verticalPixels,
            metrics.horizontalDPI,
            metrics.verticalDPI,
            metrics.aspectRatio);
    }

    if (PDL_NOERROR == PDL_GetUniqueID(buffer, 256)) {
        printf("PDL_GetUniqueID: %s\n", buffer);
    }
    if (PDL_NOERROR == PDL_GetDeviceName(buffer, 256)) {
        printf("PDL_GetDeviceName: %s\n", buffer);
    }
    if (PDL_NOERROR == PDL_GetLanguage(buffer, 256)) {
        printf("PDL_GetLanguage: %s\n", buffer);
    }
    if (pdkVersion >= 200) {
        if (PDL_NOERROR == PDL_GetRegionCountryName(buffer, 256)) {
            printf("PDL_GetRegionCountryName: %s\n", buffer);
        }
        if (PDL_NOERROR == PDL_GetRegionCountryCode(buffer, 256)) {
            printf("PDL_GetRegionCountryCode: %s\n", buffer);
        }
    }
}


int main(int argc, char** argv)
{
    // Initialize the SDL library with the Video subsystem
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE);
    atexit(SDL_Quit);
    
    // start the PDL library
    PDL_Init(0);
    atexit(PDL_Quit);
    
    // Tell it to use OpenGL version 2.0
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

    // Set the video mode to full screen with OpenGL-ES support
    // use zero for width/height to use maximum resolution
    Surface = SDL_SetVideoMode(0, 0, 0, SDL_OPENGL);

#if WIN32
    // Load the desktop OpenGL-ES emulation library
    _dgles_load_library(NULL, proc_loader);
#endif

    // Test out PDK system information APIs
    DumpSystemInfo();
    
    // Application specific Initialize of data structures & GL states
    if (Init() == false)
        return -1;

    // Event descriptor
    SDL_Event Event;
    bool paused = false;

    while (1) {
        // Render our scene
        Display();

        // Make it visible on the screen
        SDL_GL_SwapBuffers();

        bool gotEvent;
        if (paused) {
            SDL_WaitEvent(&Event);
            gotEvent = true;
        }
        else {
            gotEvent = SDL_PollEvent(&Event);
        }
        
        while (gotEvent) {
            switch (Event.type) {
                // List of keys that have been pressed
                case SDL_KEYDOWN:
                    switch (Event.key.keysym.sym) {
                        case PDLK_GESTURE_BACK: /* also maps to ESC */
                            if (PDL_GetPDKVersion() >= 200) {
                                // standard behavior is to minimize to a card when you perform a back
                                // gesture at the top level of the app
                                PDL_Minimize();
                            }
                            break;

                        default:
                            break;
                    }
                    break;

                case SDL_ACTIVEEVENT:
                    if (Event.active.state == SDL_APPACTIVE) {
                        paused = !Event.active.gain;
                    }
                    break;

                case SDL_QUIT:
                    // We exit anytime we get a request to quit the app
                    // all shutdown code is registered via atexit() so this is clean.
                    exit(0);
                    break;

                default:
                    break;
            }
            gotEvent = SDL_PollEvent(&Event);
        }
    }

    return 0;
}
