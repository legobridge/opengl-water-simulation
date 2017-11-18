#ifndef OBJECT_H
#define OBJECT_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "entity.h"

class Object
{
public:
	
	// Pointer to the entity of which the object is an instance
	Entity* baseEntity;

	// Local space to World space transformation matrix
	glm::mat4 model;

	// Global position of the object
	glm::vec3 position;

	// Absolute velocity of the object
	glm::vec3 velocity;

	// Constructor
	Object(Entity* baseEntity, glm::mat4 model, glm::vec3 position, glm::vec3 velocity);
};

#endif