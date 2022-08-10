#version 430 core

out vec4 FragColor;

layout (location = 3) uniform vec3 uVars;
layout (location = 5) uniform sampler2D uFBTexture;

in vec2 fTexCoords;
in vec2 fRes;
in vec2 fMouse;

void main()
{
    vec3 color = texture(uFBTexture, fTexCoords).rgb;
    //color = ((color + 1.0f) / 2.0f);
    FragColor = vec4(1-color ,1.0f);
}
