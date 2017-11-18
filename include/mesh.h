#ifndef MESH_H
#define MESH_H

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
};

class Mesh
{
public:

	// Mesh Data
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	// Default constructor
	Mesh();

	// Constructor
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
};
#endif