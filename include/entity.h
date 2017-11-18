#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "shader.h"
#include "mesh.h"

class Entity
{
public:
	// Vector of meshes
	std::vector<Mesh> meshes;

	// Type identifier of the entity
	// 0 = 
	// 1 = ...
	unsigned int entityType;

	// Type names of corresponding meshes
	std::vector<glm::vec3> meshColors;

	// Value assigning constructor
	Entity(std::vector<Mesh> meshes, unsigned int entityType, std::vector<glm::vec3> meshColors);

	// Model loading constructor
	Entity(char *path, unsigned int entityType);

private:
	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
};
#endif