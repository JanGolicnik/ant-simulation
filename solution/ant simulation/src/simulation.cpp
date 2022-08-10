#pragma once

#include <simulation.h>
#include <iostream>
#include <core.h>
#include <fstream>
#include <string>

namespace sim {
    GLuint mainShaderId;
    GLuint computeShaderId;

    GLuint computeTex;

    GLuint framebuffer;
    GLuint framebuffertex;

    GLuint agentSSBO;

    const int numAgents = 68000;
    const int texSize = 1000;

    float weight = 5.0f;
    float dist = 20.0f;
    float angle = 15.0f;
    float decay = 3.0f;
    float randomness = 3.0f;

    const OverlayVertex vertices[4] = {
    {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
    {glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
    {glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)},
    {glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
    };
    const int32_t elements[6] = {
        0, 1, 3,
        0, 2, 3
    };

    uint32_t VAO;
    uint32_t VBO;
    uint32_t EBO;

    void setup() {

        createShader();
        createComputeShader();
        createFrameBuffer();

        GLint data;
        glGetIntegerv(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &data);
        std::cout << data << "\n";

        glGenVertexArrays(1, &VAO);

        glBindVertexArray(VAO);

        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(OverlayVertex), 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(OverlayVertex), (void*)sizeof(glm::vec3));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

    }

    void render(ImGuiIO io) {
        //do compute shader
        glUseProgram(computeShaderId);
        glUniform1i(1, framebuffertex);
        glUniform4f(2, weight, dist, angle, randomness);
        glUniform2i(3, texSize, texSize);
        glDispatchCompute(numAgents, 1, 1);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, agentSSBO);

        //render to framebuffer and do mainshader things
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glUseProgram(mainShaderId);
        glBindVertexArray(VAO);
        glUniform2f(1, texSize, texSize);
        glUniform1i(2, computeTex);
        glUniform1i(5, framebuffertex);
        glUniform3f(3, decay / 1000.0f, 0.0f, 0.0f);

        glActiveTexture(GL_TEXTURE0 + computeTex);
        glBindTexture(GL_TEXTURE_2D, computeTex);
        glActiveTexture(GL_TEXTURE0 + framebuffertex);
        glBindTexture(GL_TEXTURE_2D, framebuffertex);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glClearTexImage(computeTex, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

        glUseProgram(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_AutoHideTabBar;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        ImGui::SetNextWindowSizeConstraints(ImVec2(1000, 1000), ImVec2(1000, 1000));
        ImGui::SetNextWindowSizeConstraints(ImVec2(1000, 1000), ImVec2(1000, 1000));
        ImGui::Begin("a", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoCollapse);
        {
            ImGui::BeginChild("ants");
            ImVec2 wsize = ImGui::GetWindowSize();
            ImGui::Image((ImTextureID)framebuffertex, wsize, ImVec2(0, 1), ImVec2(1, 0));
            ImGui::EndChild();
        }
        ImGui::End();

        ImGui::Begin("a", nullptr, ImGuiWindowFlags_NoDocking);
        ImGui::DragFloat("weight", &weight, 0.01f, 0.0f, 360.0f, "%.3f");
        ImGui::DragFloat("dist", &dist, 0.01f, 0.0f, 1000.0f, "%.3f");
        ImGui::DragFloat("angle", &angle, 0.01f, 0.0f, 1000.0f, "%.3f");
        ImGui::DragFloat("randomness", &randomness, 0.01f, 0.0f, 50.0f, "%.3f");
        ImGui::DragFloat("decay", &decay, 0.01f, 0.0f, 50.0f, "%.3f");
        ImGui::End();
        ImGui::End();

    }

    void createShader() {
        const char* fragmentPath = "ants/src/shaders/overlayFrag.frag";
        const char* vertexPath = "ants/src/shaders/overlayVert.vert";

        std::string l;

        std::string fragstr;

        std::ifstream fragmentFile(fragmentPath);
        if (fragmentFile.is_open()) {
            while (std::getline(fragmentFile, l)) {
                fragstr += l;
                fragstr += "\n";
            }
        }
        else {
            std::cout << "error reading fragment shader " << fragmentPath << "\n";
        }

        const char* fragmentShaderSource = fragstr.c_str();

        std::string vertstr;

        std::ifstream vertexFile(vertexPath);
        if (vertexFile.is_open()) {
            while (std::getline(vertexFile, l)) {
                vertstr += l;
                vertstr += "\n";
            }
        }
        else {
            std::cout << "error reading vertex shader " << vertexPath << "\n";
        }

        const char* vertexShaderSource = vertstr.c_str();

        uint32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
        glCompileShader(vertexShader);

        int success;
        char infolog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infolog);
            std::cout << "Error in vertex shader: " << vertexPath << " " << infolog << std::endl;
        }

        uint32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infolog);
            std::cout << "Error in fragment shader: " << fragmentPath << " " << infolog << std::endl;
        }

        mainShaderId = glCreateProgram();
        glAttachShader(mainShaderId, vertexShader);
        glAttachShader(mainShaderId, fragmentShader);
        glLinkProgram(mainShaderId);

        glGetProgramiv(mainShaderId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(mainShaderId, 512, NULL, infolog);
            std::cout << "Error in shader program: " << fragmentPath << " " << vertexPath << " " << infolog << std::endl;
        }

        glDetachShader(mainShaderId, fragmentShader);
        glDetachShader(mainShaderId, vertexShader);
        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
    }

    void createComputeShader() {
        std::string l;

        std::string str;

        const char* path = "ants/src/shaders/computeShader.comp";

        std::ifstream file(path);
        if (file.is_open()) {
            while (std::getline(file, l)) {
                str += l;
                str += "\n";
            }
        }
        else {
            std::cout << "error reading compute shader " << path << "\n";
        }

        const char* source = str.c_str();

        int success;
        char infolog[512];

        uint32 shader = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(shader, 1, &source, NULL);
        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 512, NULL, infolog);
            std::cout << "Error in compute shader: " << path << " " << infolog << std::endl;
        }

        computeShaderId = glCreateProgram();
        glAttachShader(computeShaderId, shader);
        glLinkProgram(computeShaderId);

        glGetProgramiv(computeShaderId, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(computeShaderId, 512, NULL, infolog);
            std::cout << "Error in shader program: " << path << " " << infolog << std::endl;
        }

        glGenTextures(1, &computeTex);
        glActiveTexture(GL_TEXTURE0 + computeTex);
        glBindTexture(GL_TEXTURE_2D, computeTex);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, texSize, texSize, 0, GL_RGBA, GL_FLOAT, NULL);
        glBindImageTexture(1, computeTex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

        //creating an ssbo
        glGenBuffers(1, &agentSSBO);
        static agent agents[numAgents];

        for (int i = 0; i < numAgents; i++) {
            agents[i].values[0] = 500;
            agents[i].values[1] = 500;
            agents[i].values[2] = 360.0f/numAgents * i;
            agents[i].values[3] = 0;
        }

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, agentSSBO);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(agents), agents, GL_DYNAMIC_COPY);
        std::cout << sizeof(agents) << "\n";
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void createFrameBuffer() {
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        glGenTextures(1, &framebuffertex);
        glBindTexture(GL_TEXTURE_2D, framebuffertex);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, texSize, texSize, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebuffertex, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "\nframebuffer error\n";
        }

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
