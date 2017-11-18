#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <map>
#include "shader.h"
#include "entity.h"
#include "object.h"

class Renderer
{
public:

	// Screen dimensions
	const float SCR_W = 1366.0f;
	const float SCR_H = 768.0f;

	// World dimensions
	const float WORLD_W = 3000.0f;
	const float WORLD_H = 1000.0f;
	const float WORLD_D = 2000.0f;

	// Shader object
	Shader myShader;

	// The Vertex Array Object, Vertex Buffer Object and Element Buffer Object
	unsigned int VAO, VBO, EBO;

	// View matrix
	glm::mat4 view;

	// Projection matrix
	glm::mat4 projection;

	// Camera position
	glm::vec3 cameraPos;

	// Light source position
	glm::vec3 lightPos;

	// Light source color
	glm::vec3 lightColor;

	// This maps stationary entity type codes to a vector of Objects
	std::map<unsigned int, std::vector<Object>> staticTypeMap;

	// This maps moving entity type codes to a vector of Objects
	std::map<unsigned int, std::vector<Object>> dynamicTypeMap;

	// Polygon representing vectors to draw from
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// Constructor
	Renderer();

	// Destructor
	~Renderer();

	// Setter for view matrix
	void setViewMatrix(glm::mat4 view);

	// Setter for camera position
	void setCameraPos(glm::vec3 cameraPos);

	// Setter for light source position
	void setLightPos(glm::vec3 lightPos);

	// Setter for light source color
	void setLightColor(glm::vec3 lightColor);

	// Inserts objects into the static type map
	void Renderer::addStaticObject(Object& object);

	// Inserts objects into the dynamic type map
	void Renderer::addDynamicObject(Object& object);

	// Preparation function for a certain Mesh type
	void prepareMeshType(Entity* baseEntity, size_t meshIndex);

	// Rendering function for a specific mesh of an object
	void renderMeshOfInstance(Object& object, size_t meshIndex);

	// Unbinding function
	void unbindMeshType();

	// Rendering function
	void render();
};

#endif