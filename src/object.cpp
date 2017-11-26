#include "object.h"

using namespace std;

// Constructor
Object::Object(Model* baseModel, glm::vec3 position, glm::vec3 scale)
{
	this->baseModel = baseModel;
	this->position = position;
	this->scale = scale;
}