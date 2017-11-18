#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "renderer.h"
#include "entity.h"
#include "object.h"

class Scene
{
public:

	// World dimensions
	const float WORLD_W = 3000.0f;
	const float WORLD_H = 1000.0f;
	const float WORLD_D = 2000.0f;

	// Master renderer object
	Renderer renderer;

	// In-Program time
	float time;

	// Timescale measure for day-night cycle
	float timescale;

	// Time pause state
	bool paused;

	// Base camera movement speed
	float cameraSpeed;

	// Euler angles
	float pitch;
	float yaw;
	float roll;

	// Camera properties
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	// Test red cube
	Entity* redCube;
	// Blender model
	// Entity blenderEntity;

	// Color map
	std::map<std::string, glm::vec3> colors;

	// Constructor
	Scene();

	// Toggle time (on/off)
	void toggleTime();

	// Slow down time
	void slowDownTime();

	// Speed up time
	void speedUpTime();

	// Change camera movement speed
	void changeSpeed(float by);

	// Move into the scene
	void moveIn();

	// Move out of the scene
	void moveOut();

	// Strafe to the left
	void strafeLeft();

	// Strafe to the right
	void strafeRight();
	
	// Pan the camera
	void pan(float xoffset, float yoffset);

	// Draw Blender object
	// void drawBlenderObject();

	// Call rendering functions for all the pre-computed objects
	void drawObjects();
};

#endif