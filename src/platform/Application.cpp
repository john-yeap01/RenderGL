#include "platform/Application.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "view/ControlPanelView.h"
#include "view/SceneRenderer.h"
#include "viewmodel/InputState.h"
#include "viewmodel/SceneViewModel.h"

#ifdef RENDERGL_HAS_IMGUI
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#endif

namespace
{
constexpr int kWindowWidth = 800;
constexpr int kWindowHeight = 800;

InputState BuildInputState(GLFWwindow* window, bool captureCameraInput)
{
    InputState inputState {};
    inputState.closeRequested = glfwWindowShouldClose(window);
    inputState.moveForward = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
    inputState.moveBackward = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;
    inputState.moveLeft = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS;
    inputState.moveRight = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS;
    inputState.moveUp = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS;
    inputState.moveDown = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS;
    inputState.boost = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;
    inputState.lookActive = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    inputState.captureCameraInput = captureCameraInput;

    glfwGetWindowSize(window, &inputState.windowWidth, &inputState.windowHeight);
    glfwGetFramebufferSize(window, &inputState.framebufferWidth, &inputState.framebufferHeight);
    glfwGetCursorPos(window, &inputState.mouseX, &inputState.mouseY);
    return inputState;
}

void RecenterCursor(GLFWwindow* window, const InputState& inputState)
{
    glfwSetCursorPos(window, inputState.windowWidth * 0.5, inputState.windowHeight * 0.5);
}
}

Application::Application() = default;

int Application::Run()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(kWindowWidth, kWindowHeight, "RenderGL MVVM", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, &Application::FramebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD\n";
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

#ifdef RENDERGL_HAS_IMGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
#endif

    int framebufferWidth = 0;
    int framebufferHeight = 0;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    SceneViewModel sceneViewModel(framebufferWidth, framebufferHeight);
    SceneRenderer renderer(ResolveResourceRoot());
    ControlPanelView controlPanelView;

    float previousTime = static_cast<float>(glfwGetTime());

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

#ifdef RENDERGL_HAS_IMGUI
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        const bool captureCameraInput = !(ImGui::GetIO().WantCaptureMouse || ImGui::GetIO().WantCaptureKeyboard);
#else
        const bool captureCameraInput = true;
#endif

        const float currentTime = static_cast<float>(glfwGetTime());
        const float deltaSeconds = currentTime - previousTime;
        previousTime = currentTime;

        InputState inputState = BuildInputState(window, captureCameraInput);
        sceneViewModel.ApplyInput(inputState, deltaSeconds);
        sceneViewModel.Update(currentTime);

        if (inputState.lookActive && captureCameraInput)
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            RecenterCursor(window, inputState);
        }
        else
        {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }

#ifdef RENDERGL_HAS_IMGUI
        controlPanelView.Draw(sceneViewModel);
#endif

        renderer.Render(sceneViewModel.BuildRenderState());

#ifdef RENDERGL_HAS_IMGUI
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

        glfwSwapBuffers(window);
    }

#ifdef RENDERGL_HAS_IMGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

std::filesystem::path Application::ResolveResourceRoot() const
{
    if (const char* configuredRoot = std::getenv("RENDERGL_RESOURCE_ROOT"))
    {
        return configuredRoot;
    }
#ifdef RENDERGL_RESOURCE_DIR
    return std::filesystem::path(RENDERGL_RESOURCE_DIR);
#else
    return std::filesystem::path("resources");
#endif
}

void Application::FramebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    (void)window;
    glViewport(0, 0, width, height);
}
