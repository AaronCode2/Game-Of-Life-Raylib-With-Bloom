#include "renderer.hpp"

// Thirdparty lib for debugging purposes

#if ENABLE_RENDERDOC

pRENDERDOC_GetAPI RendererDebugger::RENDERDOC_GetAPI;
RENDERDOC_API_1_1_2* RendererDebugger::renderDocAPI = nullptr; 
void *RendererDebugger::renderDocDLL = nullptr;

void *RendererDebugger::loadRenderDoc() {

    void *renderDocDLL = (void *) LoadLibraryA("renderdoc.dll");

    if(renderDocDLL == nullptr)
        Utils::exitApp("RenderDoc DLL was not found!");

    // This some sloppy code, even I coudld't understand it.

    pRENDERDOC_GetAPI RENDERDOC_GetAPI = 
    (pRENDERDOC_GetAPI)GetProcAddress((HMODULE)renderDocDLL, "RENDERDOC_GetAPI");
    RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_7_0, (void **)&renderDocAPI);

    if(renderDocAPI == nullptr)
        Utils::exitApp("RenderDoc API was injected and didn't init right");

    return renderDocDLL;
}

void RendererDebugger::unloadRenderDoc() {

    // renderDocAPI->Shutdown();
}

void RendererDebugger::beginRenderDocCapture() {

    if(renderDocAPI != nullptr)
        renderDocAPI->StartFrameCapture(nullptr, nullptr);
}

void RendererDebugger::endRenderDocCapture() {

    if(renderDocAPI != nullptr)
        renderDocAPI->EndFrameCapture(nullptr, nullptr);
}

bool RendererDebugger::isRenderDocFrameCapturing() {

    if(renderDocAPI != nullptr) return renderDocAPI->IsFrameCapturing();

    return false;
}

#endif

Renderer::Renderer() {

    initBuffers();
}

Renderer::~Renderer() {

#if ENABLE_RENDERDOC

    RendererDebugger::unloadRenderDoc();

#endif
}

void Renderer::setCurrentShader(const ShaderIndex newCurrentShader) {

    currentShader = newCurrentShader;
}

