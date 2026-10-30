#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;


out vec4 finalColor;


void main() {
    
    const float exposure = 0.5;
    float gamma = 2.2;
    vec3 hdrColor = texture(texture0, fragTexCoord).rgb;
    vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
    mapped = pow(mapped, vec3(1.0/gamma));

    // float brightness = dot(mapped, vec3(0.2126, 0.7152, 0.0722));

    // if(brightness < 0.01) {
    //     mapped = vec3(0.0);
    // }

    finalColor = vec4(mapped, 1.0);
}