#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Texture.h"
#include <stb/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"
#include "imgui.h"
#include "services/GuiService.h"
#include "gui/MainPanelView.h"

#include "model/CameraModel.h"
#include "viewmodel/CameraViewModel.h"
#include "renderer/Renderer.h"
#include "model/SceneModel.h"

const unsigned int width = 800;
const unsigned int height = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    CameraModel* cameraModel = static_cast<CameraModel*>(glfwGetWindowUserPointer(window));
    if (cameraModel)
    {
        cameraModel->SetAspect(static_cast<float>(width), static_cast<float>(height));
    }
    std::cout << "resizing window" << std::endl;
}

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
    // Initialize GLFW
    if (!glfwInit()) {
        std::cout << "Failed to initialize GLFW\n";
        return -1;
    }

    // Tell GLFW what version of OpenGL we are using
    // In this case we are using OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW we are using the CORE profile
    // So that means we only have the modern functions
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFWwindow object of 800 by 800 pixel
    GLFWwindow* window = glfwCreateWindow(width, height, "WINDOW", NULL, NULL);
    // Error check if the window fails to create
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Introduce the window into the current context
    glfwMakeContextCurrent(window);

    // Load GLAD so it configures OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }

	CameraController cameraController;
    Renderer renderer;
	GuiService imguiService;
	MainPanelView mainPanelView;

	imguiService.Initialize(window, "#version 330");

    // for centering the triangle with different framebuffer size
    // 1) register the resize callback:
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 2) set the initial viewport to the real framebuffer size:
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    std::cout << "Framebuffer size: " << fbWidth << "×" << fbHeight << "\n";
    glViewport(0, 0, fbWidth, fbHeight);

    int winW, winH;
    glfwGetWindowSize(window, &winW, &winH);
    std::cout << "Window size: " << winW <<  "×" << winH << '\n';

    CameraModel cameraModel;
    cameraModel.m_position_ = glm::vec3(0.0f, 0.0f, 2.0f);
    cameraModel.m_forward_  = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraModel.m_up_       = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraModel.m_fov_      = 45.0f;
    cameraModel.m_nearPlane_ = 0.1f;
    cameraModel.m_farPlane_  = 100.0f;
    cameraModel.SetAspect(static_cast<float>(fbWidth), static_cast<float>(fbHeight));

    glfwSetWindowUserPointer(window, &cameraModel);
    // normally simply just glViewport(0,0,width,height);
    // improved by calling glfwGetFramebufferSize and using that for glViewport, which is better on macOS Retina.
    // So on Retina: your viewport handling is more correct (as long as you also use fbWidth/fbHeight for projection).

    // Generates Shader object using shaders default.vert and default.frag
    Shader shaderProgram("/Users/at/LearnOpenGL/resources/shaders/default.vert", "/Users/at/LearnOpenGL/resources/shaders/default.frag");

    Shader lightProgram("/Users/at/LearnOpenGL/resources/shaders/light.vert", "/Users/at/LearnOpenGL/resources/shaders/light.frag");

    Shader cubeProgram("/Users/at/LearnOpenGL/resources/shaders/cube.vert", "/Users/at/LearnOpenGL/resources/shaders/cube.frag");

    // Generates Vertex Array Object and binds it
    VAO VAO1;
    VAO VAO2;
    VAO VAO3;

    // Use FIRST vert array now
    VAO1.Bind();
    // Generates Vertex Buffer Object and links it to vertices
    VBO VBO1(vertices, sizeof(vertices));
    // Generates Element Buffer Object and links it to indices
    EBO EBO1(indices, sizeof(indices));

    // VBO for the light source
    VBO VBO2(light_vertices, sizeof(light_vertices));

    // VBO for cube
    VBO VBO3(cube_vertices, sizeof(cube_vertices));

    // Links VBO to VAO -- how shader receives the vertex data
    // Reads the VBO data, its type, and its attribute stride and offset
    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8*sizeof(float), (void*) 0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8*sizeof(float), (void*)(3*sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8*sizeof(float), (void*)(6*sizeof(float)));
    // Unbind all to prevent accidentally modifying them
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    // Now bind for the lighting cube
    VAO2.Bind();
    VAO2.LinkAttrib(VBO2, 0, 3, GL_FLOAT, 3*sizeof(float), (void*) 0);

    VAO2.Unbind();
    VBO2.Unbind();

    // Now bind for the object cube
    VAO3.Bind();
    VAO3.LinkAttrib(VBO3, 0, 3, GL_FLOAT, 6*sizeof(float), (void*) 0);
    VAO3.LinkAttrib(VBO3, 1, 3, GL_FLOAT, 6*sizeof(float), (void*)(3*sizeof(float)));

    VAO3.Unbind();
    VBO3.Unbind();

    // get a uniform variable called scale from the compiled shader program
    GLint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

    // TEXTURE
    Texture texture(("/Users/at/LearnOpenGL/resources/assets/brick.png"), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    texture.Bind();
    texture.texUnit(shaderProgram, "tex0", 0);

    glEnable(GL_DEPTH_TEST);

    float theta = 0.0f;
    float speed = 5.0f; // deg / sec

    double lastTime = glfwGetTime();
    int frameCount = 0;
    double fps = 0.0;

    // Main while loop
    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        frameCount++;

        if (currentTime - lastTime >= 1.0)
        {
            fps = frameCount / (currentTime - lastTime);
            frameCount = 0;
            lastTime = currentTime;
        }

        glfwPollEvents();
        processInput(window);

		imguiService.BeginFrame();
		mainPanelView.Draw(speed, theta, fps);

        if (!ImGui::GetIO().WantCaptureMouse && !ImGui::GetIO().WantCaptureKeyboard)
        {
            cameraController.HandleInput(window, cameraModel);
        }

        double timeNow = glfwGetTime();
        theta = speed * static_cast<float>(timeNow);

        renderer.BeginFrame();
        renderer.RenderScene(
            cameraModel,
            shaderProgram,
            lightProgram,
            cubeProgram,
            VAO1,
            VAO2,
            VAO3,
            texture,
            uniID,
            sizeof(indices) / sizeof(GLuint),
            theta,
            static_cast<float>(timeNow)
        );
		
		imguiService.EndFrame();

        glfwSwapBuffers(window);
    }

    // Delete all the objects we've created
    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();

    VAO2.Delete();
    VBO2.Delete();

    VAO3.Delete();
    VBO3.Delete();

    texture.Delete();
    shaderProgram.Delete();
    lightProgram.Delete();
    cubeProgram.Delete();

	imguiService.Shutdown();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}