// The playerVel is optional, only use for shaders that need it.
void Renderer::applyShader() {

    switch(currentShader) {

        case ShaderIndex::GREYSCALE:

            applyShaderDirectly = true;
            break;

        case ShaderIndex::FOG: {

            // If you don't know what a buffer is: It's just a temp pool of memory used to do stuff.

            // Shaders::uniforms[UniformIndex::FOG_POS].data = &playerVelArg;

            SetShaderValueV(
                Shaders::shader[ShaderIndex::FOG],
                Shaders::uniforms[UniformIndex::FOG_POS].loc,
                Shaders::uniforms[UniformIndex::FOG_POS].data,
                SHADER_UNIFORM_VEC2, 1
            );
            
            Vector2 size = Utils::getScreenSizeV();
            Shaders::uniforms[UniformIndex::FOG_WINDOW_SIZE].data = &size;

            SetShaderValueV(
                Shaders::shader[currentShader],
                Shaders::uniforms[UniformIndex::FOG_WINDOW_SIZE].loc,
                Shaders::uniforms[UniformIndex::FOG_WINDOW_SIZE].data,
                SHADER_UNIFORM_VEC2, 1
            );
            
            float time = GetTime();
            Shaders::uniforms[UniformIndex::FOG_TIME].data = &time;

            SetShaderValue(
                Shaders::shader[currentShader],
                Shaders::uniforms[UniformIndex::FOG_TIME].loc,
                Shaders::uniforms[UniformIndex::FOG_TIME].data,
                SHADER_UNIFORM_FLOAT
            );
            
            applyShaderDirectly = true;
            break;
        }

        case ShaderIndex::BLUR: {

            // X-axis bluring

            Vector2 axis = {1.0f, 0.0f};
            Shaders::uniforms[UniformIndex::BLUR_AXIS].data = &axis;

            SetShaderValueV(
                Shaders::shader[currentShader],
                Shaders::uniforms[UniformIndex::BLUR_AXIS].loc,
                Shaders::uniforms[UniformIndex::BLUR_AXIS].data,
                SHADER_UNIFORM_VEC2, 1
            );

            applyShaderToBuffer(0, 1);

            // Y-axis bluring

            axis = {0.0f, 1.0f};
            Shaders::uniforms[UniformIndex::BLUR_AXIS].data = &axis;

            SetShaderValueV(
                Shaders::shader[currentShader],
                Shaders::uniforms[UniformIndex::BLUR_AXIS].loc,
                Shaders::uniforms[UniformIndex::BLUR_AXIS].data,
                SHADER_UNIFORM_VEC2, 1
            );

            applyShaderToBuffer(0, 0);

            // Merge Y-axis and X-axis Textures Together

            blendBuffers(0, 1, NUM_OF_RENDER_BUFFERS - 1);
            break;
        }

        // Avoid using small circles.

        case ShaderIndex::BLOOM_FŬSI: {

            // Apply affects on second buffer
            copyBuffer(TARGET_BUFFER, 1);

            for(int i = 1; i < NUM_OF_RENDER_BUFFERS - 1; i++)
                downSampleBuffer(i, i + 1);

            BeginBlendMode(BLEND_ADDITIVE);
            {
                for(int i = NUM_OF_RENDER_BUFFERS - 1; i > 1; i--)
                    upSampleBuffer(i, i - 1);
            }
            EndBlendMode();

            // Render with exposure

            BeginTextureMode(renderBuffers[TARGET_BUFFER]);
            {
               
                ClearBackground(BLACK);
                // Latin for spread of distance.     
                BeginShaderMode(Shaders::shader[ShaderIndex::BLOOM_FŬSI]);
                {
                    DrawTexturePro(
                        renderBuffers[1].texture,
                        {
                            0, 0,
                            (float) renderBuffers[1].texture.width,
                            -(float) renderBuffers[1].texture.height                        
                        },
                        {
                            0, 0,
                            (float) renderBuffers[TARGET_BUFFER].texture.width,
                            -(float) renderBuffers[TARGET_BUFFER].texture.height
                        },
                        {0, 0}, 0.0f,
                        WHITE
                    );
                }
                EndShaderMode();
            }
            EndTextureMode();


            break;
        }
    }
}

void Renderer::drawResult() {

    if(applyShaderDirectly) {

        applyShaderToTarget();
        applyShaderDirectly = false;
        return;
    }

    if(currentShader == ShaderIndex::BLOOM_FŬSI) {

        DrawTextureRec(
            renderBuffers[TARGET_BUFFER].texture,
            {
                0, 0, 
                (float) renderBuffers[TARGET_BUFFER].texture.width, 
                -(float) renderBuffers[TARGET_BUFFER].texture.height
            }, {0, 0}, 
            WHITE
        );
        return;
    }

    DrawTextureRec(
        renderBuffers[NUM_OF_RENDER_BUFFERS - 1].texture,
        {
            0, 0, 
            (float) renderBuffers[NUM_OF_RENDER_BUFFERS - 1].texture.width, 
            -(float) renderBuffers[NUM_OF_RENDER_BUFFERS - 1].texture.height
        }, {0, 0}, 
        WHITE
    );

}

void Renderer::applyShaderToBuffer(int inputBuffer, int outputBuffer) {

    BeginTextureMode(renderBuffers[outputBuffer]);
    {

        BeginShaderMode(Shaders::shader[currentShader]);
        {
            DrawTextureRec(
                renderBuffers[inputBuffer].texture,
                {
                    0, 0, 
                    (float) renderBuffers[inputBuffer].texture.width, 
                    -(float) renderBuffers[inputBuffer].texture.height
                }, {0, 0}, 
                WHITE
            );
        }
        EndShaderMode();
    }
    EndTextureMode();
}

void Renderer::applyShaderToTarget() {

    BeginShaderMode(Shaders::shader[currentShader]);
    {
        DrawTextureRec(
            renderBuffers[TARGET_BUFFER].texture,
            {
                0, 0, 
                (float) renderBuffers[TARGET_BUFFER].texture.width, 
                -(float) renderBuffers[TARGET_BUFFER].texture.height
            }, {0, 0}, 
            WHITE
        );
    }
    EndShaderMode();
}

// Doesn't require HDR monitors.

