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
	const float WORLD_W = 30.0f;
	const float WORLD_H = 20.0f;
	const float WORLD_D = 20.0f;

	// In-Program timekeeping variables
	float time; 
	float prevTime;
	float timescale;

	// Time pause state
	bool paused;

	Camera camera;

	Shader modelShader;

	Model terrainModel;
	Model treeModel;


	std::vector<Object> terrainObjects;
	std::vector<Object> treeObjects;

	std::vector<std::vector<bool> > occupied;

	// Constructor
	Scene();

	// Generic setup function
	void Scene::setupObjects(std::vector<Object> objects, Model model);

	// Prepare terrain object for instantiation
	void setupTerrainObject();

	// Prepare tree objects for instantiation
	void setupTreeObjects();

	// Toggle time (on/off)
	void toggleTime();

	// Slow down time
	void slowDownTime();

	// Speed up time
	void speedUpTime();

	// Update current time
	void updateTime();

	// Call rendering functions for all the pre-computed objects
	void drawObjects();
};

#endif