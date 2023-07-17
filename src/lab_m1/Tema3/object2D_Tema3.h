#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace object2D_Tema3
{

    // Create square with given bottom left corner, length and color
    Mesh* CreateSquare(const std::string &name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);
}

namespace object3D_Tema3
{
    Mesh* CreateParallelepiped(const std::string& name, glm::vec3 leftBottomCorner,
        float width, float height, float depth,
        glm::vec3 color);
    Mesh* CreateCone(const std::string& name, glm::vec3 center,
        float height, float radius,
        glm::vec3 color);
}