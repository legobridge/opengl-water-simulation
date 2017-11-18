#include "glad/glad.h"
#include "renderer.h"

using namespace std;

Renderer::Renderer()
	: myShader("../src/shader/vertex.shader", "../src/shader/fragment.shader")
{
	// Use the compiled shader program
	myShader.use();

	// Generate and bind vertex array object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	projection = glm::perspective(glm::radians(45.0f), SCR_W / SCR_H, 0.1f, 4 * WORLD_D);
}

// Destructor
Renderer::~Renderer()
{
	// Deallocation of Resources
	glDeleteVertexArrays(1, &VAO);
}

// Setter for view matrix
void Renderer::setViewMatrix(glm::mat4 view)
{
	this->view = view;
}

// Setter for camera position
void Renderer::setCameraPos(glm::vec3 cameraPos)
{
	this->cameraPos = cameraPos;
}

// Setter for light source position
void Renderer::setLightPos(glm::vec3 lightPos)
{
	this->lightPos = lightPos;
}

// Setter for light source color
void Renderer::setLightColor(glm::vec3 lightColor)
{
	this->lightColor = lightColor;
}

// Inserts objects into the static type map
void Renderer::addStaticObject(Object& object)
{
	unsigned int entityType = object.baseEntity->entityType;
	staticTypeMap[entityType].push_back(object);
}

// Inserts objects into the dynamic type map
void Renderer::addDynamicObject(Object& object)
{
	unsigned int entityType = object.baseEntity->entityType;
	dynamicTypeMap[entityType].push_back(object);
}

// Preparation function for a certain Mesh type
void Renderer::prepareMeshType(Entity* baseEntity, size_t meshIndex)
{
	vertices = (baseEntity->meshes[meshIndex]).vertices;
	indices = (baseEntity->meshes[meshIndex]).indices;

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	unsigned int objectColorLoc = glGetUniformLocation(myShader.ID, "objectColor");

	glm::vec3 colorVector = baseEntity->meshColors[meshIndex];
	glUniform3f(objectColorLoc, colorVector.x, colorVector.y, colorVector.z);
}

void Renderer::renderMeshOfInstance(Object& object, size_t meshIndex)
{
	unsigned int modelLoc = glGetUniformLocation(myShader.ID, "model");
	unsigned int viewLoc = glGetUniformLocation(myShader.ID, "view");
	unsigned int projectionLoc = glGetUniformLocation(myShader.ID, "projection");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(object.model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	unsigned int lightPosLoc = glGetUniformLocation(myShader.ID, "lightPos");
	unsigned int viewPosLoc = glGetUniformLocation(myShader.ID, "viewPos");
	unsigned int lightColorLoc = glGetUniformLocation(myShader.ID, "lightColor");

	glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
	glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);

	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::unbindMeshType()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Renderer::render()
{
	for (auto it = staticTypeMap.begin(); it != staticTypeMap.end(); it++)
	{
		Entity* baseEntity = (*it).second[0].baseEntity;
		for (size_t i = 0; i < baseEntity->meshes.size(); i++)
		{
			prepareMeshType(baseEntity, i);
			for (size_t j = 0; j < (*it).second.size(); j++)
			{
				renderMeshOfInstance((*it).second[j], i);
			}
			unbindMeshType();
		}
	}
	for (auto it = dynamicTypeMap.begin(); it != dynamicTypeMap.end(); it++)
	{
		Entity* baseEntity = (*it).second[0].baseEntity;
		for (size_t i = 0; i < baseEntity->meshes.size(); i++)
		{
			prepareMeshType(baseEntity, i);
			for (size_t j = 0; j < (*it).second.size(); j++)
			{
				renderMeshOfInstance((*it).second[j], i);
			}
			unbindMeshType();
		}
	}
	dynamicTypeMap.clear();
}