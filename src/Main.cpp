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
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "model/CameraModel.h"
#include "viewmodel/CameraViewModel.h"

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

GLfloat vertices[] = {
    // positions            // colors         // tex coords (UVs) -- for each vertex, this 2d coord on the texture corresponds to the pixel 3d position
     0.5f,  0.0f,  0.5f,     1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // base front-right
     0.5f,  0.0f, -0.5f,     0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // base back-right
    -0.5f,  0.0f, -0.5f,     0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // base back-left
    -0.5f,  0.0f,  0.5f,     1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // base front-left

     0.0f,  0.8f,  0.0f,     1.0f, 0.0f, 1.0f,  0.5f, 0.5f  // apex (top)
};

GLfloat light_vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

GLfloat cube_vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

GLuint indices[] = {
    // base (two triangles)
    0, 1, 2,
    2, 3, 0,

    // sides (4 triangles)
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};



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

	//Load GLAD so it configures OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	
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

    CameraController cameraController;

    glfwSetWindowUserPointer(window, &cameraModel);
	// 	normally simply just glViewport(0,0,width,height);
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
	
	//TEXTURE
	Texture texture(("/Users/at/LearnOpenGL/resources/assets/brick.png"), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	texture.Bind();
	texture.texUnit (shaderProgram, "tex0", 0);

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

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Control Panel");
		ImGui::Text("OpenGL + ImGui is running");
		ImGui::SliderFloat("Speed", &speed, 0.0f, 20.0f);
		ImGui::Text("Theta: %.2f", theta);
		ImGui::Text("FPS: %.1f", fps);
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
		ImGui::End();

		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shaderProgram.Activate();

		if (!ImGui::GetIO().WantCaptureMouse && !ImGui::GetIO().WantCaptureKeyboard)
		{
			cameraController.HandleInput(window, cameraModel);
		}
		// camera.CameraMatrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");
		glm::mat4 view = cameraModel.GetView();
		glm::mat4 proj = cameraModel.GetProjection();
		glm::mat4 camMatrix = proj * view;
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "camMatrix"), 1, GL_FALSE, glm::value_ptr(camMatrix));

		glUniform1f(uniID, 0.5f);
		texture.Bind();

		glm::mat4 model = glm::identity<glm::mat4>();

		double timeNow = glfwGetTime();
		theta = speed * timeNow;
		model = glm::rotate(model, glm::radians(theta), glm::vec3(0.0f, 1.0f, 0.0f));
		shaderProgram.SetMat4("model", model);

		GLint timeLoc = glGetUniformLocation(shaderProgram.ID, "time");
		glUniform1f(timeLoc, (GLfloat)timeNow);

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);
		VAO1.Unbind();

		lightProgram.Activate();
        glUniformMatrix4fv(
            glGetUniformLocation(lightProgram.ID, "camMatrix"),
            1,
            GL_FALSE,
            glm::value_ptr(camMatrix)
        );

		float radius = 2.0f;
		glm::vec3 lightPos = glm::vec3(
			radius * cos(glm::radians(theta)),
			1.5f,
			radius * sin(glm::radians(theta))
		);

		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.3f));
		lightProgram.SetMat4("model", lightModel);

		VAO2.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		VAO2.Unbind();

		cubeProgram.Activate();
		glUniformMatrix4fv(
            glGetUniformLocation(cubeProgram.ID, "camMatrix"),
            1,
            GL_FALSE,
            glm::value_ptr(camMatrix)
        );

		glm::mat4 cubeModel = glm::mat4(1.0f);
		cubeModel = glm::translate(cubeModel, glm::vec3(1.0f, 0.5f, 0.5f));

		cubeProgram.SetMat4("model", cubeModel);
		cubeProgram.SetVec3("viewPos", cameraModel.m_position_);
		
		cubeProgram.SetFloat("ambientStrength", 0.1f);
		cubeProgram.SetVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
		cubeProgram.SetVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
		cubeProgram.SetVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		cubeProgram.SetFloat("material.shininess", 32.0f);

		cubeProgram.SetVec3("light.position", lightPos);
		cubeProgram.SetVec3("light.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		cubeProgram.SetVec3("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		cubeProgram.SetVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		VAO3.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		VAO3.Unbind();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}