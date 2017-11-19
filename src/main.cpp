// Main Source File for simulating water in OpenGL
// Authors - Asutosh Sistla, Kushal Agrawal, Suchit Kar
// Date of Completion - 

#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
		 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shader.h"
#include "camera.h"
#include "model.h"

using namespace std;

// Constants
const unsigned int SCR_W = 1366;
const unsigned int SCR_H = 768;
const unsigned int SCR_MIN_W = 640;
const unsigned int SCR_MIN_H = 360;
const unsigned int SCR_MAX_W = 1366;
const unsigned int SCR_MAX_H = 768;
const char* WINDOW_TITLE = "Water Simulation";

// Camera
Camera camera;
float lastX = SCR_W / 2.0f;
float lastY = SCR_H / 2.0f;
bool firstMouse = true;

// Pointer to a GLFWwindow object
GLFWwindow* window;

// Pointer to the scene object
// Scene* myScene;

// Callback Function for Framebuffer Resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// ************************************************
// ************* Keypress Processing **************
// - Esc key closes the window
// - W key moves camera forward
// - S key moves camera backward
// - A key makes camera strafe left
// - D key makes camera strafe right
// - LEFT ARROW key slows down the day/night cycle
// - RIGHT ARROW  key speeds up the day/night cycle
// ************************************************
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		camera.ProcessKeyboard(FORWARD);
	}
	if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		camera.ProcessKeyboard(BACKWARD);
	}
	if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		camera.ProcessKeyboard(LEFT);
	}
	if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		camera.ProcessKeyboard(RIGHT);
	}
	/*if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		myScene->slowDownTime();
	}
	if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		myScene->speedUpTime();
	}*/
}

// **************************************
// ********** Mouse Processing **********
// - Moving the mouse pans the screen in
//   the direction of mouse movement
// **************************************
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}

// **************************************
// ********* Scroll Processing **********
// - Scroll Up zooms in to the scene
// - Scroll Down zooms out of the scene
// **************************************
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}

// OpenGL Initialization
bool initializeOpenGL()
{
	// GLFW Initialization
	if (!glfwInit())
	{
		cout << "Initialization failed" << endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Window (default: 768p fullscreen) and context creationglfwGetPrimaryMonitor()
	window = glfwCreateWindow(SCR_W, SCR_H, WINDOW_TITLE, NULL, NULL);
	if (!window)
	{
		cout << "Window or context creation failed" << endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);

	// Disable cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Register callback function for Window Resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// Register callback function for keypresses
	glfwSetKeyCallback(window, key_callback);

	// Register callback function for mouse movement
	glfwSetCursorPosCallback(window, mouse_callback);

	// Register callback function for scroll wheel movement
	glfwSetScrollCallback(window, scroll_callback);

	// Fix Aspect Ratio to 16:9
	glfwSetWindowAspectRatio(window, 16, 9);

	// Set Minimum Limit for Window Size to 360p
	glfwSetWindowSizeLimits(window, SCR_MIN_W, SCR_MIN_H, SCR_MAX_W, SCR_MAX_H);

	//  Load OpenGL function pointers using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	// Enable depth testing
	glEnable(GL_DEPTH_TEST);

	// Enable backface culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Viewport Settings
	glViewport(0, 0, SCR_W, SCR_H);

	// Avoid cursor jump
	glfwSetCursorPos(window, lastX, lastY);

	return true;
}

int main()
{
	// OpenGL Initialization
	if (!initializeOpenGL())
	{
		return -1;
	}

    Shader modelShader("../src/shader/model.vs", "../src/shader/model.fs");

    Model ourModel("../model/nanosuit/nanosuit.obj");

	// Initialize the Scene object
	// myScene = new Scene();

	// **************** Render Loop ****************
    while (!glfwWindowShouldClose(window))
    {
		// ******** Rendering Commands ********
		// Clear buffers
        glClearColor(0.05f, 0.15f, 0.35f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        modelShader.use();

        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_W / (float)SCR_H, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        modelShader.setMat4("projection", projection);
        modelShader.setMat4("view", view);

		// Draw objects
        glm::mat4 model;
        model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
        modelShader.setMat4("model", model);
        ourModel.Draw(modelShader);
		// ****** End Rendering Commands ******
		
		// Check for events, then swap buffers
		glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}