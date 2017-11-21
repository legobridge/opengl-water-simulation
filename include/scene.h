#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shader.h"
#include "camera.h"
#include "model.h"
#include "object.h"

class Scene
{
public:

	// Screen dimensions
	const unsigned int SCR_W = 1366;
	const unsigned int SCR_H = 768;

	// World dimensions
	const float WORLD_W = 300.0f;
	const float WORLD_H = 100.0f;
	const float WORLD_D = 200.0f;

	// In-Program time
	float time;

	// Timescale measure for day-night cycle
	float timescale;

	// Time pause state
	bool paused;

	// Color map
	std::map<std::string, glm::vec3> colors;

	// Constructor
	Scene();

	void setupGrassObjects();


	void setupTreeObjects();

	// Toggle time (on/off)
	void toggleTime();

	// Slow down time
	void slowDownTime();

	// Speed up time
	void speedUpTime();

	Camera camera;

	Shader modelShader;

	Model grassModel;

	std::vector<Object> grassObjects;

	Model treeModel;

	std::vector<Object> treeObjects;

	// Call rendering functions for all the pre-computed objects
	void drawObjects();
};

#endif