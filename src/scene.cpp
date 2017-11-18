#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "scene.h"
#include "shape.h"

using namespace std;

// Constructor definition
// : blenderEntity("../model/doggo/doggo.obj", 0)
Scene::Scene()
{
	time = 0.0f;

	timescale = 16.0f;

	paused = true;

	cameraSpeed = 15.0f;

	pitch = 0.0f;
	yaw = -90.0f;
	roll = 0.0f;
	
	cameraPos = glm::vec3(0.0f, 160.0f, 1200.0f);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	renderer.setViewMatrix(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));

	colors["red"] = glm::vec3(0.778f, 0.040f, 0.040f);
	colors["blue"] = glm::vec3(0.000f, 0.000f, 0.700f);
	colors["green"] = glm::vec3(0.065f, 0.350f, 0.065f);
	colors["yellow"] = glm::vec3(0.976f, 0.890f, 0.086f);
	colors["orange"] = glm::vec3(0.837f, 0.279f, 0.010f);
	colors["black"] = glm::vec3(0.010f, 0.010f, 0.010f);
	colors["white"] = glm::vec3(0.900f, 0.900f, 0.800f);
	colors["wood"] = glm::vec3(0.400f, 0.200f, 0.000f);
	colors["metal"] = glm::vec3(0.270f, 0.290f, 0.318f);
	colors["dirt"] = glm::vec3(0.308f, 0.100f, 0.053f);
	colors["grass"] = glm::vec3(0.100f, 0.600f, 0.100f);
	colors["rope"] = glm::vec3(0.398f, 0.337f, 0.220f);

	vector<Mesh> meshes;
	meshes.push_back(shape::getCuboidalMesh(100.0f, 100.0f, 100.0f));
	vector<glm::vec3> meshColors;
	meshColors.push_back(colors["red"]);
	redCube = new Entity(meshes, 0, meshColors);
	glm::mat4 model = glm::mat4(1.0f);
	for (int i = 0; i < 100; i++)
	{
		model = glm::translate(model, glm::vec3(0.0f, 100.0f, 0.0f));
		Object redCube1(redCube, model, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		renderer.addStaticObject(redCube1);
	}
}

// Toggle time (on/off)
void Scene::toggleTime()
{
	paused = !paused;
}

// Slow down time
void Scene::slowDownTime()
{
	timescale = max(0.0f, timescale - 2.0f);
}

// Speed up time
void Scene::speedUpTime()
{
	timescale = min(100.0f, timescale + 2.0f);
}

// Change camera movement speed
void Scene::changeSpeed(float by)
{
	cameraSpeed = max(cameraSpeed + by, 0.0f);
}

// Move into the scene
void Scene::moveIn()
{
	cameraPos += cameraSpeed * cameraFront;
}

// Move out of the scene
void Scene::moveOut()
{
	cameraPos -= cameraSpeed * cameraFront;
}

// Strafe to the left
void Scene::strafeLeft()
{
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// Strafe to the right
void Scene::strafeRight()
{
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

// Pan the camera
void Scene::pan(float xoffset, float yoffset)
{
	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;
	if (yaw >= 360.0f)
		yaw -= 360.0f;
	if (yaw <= -360.0f)
		yaw += 360.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);
	cameraFront = front;
}


// Draw blender object
//void Scene::drawBlenderObject()
//{
//	vector<glm::vec3> meshColors(blenderEntity.meshes.size(), glm::vec3(0.0f, 0.0f, 0.0f));
//	meshColors[0] = colors["green"];
//	meshColors[1] = colors["metal"];
//	meshColors[2] = colors["blue"];
//	meshColors[3] = colors["yellow"];
//	meshColors[4] = colors["red"];
//	for (unsigned int i = 0; i < blenderEntity.meshes.size(); i++)
//	{
//		glm::mat4 model = glm::mat4(1.0f);
//		model = glm::translate(model, glm::vec3(-100.0f, 0.0f, -250.0f));
//		model = glm::scale(model, glm::vec3(45.0f, 45.0f, 45.0f));
//		// add to list
//	}
//}

// Driver function to perform routine tasks
void Scene::drawObjects()
{
	renderer.setViewMatrix(glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));

	renderer.setCameraPos(cameraPos);
	
	float sunX = WORLD_W * 5.0f * sin(glm::radians(time));
	float sunY = WORLD_W * 5.0f * cos(glm::radians(time));
	renderer.setLightPos(glm::vec3(sunX, sunY, 0.0f));

	float lightVal = max(0.1f, cos(glm::radians(time)));
	renderer.setLightColor(glm::vec3(lightVal, lightVal, lightVal));

	// drawBlenderObject();
	renderer.render();
}