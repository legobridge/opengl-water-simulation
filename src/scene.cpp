#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "scene.h"

using namespace std;

Scene::Scene()
	: modelShader("../src/shader/model.vs", "../src/shader/model.fs")
	, terrainModel("../model/terrain/block.obj", modelShader)
	, treeModel("../model/tree/tree.obj", modelShader)
{
	time = 0.0f;
	prevTime = (float)glfwGetTime();

	timescale = 16.0f;

	paused = true;

	occupied.resize((size_t)WORLD_W);
	for (size_t i = 0; i < occupied.size(); i++)
	{
		occupied[i].assign((size_t)WORLD_D, false);
	}

	setupTerrainObject();
	setupTreeObjects();
}

void Scene::setupObjects(vector<Object> objects, Model model)
{
	vector<glm::mat4> modelMatrices(objects.size());

	for (size_t i = 0; i < objects.size(); i++)
	{
		modelMatrices[i] = glm::translate(glm::mat4(1.0f), objects[i].position) * objects[i].model;
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
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
	Object terrain(&terrainModel, model, position);
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
				glm::mat4 model = glm::mat4(1.0f);
				glm::vec3 position = glm::vec3(0.0f, 1.0f, 0.0f);
				Object tree(&treeModel, model, position);
				treeObjects.push_back(tree);
			}
		}
	}
	setupObjects(treeObjects, treeModel);
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
	float sunX = WORLD_W * 5.0f * sine;
	float sunY = WORLD_W * 5.0f * cosine;
	float lightVal = max(0.1f, cosine);
	glm::vec3 lightPos(WORLD_W * sine, WORLD_W * cosine, 0.0f);

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

	// Draw objects
	terrainModel.draw(terrainObjects.size());
	treeModel.draw(treeObjects.size());
}