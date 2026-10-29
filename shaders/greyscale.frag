#version 420 core

// Use max of only 16 vec4s, no more!

in vec2 fragTexCoord;
uniform sampler2D texture0;

out vec4 ouputColor;

void main() {

    const vec3 grey_weights = vec3(0.299, 0.587, 0.114);
    
    vec4 textureColor = texture(texture0, fragTexCoord);
    
    float grey = dot(vec3(textureColor), grey_weights);
    
    ouputColor = vec4(grey, grey, grey, textureColor.a);
}