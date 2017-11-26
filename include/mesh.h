#ifndef MESH_H
#define MESH_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#include "glad/glad.h"
#include "assimp/Importer.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "shader.h"

struct Vertex 
{
    // Position
    glm::vec3 Position;
    // Normal
    glm::vec3 Normal;
    // Texture Coordinates
    glm::vec2 TexCoords;
};

struct Texture 
{
    unsigned int id;
    std::string type;
    aiString path;
};

class Mesh 
{
public:

	// Mesh parameters
	unsigned int VAO;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	// Constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);

private:

	unsigned int VBO, EBO;

	// Setup function
	void setupMesh();
};
#endif