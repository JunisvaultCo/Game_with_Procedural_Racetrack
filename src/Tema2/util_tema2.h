#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <iostream>
#define GRAY glm::vec3(0.5, 0.5, 0.5)
#define CYAN glm::vec3(0, 1, 1)
#define GREEN glm::vec3(0, 1, 0)
#define PURPLE glm::vec3(1, 0, 1)
#define WHITE glm::vec3(1, 1, 1)
#define BLACK glm::vec3(0, 0, 0)
#define RED glm::vec3(1, 0, 0)
#define YELLOW glm::vec3(1, 1, 0)
#define BLUE glm::vec3(0, 0, 1)
#define BROWN glm::vec3(0.3, 0.1, 0)
#define DARK_GREEN glm::vec3(0, 0.5, 0)
#define SKY_COLOR 0, 0.6, 1, 1

using namespace std;

namespace utiltema2 {
    Mesh *CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);
}