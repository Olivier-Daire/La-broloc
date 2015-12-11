#pragma once 

#include <vector>
#include <sstream>
#include <string>
#include "texture.hpp"
#include "vertex.hpp"

using namespace std;

class Mesh {
    public:
        /*  Mesh Data  */
        vector<Vertex> vertices;
        vector<GLuint> indices;
        vector<Texture> textures;
        /*  Functions  */
        Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
    private:
        /*  Render data  */
        GLuint VAO, VBO, EBO;
        /*  Functions    */
        void setupMesh();
};  
