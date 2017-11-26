#ifndef OBJECT_H
#define OBJECT_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "model.h"

class Object
{
public:
	
	// Pointer to the entity of which the object is an instance
	Model* baseModel;

	// Global position of the object
	glm::vec3 position;

	// Scaling parameter of the object
	glm::vec3 scale;

	// Constructor
	Object(Model* baseModel, glm::vec3 position, glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
};

#endif