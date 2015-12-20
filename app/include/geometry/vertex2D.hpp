typedef struct Vertex2D {

    glm::vec2 position;
    glm::vec3 texture;

    Vertex2D(glm::vec2 pos, glm::vec3 tex){
        position = pos;
        texture = tex;
    };

} Vertex2D;
