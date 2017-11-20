#include "object.h"

using namespace std;

// Constructor
Object::Object(Model* baseModel, glm::mat4 model, glm::vec3 position)
{
	this->baseModel = baseModel;
	this->model = model;
	this->position = position;
}