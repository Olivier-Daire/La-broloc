#pragma once
#include <glm/glm.hpp>
#include <string>

struct ModelInfos {
    string Path;
    glm::vec3 Translate;
    glm::vec3 Scale;

    ModelInfos(string path, glm::vec3 translate, glm::vec3 scale){
        Path = path;
        Translate = translate;
        Scale = scale;
    };
};
