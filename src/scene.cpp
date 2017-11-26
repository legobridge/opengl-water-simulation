#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "scene.h"

using namespace std;

Scene::Scene()
	: modelShader("../src/shader/model.vs", "../src/shader/model.fs")
	, terrainModel("../model/terrain/terrain.obj", modelShader)
	, treeModel("../model/tree/tree.obj", modelShader)
	, waterModel("../model/water/water.obj", modelShader)
{
	time = -30.0f;
	prevTime = (float)glfwGetTime();

	timescale = 16.0f;

	paused = true;

	lastWaterUpdateTime = 0.0f;

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
	for (int i = 0; i < 1; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			for (int k = 0; k < 1; k++)
			{
				glm::vec3 position = glm::vec3(0.0f, 10.0f, 0.0f);
				Object tree(&treeModel, position);
				treeObjects.push_back(tree);
			}
		}
	}
	setupObjects(treeObjects, treeModel);
}

void Scene::setupWaterObjects()
{
	for (int i = (int)WORLD_D - 1; i >= 0; i--)
	{
		for (int j = 0; j < WORLD_W; j++)
		{
			for (int k = 1; k < 10; k++)
			{
				glm::vec3 position = glm::vec3((float)j + 0.25f - 15.0f, (float)k + 1.0f, (float)i + 0.35f - 10.0f);
				glm::vec3 scale = glm::vec3(0.0f, 0.0f, 0.0f);
				Object water(&waterModel, position, scale);
				waterObjects.push_back(water);
			}
		}
	}
	setupObjects(waterObjects, waterModel);
}

// Update water levels
void Scene::updateWaterObjects()
{
	float t = (float)glfwGetTime();
	for (int i = (int)WORLD_D - 1; i >= 0; i--)
	{
		for (int k = 1; k <= 10; k++)
		{
			for (int j = 0; j < WORLD_W; j++)
			{
				if (t - lastWaterUpdateTime < 0.1f)
				{
					continue;
				}
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
				int diffL = (j > 0 && !terrainExistence[i][j - 1][k]) ? (waterLevels[i][j][k] - waterLevels[i][j - 1][k]) : 0;
				int diffR = (j < (int)WORLD_W - 1 && !terrainExistence[i][j + 1][k]) ? (waterLevels[i][j][k] - waterLevels[i][j + 1][k]) : 0;
				if (diffL > diffR)
				{
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
				}
				else
				{
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
		}
	}
	int flow = 100;
	for (size_t i = 12; i < 18; i++)
	{
		if (t - lastWaterUpdateTime < 0.1f)
		{
			continue;
		}
		waterLevels[0][i][9] = min(255, waterLevels[0][i][9] + flow);
	}
	int z = 0;
	for (int i = (int)WORLD_D - 1; i >= 0; i--)
	{
		for (int j = 0; j < WORLD_W; j++)
		{
			for (int k = 1; k < 10; k++)
			{
				float waterPresent = (waterLevels[i][j][k] > 0) ? 1.0f : 0.0f;
				waterObjects[z++].scale = glm::vec3(waterPresent, (float)waterLevels[i][j][k] / 255.0f, waterPresent);
			}
		}
	}
	setupObjects(waterObjects, waterModel);
	if (t - lastWaterUpdateTime > 0.1f)
	{
		lastWaterUpdateTime = (float)glfwGetTime();
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
	float lightVal = max(0.1f, cosine);
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

	// Draw objects
	terrainModel.draw(terrainObjects.size());
	// treeModel.draw(treeObjects.size());
	waterModel.draw(waterObjects.size());
}