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

// Vertices coordinates
// triangle
// GLfloat vertices[] = {
//     // Positions                                 // Colors (R, G, B)
//     -0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,      1.0f, 0.0f, 0.0f,  // Red     - Lower left corner
//      0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f,      0.0f, 1.0f, 0.0f,  // Green   - Lower right corner
//      0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f,  0.0f, 0.0f, 1.0f,  // Blue    - Upper corner
//     -0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,   1.0f, 1.0f, 0.0f,  // Yellow  - Inner left
//      0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f,   0.0f, 1.0f, 1.0f,  // Cyan    - Inner right
//      0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f,      1.0f, 0.0f, 1.0f   // Magenta - Inner down
// };

// square 
// GLfloat vertices[] = {
//     // positions          // colors           // texture coords
//      0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
//      0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
//     -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
//     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
// };

GLfloat vertices[] = {
    // positions            // colors         // tex coords
     0.5f,  0.0f,  0.5f,     1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // base front-right
     0.5f,  0.0f, -0.5f,     0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // base back-right
    -0.5f,  0.0f, -0.5f,     0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // base back-left
    -0.5f,  0.0f,  0.5f,     1.0f, 1.0f, 0.0f,  0.0f, 1.0f, // base front-left

     0.0f,  0.8f,  0.0f,     1.0f, 0.0f, 1.0f,  0.5f, 0.5f  // apex (top)
};

// Indices for vertices order
//triangle
// GLuint indices[] =
// {
// 	0, 3, 5, // Lower left triangle
// 	3, 2, 4, // Lower right triangle
// 	5, 4, 1 // Upper triangle
// };

//square
// GLuint indices[] = {
//     0, 1, 2,   // first triangle (lower-left, lower-right, upper-right)
//     2, 3, 0    // second triangle (upper-right, upper-left, lower-left)
// };

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



	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO to VAO -- how shader receives the vertex data
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8*sizeof(float), (void*) 0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8*sizeof(float), (void*)(3*sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8*sizeof(float), (void*)(6*sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	// get a uniform variable called scale from the compiled shader program
	GLint uniID = glGetUniformLocation(shaderProgram.ID, "scale");
	
	//TEXTURE
	Texture texture(("/Users/at/LearnOpenGL/resources/assets/brick.png"), GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
	texture.Bind();
	texture.texUnit (shaderProgram, "tex0", 0);

	glEnable(GL_DEPTH_TEST);
	
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		// Clean the back buffer and assign the new color to it
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();


		camera.Inputs(window);
		// send uniform to the shader
		camera.Matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

		// Only after activating (one float)
		glUniform1f(uniID, 0.5f);
		texture.Bind();
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		// Draw primitives, number of indices, datatype of indices, index of indices
		// glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);
		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);
		// Take care of all GLFW events
		glfwPollEvents();
	}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	texture.Delete();
	shaderProgram.Delete();
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}