#pragma once
#include <raylib.h>
#include <rlgl.h>
#include "../utility/utils.hpp"
#include "../utility/assets.hpp"

// Burrowed Code, not all funcs are used.

// This is a thirdparty lib extension using DLLs (RenderDoc)

#if ENABLE_RENDERDOC

#define NOGDI
#define NOUSER
#ifndef WIN32_LEAN_AND_MEAN
    #define WIN32_LEAN_AND_MEAN
#endif

#include <windows.h>
#include <assert.h>
#include "../../libs/renderdoc/renderdoc_app.h"

struct RendererDebugger {

    static pRENDERDOC_GetAPI RENDERDOC_GetAPI;
    static RENDERDOC_API_1_1_2* renderDocAPI; 
    static void *renderDocDLL;

    static void* loadRenderDoc();
    static void unloadRenderDoc();

    static void beginRenderDocCapture();
    static void endRenderDocCapture();

    static bool isRenderDocFrameCapturing();
};

#endif

// If your wonder, yes I do touch grass.

class Renderer {

    public:

        Renderer();
        ~Renderer();

        // Make sure you add the appriotate Unifrom data with the shader, if it needs one
        void setCurrentShader(const ShaderIndex newCurrentShader); 

        void beginTargetRender() const { BeginTextureMode(renderBuffers[TARGET_BUFFER]); };
        void endTargetRender() const { EndTextureMode(); };

        void applyShader();

        void drawResult();

        // RenderDoc - Debug Rendering Tool.

    private:

        void applyShaderToBuffer(int inputBuffer = 0, int outputBuffer = 1);

        // Skips using mulitple buffers and just draws -> Gain Performance, when using low-end shaders
        void applyShaderToTarget();

        RenderTexture2D LoadHDRRender(int width, int height);

        void blendBuffers(int inputBuffer1, int inputBuffer2, int outputBuffer);

        void initBuffers();

        void copyBuffer(int inputBuffer, int outputBuffer);
        void copyBufferWithBrightness(int inputBuffer, int outputBuffer);

        void copyBufferWithDifferentSizes(int inputBuffer, int outputBuffer);

        void downSampleBuffer(int inputBuffer, int outputBuffer);
        void upSampleBuffer(int inputBuffer, int outputBuffer);

        bool applyShaderDirectly = false;

        ShaderIndex currentShader;

        RenderTexture2D renderBuffers[NUM_OF_RENDER_BUFFERS];


       
};