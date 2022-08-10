#pragma once

#include <imgui.h>

namespace sim {
    void setup();
    void render(ImGuiIO io);
    void createShader();
    void createComputeShader();
    void createFrameBuffer();
};