RenderTexture2D Renderer::LoadHDRRender( int width, int height ) {

    RenderTexture2D target = { 0 };

    target.id = rlLoadFramebuffer(); // Load an empty framebuffer

    if ( target.id > 0 ) {
        rlEnableFramebuffer( target.id );

        // Create color texture (default to RGBA)
        target.texture.id = rlLoadTexture(
            NULL,
            width,
            height,
            PIXELFORMAT_UNCOMPRESSED_R32G32B32A32,
            1
        );
        target.texture.width = width;
        target.texture.height = height;
        target.texture.format = PIXELFORMAT_UNCOMPRESSED_R32G32B32A32;
        target.texture.mipmaps = 1;

        // Create depth renderbuffer/texture
        target.depth.id = rlLoadTextureDepth( width, height, true );
        target.depth.width = width;
        target.depth.height = height;
        target.depth.format = 19; // DEPTH_COMPONENT_24BIT?
        target.depth.mipmaps = 1;

        // Attach color texture and depth renderbuffer/texture to FBO
        rlFramebufferAttach(
            target.id,
            target.texture.id,
            RL_ATTACHMENT_COLOR_CHANNEL0,
            RL_ATTACHMENT_TEXTURE2D,
            0
        );
        rlFramebufferAttach(
            target.id,
            target.depth.id,
            RL_ATTACHMENT_DEPTH,
            RL_ATTACHMENT_RENDERBUFFER,
            0
        );

        // Check if fbo is complete with attachments (valid)
        if (rlFramebufferComplete( target.id ))
            TRACELOG(
                LOG_INFO,
                "FBO: [ID %i] Framebuffer object created successfully",
                target.id
            );

        rlDisableFramebuffer();
    } else
        TRACELOG( LOG_WARNING, "FBO: Framebuffer object can not be created" );

    // AHHHHHHHHHHHHHHHHHHHHHHHH
    SetTextureFilter( target.texture, TEXTURE_FILTER_BILINEAR );
    SetTextureWrap( target.texture, TEXTURE_WRAP_CLAMP );

    return target;
}

void Renderer::blendBuffers(int inputBuffer1, int inputBuffer2, int outputBuffer) {

    BeginTextureMode(renderBuffers[outputBuffer]);
    {

        // The texture.height it negative since OpenGL flips y-axis. LOL

        DrawTextureRec(
            renderBuffers[inputBuffer1].texture,
            {
                0, 0, 
                (float) renderBuffers[inputBuffer1].texture.width, 
                -(float) renderBuffers[inputBuffer1].texture.height
            }, {0, 0}, 
            WHITE
        );

        DrawTextureRec(
            renderBuffers[inputBuffer2].texture,
            {
                0, 0, 
                (float) renderBuffers[inputBuffer2].texture.width, 
                -(float) renderBuffers[inputBuffer2].texture.height
            }, {0, 0}, 
            WHITE
        );

        EndBlendMode();
    }
    EndTextureMode();
}

void Renderer::initBuffers() {

    // Some Vendors can struggle with this many buffers. Use lower when in OpenGL-ES
    renderBuffers[TARGET_BUFFER] = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    Vector2 bufferSize = Utils::getScreenSizeV();

    for(int i = 1; i < NUM_OF_RENDER_BUFFERS; i++) {

        renderBuffers[i] = LoadHDRRender(bufferSize.x, bufferSize.y);

        bufferSize /= 2.0f;
    }
}

void Renderer::copyBuffer(int inputBuffer, int outputBuffer) {

    BeginTextureMode(renderBuffers[outputBuffer]);
    {
        DrawTexturePro(
            renderBuffers[inputBuffer].texture,
            {
                0, 0,
                (float) renderBuffers[inputBuffer].texture.width,
                -(float) renderBuffers[inputBuffer].texture.height
            },
            {
                0, 0, 
                (float) renderBuffers[outputBuffer].texture.width, 
                -(float) renderBuffers[outputBuffer].texture.height
            }, 
            {0, 0}, 0.0f,
            WHITE
        );
    }
    EndTextureMode();
}

