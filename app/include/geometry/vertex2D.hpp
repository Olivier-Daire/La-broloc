typedef struct Vertex2D {

    glm::vec2 position;
    glm::vec2 texture;

    Vertex2D(glm::vec2 pos, glm::vec2 tex){
        position = pos;
        texture = tex;
    };

} Vertex2D;
