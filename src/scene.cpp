#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "scene.h"

using namespace std;

Scene::Scene()
	: modelShader("../src/shader/model.vs", "../src/shader/model.fs")
	, terrainModel("../model/terrain/terrain.obj", modelShader)
	, treeModel("../model/tree/tree.obj", modelShader)
	, grassModel("../model/grass/grass.obj", modelShader)
	, waterModel("../model/water/water.obj", modelShader)
{
	time = -30.0f;
	prevTime = (float)glfwGetTime();

	timescale = 16.0f;

	paused = true;

	k = 1;

	waterLevels.resize((size_t)WORLD_D);
	terrainExistence.resize((size_t)WORLD_D);
	for (size_t i = 0; i < waterLevels.size(); i++)
	{
		waterLevels[i].resize((size_t)WORLD_W);
		terrainExistence[i].resize((size_t)WORLD_W);
		for (size_t j = 0; j < waterLevels[i].size(); j++)
		{
			waterLevels[i][j].assign((size_t)WORLD_H, 0);
			terrainExistence[i][j].assign((size_t)WORLD_H, false);
		}
	}
	setupTerrainHeights();

	setupTerrainObject();
	setupTreeObjects();
	setupGrassObjects();
	setupWaterObjects();
}

void Scene::setupTerrainHeights()
{
	ifstream heightMap;
	heightMap.open("../model/terrain/height_map.txt");
	if (!heightMap)
	{
		cout << "Unable to open file height_map.txt" << endl;
	}
	else
	{
		for (size_t i = 0; i < terrainExistence.size(); i++)
		{
			int n;
			heightMap >> n;
			size_t j = 0;
			for (int x = 0; x < n; x++)
			{
				int m, h;
				heightMap >> m >> h;
				while (m--)
				{
					for (int k = 0; k < h; k++)
					{
						terrainExistence[i][j][k] = true;
					}
					j++;
				}
			}
		}
	}
}

void Scene::setupObjects(vector<Object> objects, Model model)
{
	vector<glm::mat4> modelMatrices(objects.size());

	for (size_t i = 0; i < objects.size(); i++)
	{
		modelMatrices[i] = glm::translate(glm::mat4(1.0f), objects[i].position);
		modelMatrices[i] = glm::scale(modelMatrices[i], objects[i].scale);
	}

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, objects.size() * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (size_t i = 0; i < model.meshes.size(); i++)
	{
		unsigned int VAO = model.meshes[i].VAO;
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
	}
}

void Scene::setupTerrainObject()
{
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	Object terrain(&terrainModel, position);
	terrainObjects.push_back(terrain);
	setupObjects(terrainObjects, terrainModel);
}

void Scene::setupTreeObjects()
{
	Object tree1(&treeModel, glm::vec3(9.0f, 8.0f, -7.0f), glm::vec3(2.0f, 2.0f, 2.0f));
	treeObjects.push_back(tree1);
	Object tree2(&treeModel, glm::vec3(13.0f, 7.0f, -4.0f), glm::vec3(3.0f, 3.0f, 3.0f));
	treeObjects.push_back(tree2);
	Object tree3(&treeModel, glm::vec3(-12.0f, 7.0f, -4.0f), glm::vec3(2.5f, 2.5f, 2.5f));
	treeObjects.push_back(tree3);
	Object tree4(&treeModel, glm::vec3(-8.0f, 9.0f, -8.0f), glm::vec3(3.5f, 3.5f, 3.5f));
	treeObjects.push_back(tree4);
	setupObjects(treeObjects, treeModel);
}

void Scene::setupGrassObjects()
{
	for (int i = (int)WORLD_D - 1; i >= 0; i--)
	{
		for (int j = 1; j < 10; j++)
		{
			for (int k = 1; k <= 10; k++)
			{
				if (terrainExistence[i][j][k - 1] && !terrainExistence[i][j][k])
				{
					glm::vec3 position = glm::vec3((float)j + 0.25f - 15.0f, (float)k, (float)i + 0.435f - 10.0f);
					glm::vec3 scale = glm::vec3(0.5f, 0.8f, 0.20f);
					Object water(&grassModel, position, scale);
					grassObjects.push_back(water);
				}
			}
		}
		for (int j = 21; j < (int)WORLD_W - 1; j++)
		{
			for (int k = 1; k <= 10; k++)
			{
				if (terrainExistence[i][j][k - 1] && !terrainExistence[i][j][k])
				{
					glm::vec3 position = glm::vec3((float)j + 0.25f - 15.0f, (float)k, (float)i + 0.435f - 10.0f);
					glm::vec3 scale = glm::vec3(0.5f, 0.8f, 0.20f);
					Object water(&grassModel, position, scale);
					grassObjects.push_back(water);
				}
			}
		}
	}
	setupObjects(grassObjects, grassModel);
}

