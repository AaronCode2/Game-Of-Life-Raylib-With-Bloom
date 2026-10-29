#version 420 core

in vec2 fragTexCoord;
uniform sampler2D texture0;

// Shaders are a much of maths, I'm not a fan of maths

uniform float time;
uniform vec2 position;
uniform vec2 screenSize;

out vec4 ouputColor;

void main() {

    const vec4 textureColor = texture(texture0, fragTexCoord);

    const float lightRange = 60.0 + sin(time) * 10;

    // pathogerm, dude this triangle thing again

    const float xDistance = abs((position.x - gl_FragCoord.x) * (position.x - gl_FragCoord.x));
    const float yDistance = abs((gl_FragCoord.y - position.y) * (gl_FragCoord.y - position.y));

    const float distance = sqrt(yDistance + xDistance);

    const float alpha = clamp(1 / (distance / lightRange), 1.0, 0.0);

    if(textureColor.a == 0.0)
        ouputColor = vec4(textureColor.rgb, 0.0);
    else
        ouputColor = vec4(textureColor.rgb, alpha);

}