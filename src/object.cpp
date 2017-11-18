#include "object.h"

using namespace std;

// Constructor
Object::Object(Entity* baseEntity, glm::mat4 model, glm::vec3 position, glm::vec3 velocity)
{
	this->baseEntity = baseEntity;
	this->model = model;
	this->position = position;
	this->velocity = velocity;
}