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

	Model(std::string const &path, Shader s, bool gamma = false);

	void draw();

	void drawMesh(Mesh mesh);
   
private:

	void loadModel(std::string const &path);

	void processNode(aiNode *node, const aiScene *scene);

	Mesh processMesh(aiMesh *mesh, const aiScene *scene);

	std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

	unsigned int TextureFromFile(const char *path, const std::string &directory, bool gamma = false);
};

#endif