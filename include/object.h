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

	// Local space to world space transformation matrix
	glm::mat4 model;

	// Global position of the object
	glm::vec3 position;

	// Constructor
	Object(Model* baseModel, glm::mat4 model, glm::vec3 position);
};

#endif