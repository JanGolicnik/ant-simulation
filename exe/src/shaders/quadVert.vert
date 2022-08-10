#version 430 core
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec2 aTexCoords;

layout (location = 0) uniform vec2 uMouse;
layout (location = 1) uniform vec2 uRes;

out vec2 fTexCoords;
out vec2 fRes;
out vec2 fMouse;

void main() {
	gl_Position = vec4(aPosition, 1.0f);
	fTexCoords = aTexCoords;
	fMouse = uMouse;
	fRes = uRes;
}