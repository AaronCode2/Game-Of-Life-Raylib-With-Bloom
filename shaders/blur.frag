#version 420 core

// Making my own, because all the others suck. This is pretty dam good.

in vec2 fragTexCoord;
in vec4 fragColor;

uniform vec4 colDiffuse;
uniform sampler2D texture0;

uniform vec2 axis;

out vec4 outputColor;

void main() {

    // Pre-computed Gaussian floats.

    float weights[9];
    weights[0] = 0.080497596;
    weights[1] = 0.078903637;
    weights[2] = 0.074308647;
    weights[3] = 0.067237244;
    weights[4] = 0.058453252;
    weights[5] = 0.048824260;
    weights[6] = 0.039182387;
    weights[7] = 0.030211641;
    weights[8] = 0.022381334;

    vec4 textureColor = texture(texture0, fragTexCoord) * weights[0];

    ivec2 textureSize = textureSize(texture0, 0);
    vec2 texelSize = 1.0 / vec2(textureSize);

    float wieghtSum = weights[0];

    // This is the best bluring, clumps performance really quickly. 

    for(int x = 0; x <= 8; x++) {

        vec2 offset = vec2(axis * float(x)) * texelSize;

        // Merge weights with the offset for shades

        textureColor += texture(texture0, fragTexCoord + offset) * weights[x];
        textureColor += texture(texture0, fragTexCoord - offset) * weights[x];

        wieghtSum += weights[x] * 2.0;
    }

    const vec4 finalTexture = textureColor / wieghtSum; 

    outputColor = textureColor * colDiffuse;
}