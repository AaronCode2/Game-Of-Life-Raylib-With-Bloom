#include "assets.hpp"

std::map<ShaderIndex, Shader> Shaders::shader;
std::map<UniformIndex, Uniform> Shaders::uniforms;

// Shaders are complied at runtime, no cache / binary shaders used

void __stdcall Shaders::compileShaders() {

    // There is no Vertex shader, we use default raylib vertex, please don't make your own

    shader[ShaderIndex::GREYSCALE] = LoadShader(RAYLIB_SHADER, GREYSCALE_FRAG_PATH);
    shader[ShaderIndex::FOG] = LoadShader(RAYLIB_SHADER, FOG_FRAG_PATH);
    shader[ShaderIndex::BLUR] = LoadShader(RAYLIB_SHADER, BLUR_FRAG_PATH);
    shader[ShaderIndex::BLOOM_FŬSI] = LoadShader(RAYLIB_SHADER, BLOOM_FŬSI_FRAG_PATH);

    shader[ShaderIndex::DOWN_SAMPLING] = LoadShader(RAYLIB_SHADER, DOWN_SAMPLING_FRAG_PATH);
    shader[ShaderIndex::UP_SAMPLING] = LoadShader(RAYLIB_SHADER, UP_SAMPLING_FRAG_PATH);

    _getUniforms();
}

void Shaders::_getUniforms() {

    uniforms[UniformIndex::DOWN_SAMPLE_TEXTURE_SIZE].loc = GetShaderLocation(shader[ShaderIndex::DOWN_SAMPLING], "textureSize");
    uniforms[UniformIndex::UP_SAMPLE_TEXTURE_SIZE].loc = GetShaderLocation(shader[ShaderIndex::UP_SAMPLING], "textureSize");

    uniforms[UniformIndex::FOG_WINDOW_SIZE].loc = GetShaderLocation(shader[ShaderIndex::FOG], "screenSize");
    uniforms[UniformIndex::FOG_POS].loc = GetShaderLocation(shader[ShaderIndex::FOG], "position");
    uniforms[UniformIndex::FOG_TIME].loc = GetShaderLocation(shader[ShaderIndex::FOG], "time");

    uniforms[UniformIndex::BLUR_AXIS].loc = GetShaderLocation(shader[ShaderIndex::BLUR], "axis");
}
