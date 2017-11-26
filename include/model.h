#ifndef MODEL_H
#define MODEL_H


#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "glad/glad.h" 
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "stb_image/stb_image.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "mesh.h"
#include "shader.h"


class Model 
{
public:
	std::vector<Texture> textures_loaded;
	std::vector<Mesh> meshes;
	std::string directory;
	Shader shader;
    bool gammaCorrection;

	// Constructor
	Model(std::string const &path, Shader s, bool gamma = false);

	// Draw a set number of objects
	void draw(unsigned int amount);

	// Draw the mesh with given arguments
	void drawMesh(Mesh mesh, unsigned int amount);
   
private:

	// Load model from file
	void loadModel(std::string const &path);

	// Process a node recursively
	void processNode(aiNode *node, const aiScene *scene);

	// Process mesh and organize into data structures
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	// Load textures for the model
	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

	// Load textures from file
	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
};

#endif