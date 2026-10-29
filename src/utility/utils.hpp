#pragma once
#include <raylib.h>
#include <iostream>
#include <assert.h>
#include <fstream>

// Burrowed Code, not all funcs are used.

/*
    Disable this if you don't have RenderDoc installed on your pc.
    This is just used for debugging purposes. (Only Works on Windows) 
    Check Json file for more info

    In: #define ENABLE_RENDERDOC true 
    Replcae with: with #define ENABLE_RENDERDOC false
*/

#define ENABLE_RENDERDOC true

// Some C++ operator magic

inline Vector2 operator<<=(Vector2 &v1, const Vector2 &v2) {

    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

inline Vector2 operator*(Vector2 vec, const float numf) {

    vec.x *= numf;
    vec.y *= numf;
    return vec;
}

inline Vector2 operator*(Vector2 vec1, Vector2 vec2) {

    vec1.x *= vec2.x;
    vec1.y *= vec2.y;
    return vec1;
}

inline Vector2 operator+(Vector2 vec1, Vector2 vec2) {

    vec1.x += vec2.x;
    vec1.y += vec2.y;
    return vec1;
}

inline Vector2 operator/(Vector2 vec1, const float num) {

    vec1.x /= num;
    vec1.y /= num;
    return vec1;
}

inline Vector2 operator/=(Vector2 &vec1, const float num) {

    vec1.x /= num;
    vec1.y /= num;
    return vec1;
}

inline Vector2 operator>>=(Vector2 &v1, const Vector2 &v2) {

    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}

// I wanna do something else than coding, It really is a concern. 

inline bool operator!=(const Vector2 &v1, const Vector2 &v2) {

    return (v1.x != v2.x || v1.y != v2.y);
}

inline bool operator==(const Vector2 &v1, const Vector2 &v2) {

    return (v1.x == v2.x && v1.y == v2.y);
}

inline Rectangle operator<<=(Rectangle &v1, const Vector2 &v2) {

    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

namespace Utils {

    extern float deltaTimeIt;
    extern Color const testColor;
    Rectangle getScreenRect();
    Vector2 getScreenSizeV();

    std::string formatZeros(int num, int width);
    extern int fontSize;

    bool isSameXY(const Rectangle aObject, const Rectangle bObject);
    bool isSameXY(const Vector2 aObject, const Vector2 bObject);
    bool isSameXY(const Vector2 aObject, const Vector2 bObject, const Vector2 bObject2);
    
    void collisionActionX(Rectangle &aObject, const Rectangle ahitBox, const Rectangle bHitBox, Vector2 &aVelocity, bool resetVelocity = true);
    void collisionActionY(Rectangle &aObject, const Rectangle ahitBox, const Rectangle bHitBox, Vector2 &aVelocity, bool resetVelocity = true);
    
    void debugRect(Rectangle object);

    void exitApp(const std::string errorInfo);

    extern Font font;

    // Did this for emphasis

    extern Rectangle THE_DEBUG_RECT;
    extern Vector2 THE_DEBUG_VECTOR;
    extern int selectedtileTypeIndex;

    extern bool hackerMode;

    extern int shaderIndex;
} 


namespace Mouse {

    bool isHovering(const Rectangle object);
    bool isClickedL(const Rectangle object);
    bool isClickedR(const Rectangle object);

    bool isClickedOnceR(const Rectangle object);
    bool isClickedOnceL(const Rectangle object);
}

// constants

#define GREYSCALE_FRAG_PATH (const char *) "../shaders/greyscale.frag"
#define FOG_FRAG_PATH (const char *) "../shaders/fog.frag"
#define BLUR_FRAG_PATH (const char *) "../shaders/blur.frag"
#define BLOOM_FŬSI_FRAG_PATH (const char *) "../shaders/bloom_fusi.frag"

#define UP_SAMPLING_FRAG_PATH (const char *) "../shaders/up_sampling.frag"
#define DOWN_SAMPLING_FRAG_PATH (const char *) "../shaders/down_sampling.frag"

#define RAYLIB_SHADER 0

#define NUM_OF_RENDER_BUFFERS 8
#define TARGET_BUFFER 0