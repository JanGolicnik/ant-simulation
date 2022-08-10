#pragma once

#include <imgui.h>
#include <core.h>

namespace sim {
    void setup();
    void compute(GLFWwindow* window);
    void createShader(const char* path, GLuint* id);
    void createComputeShader(const char* path, GLuint* id);
    void createFrameBuffer(GLuint *framebuffer, GLuint *tex);
    void mbCallback(GLFWwindow* window, int button, int action, int mods);
    void setupSim(int sim);
    void clearEverything();
    void renderGUI(ImGuiIO io, GLFWwindow* window, int * updateSpeed);
    void createCShaderBuffers();

};
