#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <fstream>
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

	// Current height of water level to be computed
	int k;

	Camera camera;

	Shader modelShader;

	Model terrainModel;
	Model waterModel;
	Model treeModel;
	Model grassModel;

	std::vector<Object> terrainObjects;
	std::vector<Object> treeObjects;
	std::vector<Object> waterObjects;
	std::vector<Object> grassObjects;

	std::vector<std::vector<std::vector<bool> > > terrainExistence;
	std::vector<std::vector<std::vector<int> > > waterLevels;

	// Constructor
	Scene();

	// Generic setup function
	void setupObjects(std::vector<Object> objects, Model model);

	// Read in the heightmap and setup terrain heights
	void setupTerrainHeights();

	// Prepare terrain object for instantiation
	void setupTerrainObject();

	// Prepare tree objects for instantiation
	void setupTreeObjects();

	// Prepare tree objects for instantiation
	void setupGrassObjects();

	// Prepare water blocks for instantiation
	void setupWaterObjects();

	// Add water at the top
	void addWater();

	// Refresh scales of water blocks to match waterLevels
	void refreshLevels();

	// Update water levels
	void updateWaterObjects();

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