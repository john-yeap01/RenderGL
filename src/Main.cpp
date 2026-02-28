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

const unsigned int width = 800;
const unsigned int height = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
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
	glfwInit();

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
	gladLoadGL();
	
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
	// 	normally simply just glViewport(0,0,width,height);
	// improved by calling glfwGetFramebufferSize and using that for glViewport, which is better on macOS Retina.
	// So on Retina: your viewport handling is more correct (as long as you also use fbWidth/fbHeight for projection).

	Camera camera(fbWidth, fbHeight, glm::vec3(0.0f, 0.0f, 2.0f));

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
	VAO3.LinkAttrib(VBO3, 1, 3, GL_FLOAT, 6*sizeof(float), (void*) 0);

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
	
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();

		camera.Inputs(window);
		// send uniform to the shader
		camera.CameraMatrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		// Only after activating (one float)
		glUniform1f(uniID, 0.5f);
		texture.Bind();

		glm::mat4 model = glm::identity<glm::mat4>();

		double timeNow = glfwGetTime();
		theta = speed * timeNow;
		model = glm::rotate(model, glm::radians(theta), glm::vec3(0.0f, 1.0f, 0.0f));
		unsigned int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		// For UV distortion (float uniform)
		GLint timeLoc = glGetUniformLocation(shaderProgram.ID, "time");
		glUniform1f(timeLoc, (GLfloat)timeNow);

		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);

		VAO1.Unbind();
		
		// NOW ACTIVATE THE LIGHTING SHADER 
		lightProgram.Activate();
		camera.CameraMatrix(45.0f, 0.1f, 100.0f, lightProgram, "camMatrix");

		glm::mat4 cube_model = glm::mat4(1.0f);
		cube_model = glm::translate(cube_model, glm::vec3(2.0f, 1.5f, 1.75f));
		unsigned int cube_modelLoc = glGetUniformLocation(lightProgram.ID, "model");
		glUniformMatrix4fv(cube_modelLoc, 1, GL_FALSE, glm::value_ptr(cube_model));
		
		// DRAW THE LIGHTING CUBE
		VAO2.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		VAO2.Unbind();

		// NOW ACTIVATE THE BASIC CUBE  
		cubeProgram.Activate();
		camera.CameraMatrix(45.0f, 0.1f, 100.0f, cubeProgram, "camMatrix");

		cube_model = glm::mat4(1.0f);
		cube_model = glm::translate(cube_model, glm::vec3(1.0f, 0.5f, 0.5f));
		cube_modelLoc = glGetUniformLocation(cubeProgram.ID, "model");
		glUniformMatrix4fv(cube_modelLoc, 1, GL_FALSE, glm::value_ptr(cube_model));

		cubeProgram.SetFloat("ambientStrength", 0.4f);
		cubeProgram.SetVec3("lightPos", glm::vec3(2.0f, 1.5f, 1.75f)); // same as lamp position
		cubeProgram.SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));
		cubeProgram.SetVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));


		VAO3.Bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		VAO3.Unbind();
	
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
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

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}