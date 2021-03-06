#ifndef SHADER_H
#define SHADER_H

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
    unsigned int ID;

    // Constructor generates the shader on the fly
	Shader(const char* vertexPath, const char* fragmentPath);

    // Activate the shader
	void use();

	// Utility functions for setting uniforms
	void setBool(const std::string &name, bool value);
	void setInt(const std::string &name, int value);
	void setFloat(const std::string &name, float value);
	void setVec2(const std::string &name, const glm::vec2 &value);
	void setVec2(const std::string &name, float x, float y);
	void setVec3(const std::string &name, const glm::vec3 &value);
	void setVec3(const std::string &name, float x, float y, float z);
	void setVec4(const std::string &name, const glm::vec4 &value);
	void setVec4(const std::string &name, float x, float y, float z, float w);
	void setMat2(const std::string &name, const glm::mat2 &mat);
	void setMat3(const std::string &name, const glm::mat3 &mat);
	void setMat4(const std::string &name, const glm::mat4 &mat);
	
private:
    // Utility function for checking shader compilation/linking errors.
	void checkCompileErrors(unsigned int shader, std::string type);
};

#endif