void Renderer::copyBufferWithBrightness(int inputBuffer, int outputBuffer) {

    BeginTextureMode(renderBuffers[outputBuffer]);
    {
        DrawTexturePro(
            renderBuffers[inputBuffer].texture,
            {
                0, 0,
                (float) renderBuffers[inputBuffer].texture.width,
                -(float) renderBuffers[inputBuffer].texture.height
            },
            {
                0, 0, 
                (float) renderBuffers[outputBuffer].texture.width, 
                -(float) renderBuffers[outputBuffer].texture.height
            }, 
            {0, 0}, 0.0f,
            ColorBrightness(WHITE, 0.0f)
        );
    }
    EndTextureMode();
}

void Renderer::copyBufferWithDifferentSizes(int inputBuffer, int outputBuffer) {

    BeginTextureMode(renderBuffers[outputBuffer]);
    {
        DrawTexturePro(
            renderBuffers[inputBuffer].texture,
            {
                0, 0,
                (float) renderBuffers[inputBuffer].texture.width,
                (float) renderBuffers[inputBuffer].texture.height,
            },
            {
                0, 0, 
                (float) renderBuffers[outputBuffer].texture.width, 
                -(float) renderBuffers[outputBuffer].texture.height
            }, 
            {0, 0}, 0.0f,
            WHITE
        );
    }
    EndTextureMode();
}

void Renderer::downSampleBuffer(int inputBuffer, int outputBuffer) {

    Vector2 bufferSize = {
        (float)  renderBuffers[outputBuffer].texture.width,
        (float) renderBuffers[outputBuffer].texture.height
    };

    Shaders::uniforms[UniformIndex::DOWN_SAMPLE_TEXTURE_SIZE].data = &bufferSize;

    SetShaderValueV(
        Shaders::shader[ShaderIndex::DOWN_SAMPLING],
        Shaders::uniforms[UniformIndex::DOWN_SAMPLE_TEXTURE_SIZE].loc,
        Shaders::uniforms[UniformIndex::DOWN_SAMPLE_TEXTURE_SIZE].data,
        SHADER_UNIFORM_VEC2, 1
    );

    BeginTextureMode(renderBuffers[outputBuffer]);
    {
        ClearBackground(BLACK);
        BeginShaderMode(Shaders::shader[ShaderIndex::DOWN_SAMPLING]);
        {
            DrawTexturePro(
                renderBuffers[inputBuffer].texture,
                {
                    0, 0,
                    (float) renderBuffers[inputBuffer].texture.width,
                    -(float) renderBuffers[inputBuffer].texture.height,
                },
                {
                    0, 0,
                    (float) renderBuffers[outputBuffer].texture.width,
                    (float) renderBuffers[outputBuffer].texture.height,
                }, {0, 0}, 0.0f,
                WHITE
            );
        }
        EndShaderMode();
    }   
    EndTextureMode();
}

void Renderer::upSampleBuffer(int inputBuffer, int outputBuffer) {

    // We use filter radius instead of bufferSize

    Vector2 bufferSize = {
        (float)  renderBuffers[outputBuffer].texture.width,
        (float) renderBuffers[outputBuffer].texture.height
    };

    Shaders::uniforms[UniformIndex::UP_SAMPLE_TEXTURE_SIZE].data = &bufferSize;

    SetShaderValueV(
        Shaders::shader[ShaderIndex::UP_SAMPLING],
        Shaders::uniforms[UniformIndex::UP_SAMPLE_TEXTURE_SIZE].loc,
        Shaders::uniforms[UniformIndex::UP_SAMPLE_TEXTURE_SIZE].data,
        SHADER_UNIFORM_VEC2, 1
    );                                                          

    BeginTextureMode(renderBuffers[outputBuffer]);
    {
        BeginShaderMode(Shaders::shader[ShaderIndex::UP_SAMPLING]);
        {
            DrawTexturePro(
                renderBuffers[inputBuffer].texture,
                {
                    0, 0,
                    (float) renderBuffers[inputBuffer].texture.width,
                    -(float) renderBuffers[inputBuffer].texture.height,
                },
                {
                    0, 0,
                    (float) renderBuffers[outputBuffer].texture.width,
                    (float) renderBuffers[outputBuffer].texture.height,
                }, 
                {0, 0}, 0.0f,
                WHITE
            );
        }
        EndShaderMode();
    }   
    EndTextureMode();
}
