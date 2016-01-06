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
#include <float.h>

class Model 
{
    public:
        Model();
        Model(string path);
        Model(string path, glm::vec3 translate, glm::vec3 scale, glm::vec3 rotate, float rotateAngle, int interaction);
        void Draw(Shader shader);

        void updatePosition();
        glm::vec3 getPosition();
        glm::vec3 getTranslate();
        glm::vec3 getScale();
        glm::vec3 getRotate();
        float getRotateAngle();
        int getInteractionDialogue();

    private:
        vector<Mesh> meshes;
        vector<Texture> textures_loaded;
        string directory;

        glm::vec3 position;
        glm::vec3 translate;
        glm::vec3 scale;
        glm::vec3 rotate;
        float rotateAngle;
        int interactionDialogue;

        void loadModel(string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        GLint TextureFromFile(const char* path, string directory);
        vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};