void Scene::setupWaterObjects()
{
	for (int i = (int)WORLD_D - 1; i >= 0; i--)
	{
		for (int j = 0; j < WORLD_W; j++)
		{
			for (int k = 1; k <= 10; k++)
			{
				glm::vec3 position = glm::vec3((float)j + 0.25f - 15.0f, (float)k, (float)i + 0.435f - 10.0f);
				glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);
				Object water(&waterModel, position, scale);
				waterObjects.push_back(water);
			}
		}
	}
	setupObjects(waterObjects, waterModel);
}

// Add water at the top
void Scene::addWater()
{
	int flow = 100;
	for (size_t j = 12; j < 18; j++)
	{
		waterLevels[0][j][10] = min(255, waterLevels[0][j][9] + flow);
	}
}

// Refresh scales of water blocks to match waterLevels
void Scene::refreshLevels()
{
	int z = 0;
	for (int i = (int)WORLD_D - 1; i >= 0; i--)
	{
		for (int j = 0; j < WORLD_W; j++)
		{
			for (int k = 1; k <= 10; k++)
			{
				float waterPresent = (waterLevels[i][j][k] > 0) ? 1.0f : 0.0f;
				waterObjects[z++].scale = glm::vec3(waterPresent, (float)waterLevels[i][j][k] / 255.0f, waterPresent);
			}
		}
	}
	setupObjects(waterObjects, waterModel);
}

// Update water levels
void Scene::updateWaterObjects()
{
	for (int i = (int)WORLD_D - 1; i >= 0; i--)
	{
		for (int j = (int)(WORLD_W / 2)- 1; j >= 0; j--)
		{
			if (!waterLevels[i][j][k] == 0)
			{
				if (!terrainExistence[i][j][k - 1])
				{
					if (waterLevels[i][j][k - 1] < 255)
					{
						int capacity = 255 - waterLevels[i][j][k - 1];
						int amount = min(capacity, waterLevels[i][j][k]);
						waterLevels[i][j][k] -= amount;
						waterLevels[i][j][k - 1] += amount;
					}
				}
				if (i < (int)WORLD_D - 1 && !terrainExistence[i + 1][j][k])
				{
					if (waterLevels[i + 1][j][k] < waterLevels[i][j][k])
					{
						int capacity = 255 - waterLevels[i + 1][j][k];
						int amount = min(capacity, waterLevels[i][j][k]);
						waterLevels[i][j][k] -= amount;
						waterLevels[i + 1][j][k] += amount;
					}
				}
				if (j > 0 && !terrainExistence[i][j - 1][k])
				{
					if (waterLevels[i][j - 1][k] < waterLevels[i][j][k])
					{
						int capacity = 255 - waterLevels[i][j - 1][k];
						int amount = min(capacity, waterLevels[i][j][k] / 2);
						waterLevels[i][j][k] -= amount;
						waterLevels[i][j - 1][k] += amount;
					}
				}
				if (j < (int)WORLD_W - 1 && !terrainExistence[i][j + 1][k])
				{
					if (waterLevels[i][j + 1][k] < waterLevels[i][j][k])
					{
						int capacity = 255 - waterLevels[i][j + 1][k];
						int amount = min(capacity, waterLevels[i][j][k] / 2);
						waterLevels[i][j][k] -= amount;
						waterLevels[i][j + 1][k] += amount;
					}
				}
				if (i > 0 && !terrainExistence[i - 1][j][k])
				{
					if (waterLevels[i - 1][j][k] < waterLevels[i][j][k])
					{
						int capacity = 255 - waterLevels[i - 1][j][k];
						int amount = min(capacity, waterLevels[i][j][k] / 2);
						waterLevels[i][j][k] -= amount;
						waterLevels[i - 1][j][k] += amount;
					}
				}
			}
			if (!waterLevels[i][(int)WORLD_W - 1 - j][k] == 0)
			{
				if (!terrainExistence[i][(int)WORLD_W - 1 - j][k - 1])
				{
					if (waterLevels[i][(int)WORLD_W - 1 - j][k - 1] < 255)
					{
						int capacity = 255 - waterLevels[i][(int)WORLD_W - 1 - j][k - 1];
						int amount = min(capacity, waterLevels[i][(int)WORLD_W - 1 - j][k]);
						waterLevels[i][(int)WORLD_W - 1 - j][k] -= amount;
						waterLevels[i][(int)WORLD_W - 1 - j][k - 1] += amount;
					}
				}
				if (i < (int)WORLD_D - 1 && !terrainExistence[i + 1][(int)WORLD_W - 1 - j][k])
				{
					if (waterLevels[i + 1][(int)WORLD_W - 1 - j][k] < waterLevels[i][(int)WORLD_W - 1 - j][k])
					{
						int capacity = 255 - waterLevels[i + 1][(int)WORLD_W - 1 - j][k];
						int amount = min(capacity, waterLevels[i][(int)WORLD_W - 1 - j][k]);
						waterLevels[i][(int)WORLD_W - 1 - j][k] -= amount;
						waterLevels[i + 1][(int)WORLD_W - 1 - j][k] += amount;
					}
				}
				if ((int)WORLD_W - 1 - j < (int)WORLD_W - 1 && !terrainExistence[i][(int)WORLD_W - 1 - j + 1][k])
				{
					if (waterLevels[i][(int)WORLD_W - 1 - j + 1][k] < waterLevels[i][(int)WORLD_W - 1 - j][k])
					{
						int capacity = 255 - waterLevels[i][(int)WORLD_W - 1 - j + 1][k];
						int amount = min(capacity, waterLevels[i][(int)WORLD_W - 1 - j][k] / 2);
						waterLevels[i][(int)WORLD_W - 1 - j][k] -= amount;
						waterLevels[i][(int)WORLD_W - 1 - j + 1][k] += amount;
					}
				}
				if ((int)WORLD_W - 1 - j > 0 && !terrainExistence[i][(int)WORLD_W - 1 - j - 1][k])
				{
					if (waterLevels[i][(int)WORLD_W - 1 - j - 1][k] < waterLevels[i][(int)WORLD_W - 1 - j][k])
					{
						int capacity = 255 - waterLevels[i][(int)WORLD_W - 1 - j - 1][k];
						int amount = min(capacity, waterLevels[i][(int)WORLD_W - 1 - j][k] / 2);
						waterLevels[i][(int)WORLD_W - 1 - j][k] -= amount;
						waterLevels[i][(int)WORLD_W - 1 - j - 1][k] += amount;
					}
				}
				if (i > 0 && !terrainExistence[i - 1][(int)WORLD_W - 1 - j][k])
				{
					if (waterLevels[i - 1][(int)WORLD_W - 1 - j][k] < waterLevels[i][(int)WORLD_W - 1 - j][k])
					{
						int capacity = 255 - waterLevels[i - 1][(int)WORLD_W - 1 - j][k];
						int amount = min(capacity, waterLevels[i][(int)WORLD_W - 1 - j][k] / 2);
						waterLevels[i][(int)WORLD_W - 1 - j][k] -= amount;
						waterLevels[i - 1][(int)WORLD_W - 1 - j][k] += amount;
					}
				}
			}
		}
	}
}

