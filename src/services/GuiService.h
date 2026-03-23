#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <stdexcept>

struct GLFWwindow;

class GuiService
{
public:

    void Initialize(GLFWwindow* window, const char* glslVersion)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        ImGui::StyleColorsDark();

        if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
        {
            throw std::runtime_error("Failed to initialize ImGui GLFW backend");
        }

        if (!ImGui_ImplOpenGL3_Init(glslVersion))
        {
            throw std::runtime_error("Failed to initialize ImGui OpenGL3 backend");
        }
    }

    void BeginFrame() const
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void EndFrame() const
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Shutdown() const
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
};