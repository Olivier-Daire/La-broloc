#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "mesh.hpp"
#include "texture.hpp"
#include <glimac/Image.hpp>
#include "AABB.hpp"
#include <float.h>

class Model 
{
    public:
        Model();
        Model(string path);
        void Draw(Shader shader);
        glm::mat4 translate(glm::mat4 mat,float x,float y,float z);
        glm::mat4 scale(glm::mat4 mat,float x,float y,float z); 
        AABB box;

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
