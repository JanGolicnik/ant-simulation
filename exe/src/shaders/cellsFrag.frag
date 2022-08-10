#version 430 core

out vec4 FragColor;

layout (location = 2) uniform sampler2D uTexture;
layout (location = 3) uniform vec3 uVars;
layout (location = 4) uniform bool uDraw;
layout (location = 5) uniform vec3 kernel1;
layout (location = 6) uniform vec3 kernel2;
layout (location = 7) uniform vec3 kernel3;

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

void main()
{

const float kernel[9] = {
    kernel1.x,
    kernel1.y,
    kernel1.z,

    kernel2.x,
    kernel2.y,
    kernel2.z,

    kernel3.x,
    kernel3.y,
    kernel3.z
};

    vec2 cP = gl_FragCoord.xy;

    vec2 st = cP / fRes;

    FragColor = texture(uTexture, st);

    vec3 nValues[9];

    vec3 lap;
    for(int i = 0; i < 9; i++){
        vec3 tmp = texelFetch(uTexture, ivec2(cP + offset[i]), 0).rgb;
        lap += vec3(tmp * kernel[i]);
    }

    float diffRA = 1.0f;
    float diffRB = 0.5f;

    float A = FragColor.r;
    float B = FragColor.g;

    A = A + ( ((  lap.r   ) * diffRA ) - A * B * B  +  uVars.x * ( 1- A) );
    B = B + ( ((  lap.g   ) * diffRB ) + A * B * B - ( uVars.x + uVars.y ) * B );

    A = clamp(A, 0.0f, 1.0f);
    B = clamp(B, 0.0f, 1.0f);

    if(uDraw){
        if(distance(st, vec2(fMouse.x, 1-fMouse.y)) < uVars.z){
            B = 1.0f;
        }
    }
    FragColor = vec4(A, B, 0.0f, 1.0f);
}
