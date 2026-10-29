#version 420 core

// I didn't write this, burrowed somone's code

// steps to Bloom: 
// step 1. Thersold -> highlights
// step 2 -> Blur
// step 3 -> Layering

// Using what COD uses for bloom

uniform sampler2D texture0;
in vec2 fragTexCoord;

out vec4 outputColor;

void main() {

    // Up sampling

    // Prvents when rendering on edge, to adjust cords

    vec2 texelSize = 1.0 / vec2(textureSize(texture0, 0));

    const float filterRadius = 1.0;

	float x = texelSize.x * filterRadius;
	float y = texelSize.y * filterRadius;

	// Take 9 samples around current texel:
	// a - b - c
	// d - e - f
	// g - h - i
	// === ('e' is the current texel) ===
	vec3 a = texture(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y + y)).rgb;
	vec3 b = texture(texture0, vec2(fragTexCoord.x,     fragTexCoord.y + y)).rgb;
	vec3 c = texture(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y + y)).rgb;

	vec3 d = texture(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y)).rgb;
	vec3 e = texture(texture0, vec2(fragTexCoord.x,     fragTexCoord.y)).rgb;
	vec3 f = texture(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y)).rgb;

	vec3 g = texture(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y - y)).rgb;
	vec3 h = texture(texture0, vec2(fragTexCoord.x,     fragTexCoord.y - y)).rgb;
	vec3 i = texture(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y - y)).rgb;

    vec3 upsample;

	// Apply weighted distribution, by using a 3x3 tent filter:
	//  1   | 1 2 1 |
	// -- * | 2 4 2 |
	// 16   | 1 2 1 |
	upsample = e*4.0;
	upsample += (b+d+f+h)*2.0;
	upsample += (a+c+g+i);
	upsample *= 1.0 / 16.0;

    outputColor = vec4(upsample, 1.0);
}