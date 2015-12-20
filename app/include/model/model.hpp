#pragma once
#include <vector>
#include <string>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.hpp"
#include "texture.hpp"
#include <glimac/Image.hpp>

class Model 
{
    public:
        Model(string path);
        void Draw(Shader shader);   
    private:
        vector<Mesh> meshes;
        vector<Texture> textures_loaded;
        string directory;

        void loadModel(string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        GLint TextureFromFile(const char* path, string directory);
        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
