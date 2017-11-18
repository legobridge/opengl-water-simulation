#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "shape.h"

using namespace std;

namespace shape
{
	Mesh getCuboidalMesh(float x, float y, float z)
	{
		vector<Vertex> vertices;

		float vX = x / 2;
		float vY = y / 2;
		float vZ = z / 2;

		Vertex vertex;
		glm::vec3 v;

		for (int i = 0; i < 8; i++)
		{
			if (i & 4)
			{
				v.x = vX;
			}
			else
			{
				v.x = -vX;
			}
			if (i & 2)
			{
				v.y = vY;
			}
			else
			{
				v.y = -vY;
			}
			if (i & 1)
			{
				v.z = vZ;
			}
			else
			{
				v.z = -vZ;
			}
			vertex.Position = v;
			vertices.push_back(vertex);
		}

		v.x = -vX;
		v.y = 0.0f;
		v.z = 0.0f;
		vertex.Position = v;
		vertices.push_back(vertex);

		v.x = 0.0f;
		v.y = -vY;
		v.z = 0.0f;
		vertex.Position = v;
		vertices.push_back(vertex);

		v.x = vX;
		v.y = 0.0f;
		v.z = 0.0f;
		vertex.Position = v;
		vertices.push_back(vertex);

		v.x = 0.0f;
		v.y = vY;
		v.z = 0.0f;
		vertex.Position = v;
		vertices.push_back(vertex);

		v.x = 0.0f;
		v.y = 0.0f;
		v.z = -vZ;
		vertex.Position = v;
		vertices.push_back(vertex);

		v.x = 0.0f;
		v.y = 0.0f;
		v.z = vZ;
		vertex.Position = v;
		vertices.push_back(vertex);

		vector<unsigned int> indices = {
			8, 0, 1,
			8, 1, 3,
			8, 3, 2,
			8, 2, 0,

			9, 4, 5,
			9, 5, 1,
			9, 1, 0,
			9, 0, 4,

			10, 6, 7,
			10, 7, 5,
			10, 5, 4,
			10, 4, 6,

			11, 2, 3,
			11, 3, 7,
			11, 7, 6,
			11, 6, 2,

			12, 2, 6,
			12, 6, 4,
			12, 4, 0,
			12, 0, 2,

			13, 1, 5,
			13, 5, 7,
			13, 7, 3,
			13, 3, 1
		};

		for (size_t i = 0; i < indices.size(); i += 3)
		{
			glm::vec3 faceNormal = glm::normalize(glm::cross(vertices[indices[i + 1]].Position - vertices[indices[i]].Position, vertices[indices[i + 2]].Position - vertices[indices[i]].Position));
			vertices[indices[i]].Normal += faceNormal;
			vertices[indices[i + 1]].Normal += faceNormal;
			vertices[indices[i + 2]].Normal += faceNormal;
		}

		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertices[i].Normal = glm::normalize(vertices[i].Normal);
		}

		Mesh mesh(vertices, indices);
		return mesh;
	}

	Mesh getCylindricalMesh(float r, float h)
	{
		const float incr = 1.0f;
		const unsigned int points = (unsigned int)(360 / incr);
		vector<Vertex> vertices(1 + 1 + points + points);
		vector<unsigned int> indices;

		Vertex vertex;
		glm::vec3 v;

		v.x = 0.0f;
		v.y = h / 2;
		v.z = 0.0f;
		vertex.Position = v;
		vertices[0] = vertex;

		v.x = 0.0f;
		v.y = -h / 2;
		v.z = 0.0f;
		vertex.Position = v;
		vertices[1] = vertex;

		for (int i = 0; i < points; i++)
		{
			v.x = r * (float)sin(glm::radians(i * incr));
			v.y = h / 2;
			v.z = r * (float)cos(glm::radians(i * incr));
			vertex.Position = v;
			vertices[2 + i] = vertex;

			unsigned int first1 = 2 + i;
			unsigned int second1 = (2 + i + 1 < 2 + points) ? (2 + i + 1) : (2);
			indices.push_back(0);
			indices.push_back(first1);
			indices.push_back(second1);

			v.x = r * (float)sin(glm::radians(i * incr));
			v.y = -h / 2;
			v.z = r * (float)cos(glm::radians(i * incr));
			vertex.Position = v;
			vertices[2 + points + i] = vertex;

			unsigned int first2 = 2 + points + i;
			unsigned int second2 = (2 + points + i + 1 < 2 + points + points) ? (2 + points + i + 1) : (2 + points);
			indices.push_back(1);
			indices.push_back(first2);
			indices.push_back(second2);

			indices.push_back(first1);
			indices.push_back(first2);
			indices.push_back(second2);

			indices.push_back(first1);
			indices.push_back(second2);
			indices.push_back(second1);
		}

		for (size_t i = 0; i < indices.size(); i += 3)
		{
			glm::vec3 faceNormal = glm::normalize(glm::cross(vertices[indices[i + 1]].Position - vertices[indices[i]].Position, vertices[indices[i + 2]].Position - vertices[indices[i]].Position));
			vertices[indices[i]].Normal += faceNormal;
			vertices[indices[i + 1]].Normal += faceNormal;
			vertices[indices[i + 2]].Normal += faceNormal;
		}

		for (size_t i = 0; i < vertices.size(); i++)
		{
			vertices[i].Normal = glm::normalize(vertices[i].Normal);
		}

		Mesh mesh(vertices, indices);
		return mesh;
	}
}