#version 450 core
layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec2 vTexCoords;

layout (location = 0) uniform mat4 uTransform;
layout (location = 1) uniform mat4 uView;
layout (location = 2) uniform mat4 uProjection;

out vec2 fTexCoords;

void main() {
	gl_Position = uProjection * uView * (uTransform * vec4(vPosition, 1.0));
	fTexCoords = vTexCoords;
}