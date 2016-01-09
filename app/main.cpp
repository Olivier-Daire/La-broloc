#include <glimac/SDLWindowManager.hpp>
#include <string>
#include <iostream>
#include "application.hpp"

using namespace glimac;

int main(int argc, char** argv) {



    std::string scene = "Home";

    while(scene != "fin") {
        scene = Application::launch(scene);
    }

    return EXIT_SUCCESS;
}
