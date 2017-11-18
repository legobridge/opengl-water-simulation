// Main Source File for simulating water in OpenGL
// Authors - Asutosh Sistla, Kushal Agrawal, Suchit Kar
// Date of Completion - 

#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "scene.h"

using namespace std;


// Constants
const unsigned int SCR_W = 1366;
const unsigned int SCR_H = 768;
const unsigned int SCR_MIN_W = 640;
const unsigned int SCR_MIN_H = 360;
const unsigned int SCR_MAX_W = 1366;
const unsigned int SCR_MAX_H = 768;
const float MOUSE_SENSITIVITY = 0.2f;
const char* WINDOW_TITLE = "Water Simulation";

// Mouse cursor positions
float lastX = SCR_W / 2;
float lastY = SCR_H / 2;

// Pointer to a GLFWwindow object
GLFWwindow* window;

// Pointer to the scene object
Scene* myScene;

// Callback Function for Framebuffer Resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// **************************************
// ********* Keypress Processing ********
// - Esc key closes the window
// - W key moves camera forward
// - S key moves camera backward
// - A key makes camera strafe left
// - D key makes camera strafe right
// - P key toggles the day/night cycle
// - [ key slows down the day/night cycle
// - ] key speeds up the day/night cycle
// **************************************
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (key == GLFW_KEY_W && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		myScene->moveIn();
	}
	if (key == GLFW_KEY_S && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		myScene->moveOut();
	}
	if (key == GLFW_KEY_A && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		myScene->strafeLeft();
	}
	if (key == GLFW_KEY_D && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		myScene->strafeRight();
	}
	if (key == GLFW_KEY_P && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		myScene->toggleTime();
	}
	if (key == GLFW_KEY_LEFT_BRACKET && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		myScene->slowDownTime();
	}
	if (key == GLFW_KEY_RIGHT_BRACKET && (action == GLFW_REPEAT || action == GLFW_PRESS))
	{
		myScene->speedUpTime();
	}
}

// **************************************
// ********** Mouse Processing **********
// - Moving the mouse pans the screen in
//   the direction of mouse movement
// **************************************
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
	float xp = (float)xpos;
	float yp = (float)ypos;
	float xoffset = xp - lastX;
	float yoffset = lastY - yp;
	lastX = xp;
	lastY = yp;

	xoffset *= MOUSE_SENSITIVITY;
	yoffset *= MOUSE_SENSITIVITY;

	myScene->pan(xoffset, yoffset);
}

// **************************************
// ********** Mouse Processing **********
// - Scroll Up increases camera speed
// - Scroll Down decreases camera speed
// **************************************
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	myScene->changeSpeed((float)yoffset);
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

	// Window (default: 768p fullscreen) and context creation
	window = glfwCreateWindow(SCR_W, SCR_H, WINDOW_TITLE, glfwGetPrimaryMonitor(), NULL);
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
	glfwSetCursorPosCallback(window, cursor_pos_callback);

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

	return true;
}

int main()
{
	// OpenGL Initialization
	if (!initializeOpenGL())
	{
		return -1;
	}

	// Avoid cursor jump
	glfwSetCursorPos(window, lastX, lastY);

	// Initialize the Scene object
	myScene = new Scene();

	float prev = (float)glfwGetTime();

	// **************** Render Loop ****************
	while (!glfwWindowShouldClose(window))
	{
		float add = 0.0f;
		float cur = (float)glfwGetTime();
		float deltaTime = cur - prev;
		prev = cur;

		if (!myScene->paused)
		{
			add = deltaTime * myScene->timescale;
		}
		myScene->time += add;
		float c = max(0.1f, (float)cos(glm::radians(myScene->time)));
		glm::vec3 backgroundColor = glm::vec3(0.404f * c, 0.684f * c, 0.900f * c);

		// ******** Rendering Commands ********

		// Clear buffers
		glClearColor(backgroundColor.x, backgroundColor.y, backgroundColor.z, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw objects
		myScene->drawObjects();

		// ****** End Rendering Commands ******

		// Check for events, then swap buffers
		glfwPollEvents();
		glfwSwapBuffers(window);
	}

	// Deallocation of Resources
	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}