// Toggle time (on/off)
void Scene::toggleTime()
{
	paused = !paused;
}

// Slow down time
void Scene::slowDownTime()
{
	timescale = max(0.0f, timescale - 2.0f);
}

// Speed up time
void Scene::speedUpTime()
{
	timescale = min(100.0f, timescale + 2.0f);
}

// Update current time
void Scene::updateTime()
{
	float add = 0.0f;
	float cur = (float)glfwGetTime();
	float deltaTime = cur - prevTime;
	prevTime = cur;

	if (!paused)
	{
		add = deltaTime * timescale;
	}
	time += add;
}

// Driver function to perform routine tasks
void Scene::drawObjects()
{
	updateTime();
	float cosine = cos(glm::radians(time));
	float sine = sin(glm::radians(time));
	float sunX = WORLD_W * sine;
	float sunY = WORLD_W * cosine;
	float lightVal = max(0.3f, cosine);
	glm::vec3 lightPos(sunX, sunY, 0.0f);

	// Clear buffers
	glClearColor(lightVal, lightVal, lightVal, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup shader
	modelShader.use();

	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_W / (float)SCR_H, 0.1f, 10.0f * WORLD_W);
	glm::mat4 view = camera.GetViewMatrix();
	modelShader.setMat4("projection", projection);
	modelShader.setMat4("view", view);

	modelShader.setVec3("lightPos", lightPos);
	modelShader.setVec3("viewPos", camera.position);
	modelShader.setVec3("lightColor", glm::vec3(lightVal, lightVal, lightVal));

	updateWaterObjects();
	k++;
	if (k == 11)
	{
		k = 1;
		addWater();
		refreshLevels();
	}

	// Draw objects
	terrainModel.draw(terrainObjects.size());
	grassModel.draw(grassObjects.size());
	waterModel.draw(waterObjects.size());
	treeModel.draw(treeObjects.size());
}