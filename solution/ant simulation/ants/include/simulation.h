#pragma once

#include <imgui.h>
#include <core.h>

namespace sim {
    void setup();
    void compute(GLFWwindow* window);
    void createShader();
    void createComputeShader();
    void createFrameBuffer();
    void mbCallback(GLFWwindow* window, int button, int action, int mods);
    void setupSim(int sim);
    void clearEverything();
    void renderGUI(ImGuiIO io, GLFWwindow* window, int * updateSpeed);

};
