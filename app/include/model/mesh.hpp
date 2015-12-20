#pragma once 

#include <vector>
#include <sstream>
#include <string>
#include "texture.hpp"
#include "geometry/vertex.hpp"
#include "shader.hpp"

using namespace std;

class Mesh {
    public:
        vector<Vertex> vertices;
        vector<GLuint> indices;
        vector<Texture> textures;
        
        Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
        void Draw(Shader shader);
    private:
        GLuint VAO, VBO, EBO;
        
        void setupMesh();
};  
