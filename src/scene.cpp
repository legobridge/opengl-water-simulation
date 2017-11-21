#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "scene.h"

using namespace std;

Scene::Scene()
	: modelShader("../src/shader/model.vs", "../src/shader/model.fs")
	, grassModel("../model/grass/Grass_02.obj", modelShader)
	, treeModel("../model/tree/Tree.obj", modelShader)
{
	time = 0.0f;

	timescale = 16.0f;

	paused = true;

	colors["red"] = glm::vec3(0.778f, 0.040f, 0.040f);
	colors["blue"] = glm::vec3(0.000f, 0.000f, 0.700f);
	colors["green"] = glm::vec3(0.065f, 0.350f, 0.065f);
	colors["yellow"] = glm::vec3(0.976f, 0.890f, 0.086f);
	colors["orange"] = glm::vec3(0.837f, 0.279f, 0.010f);
	colors["black"] = glm::vec3(0.010f, 0.010f, 0.010f);
	colors["white"] = glm::vec3(0.900f, 0.900f, 0.800f);
	colors["wood"] = glm::vec3(0.400f, 0.200f, 0.000f);
	colors["metal"] = glm::vec3(0.270f, 0.290f, 0.318f);
	colors["dirt"] = glm::vec3(0.308f, 0.100f, 0.053f);
	colors["grass"] = glm::vec3(0.100f, 0.600f, 0.100f);
	colors["rope"] = glm::vec3(0.398f, 0.337f, 0.220f);

	setupGrassObjects();
	setupTreeObjects();
}


void Scene::setupGrassObjects()
{
	for (int i = -4; i < 5; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			for (int k = -4; k < 5; k++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				glm::vec3 position = glm::vec3(5.0f * i, 0.0f, 5.0f * k);
				Object grass(&grassModel, model, position);
				grassObjects.push_back(grass);
			}
		}
	}

	vector<glm::mat4> modelMatrices(grassObjects.size());

	for (size_t i = 0; i < grassObjects.size(); i++)
	{
		modelMatrices[i] = glm::translate(glm::mat4(1.0f), grassObjects[i].position) * grassObjects[i].model;
	}

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, grassObjects.size() * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (size_t i = 0; i < grassModel.meshes.size(); i++)
	{
		unsigned int VAO = grassModel.meshes[i].VAO;
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

void Scene::setupTreeObjects()
{
	for (int i = -1; i < 2; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			for (int k = -1; k < 2; k++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
				glm::vec3 position = glm::vec3(20.0f * i, 0.0f, 20.0f * k);
				Object tree(&treeModel, model, position);
				treeObjects.push_back(tree);
			}
		}
	}

	vector<glm::mat4> modelMatrices(treeObjects.size());

	for (size_t i = 0; i < treeObjects.size(); i++)
	{
		modelMatrices[i] = glm::translate(glm::mat4(1.0f), treeObjects[i].position) * treeObjects[i].model;
	}

	unsigned int buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, treeObjects.size() * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

	for (size_t i = 0; i < treeModel.meshes.size(); i++)
	{
		unsigned int VAO = treeModel.meshes[i].VAO;
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

// Driver function to perform routine tasks
void Scene::drawObjects()
{
	float sunX = WORLD_W * 5.0f * sin(glm::radians(time));
	float sunY = WORLD_W * 5.0f * cos(glm::radians(time));

	float lightVal = max(0.1f, cos(glm::radians(time)));
	

	// ******** Rendering Commands ********
	// Clear buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	modelShader.use();

	glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)SCR_W / (float)SCR_H, 0.1f, 1000.0f);
	glm::mat4 view = camera.GetViewMatrix();
	modelShader.setMat4("projection", projection);
	modelShader.setMat4("view", view);

	// Draw objects
	grassModel.draw(grassObjects.size());
	treeModel.draw(treeObjects.size());

	// ****** End Rendering Commands ******
}