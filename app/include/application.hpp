#pragma once

#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "shader.hpp"
#include "model/model.hpp"
#include "tools/camera.hpp"
#include "tools/command.hpp"
#include "scene.hpp"
#include "tools/text.hpp"
#include <string>
#include <sstream>
#include <cmath>

using namespace glimac;

class Application 
{
    public:
        Application();
        static std::string launch(std::string currentScene);
};
