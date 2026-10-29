#pragma once
#include <raylib.h>
#include <map>
#include <math.h>
#include "utils.hpp"

// storing both vs and fs together

enum class ShaderIndex {

    GREYSCALE = 0,
    FOG,
    BLUR,
    BLOOM_FŬSI,
    DOWN_SAMPLING,
    UP_SAMPLING
};

enum class UniformIndex {

    DOWN_SAMPLE_TEXTURE_SIZE = 0,
    UP_SAMPLE_TEXTURE_SIZE,

    FOG_WINDOW_SIZE,
    FOG_POS,
    FOG_TIME,

    BLUR_AXIS
};

struct Uniform {

    int loc;
    void *data;
};

namespace Shaders {

    extern std::map<ShaderIndex, Shader> shader;
    extern std::map<UniformIndex, Uniform> uniforms;

    // Using fast calling reg

    void __stdcall  compileShaders();

    void _getUniforms();
}