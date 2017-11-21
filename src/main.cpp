// Main Source File for simulating water in OpenGL
// Authors - Asutosh Sistla, Kushal Agrawal, Suchit Kar
// Date of Completion - 

#include <iostream>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
		 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "camera.h"
#include "scene.h"

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
Camera* camera;
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
		camera->ProcessKeyboardStrafe(FORWARD);
	}
	if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		camera->ProcessKeyboardStrafe(BACKWARD);
	}
	if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		camera->ProcessKeyboardStrafe(LEFT);
	}
	if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		camera->ProcessKeyboardStrafe(RIGHT);
	}
	/*if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		myScene->slowDownTime();
	}
	if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		myScene->speedUpTime();
	}*/
	if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		camera->changeCameraSpeed(0.5f);
	}
	if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		camera->changeCameraSpeed(-0.5f);
	}
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
	camera->ProcessMouseMovement(xoffset, yoffset);
}

// **************************************
// ********* Scroll Processing **********
// - Scroll Up zooms in to the scene
// - Scroll Down zooms out of the scene
// **************************************
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->ProcessMouseScroll((float)yoffset);
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
	/*glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);*/

	// Enable alpha blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

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


	// Initialize the Scene object
	Scene myScene;

	camera = &(myScene.camera);

	// **************** Render Loop ****************
    while (!glfwWindowShouldClose(window))
    {
		myScene.drawObjects();
		// Check for events, then swap buffers
		glfwPollEvents();
        glfwSwapBuffers(window);
    }

	// Deallocation of Resources
	glfwDestroyWindow(window);
	glfwTerminate();
    return 0;
}