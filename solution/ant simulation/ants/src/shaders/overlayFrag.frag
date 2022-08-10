#version 430 core

out vec4 FragColor;

layout (location = 2) uniform sampler2D uTexture;
layout (location = 3) uniform vec3 uVars;
layout (location = 5) uniform sampler2D uFBTexture;

in vec2 fTexCoords;
in vec2 fRes;
in vec2 fMouse;

const float w  = 1.0f;
const float h  = 1.0f;

const vec2 offset[9] = {
    vec2(-w, -h), vec2(0.0f, -h), vec2(w, -h),
    vec2(-w, 0.0f), vec2(0.0f, 0.0f), vec2(w, 0.0f),
    vec2(-w, h), vec2(0.0f, h), vec2(w, h)
};

const float kernel[9] = {
    0.05f,
    0.2f,
    0.05f,
    
    0.2f,
    -1.0f,
    0.2f,
    
    0.05f,
    0.2f,
    0.05f,

};
uint random(uint state){
    state ^= 2747636419u;
    state *= 2654435769u;
    state ^= state >> 16;
    state *= 2654435769u;
    state ^= state >> 16;
    state *= 2654435769u;
    return state;
    //maximum 4294967295
}

void main()
{
    vec2 cP = gl_FragCoord.xy;

    vec3 lap;
    for(int i = 0; i < 9; i++){
        vec3 tmp = texelFetch(uFBTexture, ivec2(cP + offset[i]), 0).rgb;
        lap += vec3(tmp * kernel[i]);
    }

    vec3 color = texture(uFBTexture, fTexCoords).rgb;
    color += texture(uTexture, fTexCoords).rgb;
    FragColor = vec4(color,1.0f);

    //float dist = distance(cP, vec2(700, 500));
    //if(dist < 5){
    //FragColor = vec4(1.0f);
    //}

    float dif = 1.0f;
    FragColor.r = (FragColor.r + ( lap.r * dif)) - uVars.x;
    FragColor.g = (FragColor.g + ( lap.g * dif)) - uVars.x;
    FragColor.b = (FragColor.b + ( lap.b * dif)) - uVars.x;

    FragColor = clamp(FragColor, 0.0f, 1.0f);

   // FragColor = vec4(texelFetch(uFBTexture, ivec2(cP), 0).rgb, 1.0f);
}
