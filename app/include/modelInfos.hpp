#pragma once
#include <glm/glm.hpp>
#include <string>

struct ModelInfos {
    string Path;
    glm::vec3 Translate;
    glm::vec3 Scale;
    glm::vec3 Rotate;
    float RotateAngle;

    ModelInfos(string path, glm::vec3 translate, glm::vec3 scale, glm::vec3 rotate, float rotateAngle){
        Path = path;
        Translate = translate;
        Scale = scale;
        Rotate = rotate;
        RotateAngle = rotateAngle;
    };
};
