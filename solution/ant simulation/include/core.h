#ifndef CORE_H
#define CORE_H

// Glad and GLFW
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard library stuff
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <string>
#include <vector>
#include <array>
#include <unordered_map>
//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef uint64_t uint64;
typedef uint32_t uint32;
typedef uint16_t uint16;
typedef uint8_t uint8;

typedef int64_t int64;
typedef int32_t int32;
typedef int16_t int16;
typedef int8_t int8;

struct OverlayVertex {
	glm::vec3 position;
	glm::vec2 texCoords;
};

struct agent {
    float values[4];
};

#endif 
