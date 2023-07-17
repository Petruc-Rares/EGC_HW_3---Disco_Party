#include "object2D_Tema3.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D_Tema3::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    //printf("corner[0]: %f, corner[1]: %f, corner[2]: %f, length: %f\n", corner[0], corner[1], corner[2], length);

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, 0, -length), color),
        VertexFormat(corner + glm::vec3(0, 0, -length), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object3D_Tema3::CreateParallelepiped(
    const std::string& name, glm::vec3 leftBottomCorner,
    float width, float height, float depth,
    glm::vec3 color)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(width, 0, 0), color),
        VertexFormat(corner + glm::vec3(width, height, 0), color),
        VertexFormat(corner + glm::vec3(0, height, 0), color),
        VertexFormat(corner + glm::vec3(0, 0, depth), color),
        VertexFormat(corner + glm::vec3(width, 0, depth), color),
        VertexFormat(corner + glm::vec3(width, height, depth), color),
        VertexFormat(corner + glm::vec3(0, height, depth), color),
    };

    Mesh* parallelepiped = new Mesh(name);
    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3,
        1, 5, 6,
        1, 6, 2,
        3, 2, 6,
        3, 6, 7,
        0, 5, 1,
        0, 4, 5,
        4, 6, 5,
        4, 7, 6,
        0, 7, 4,
        0, 3, 7 };

    parallelepiped->SetDrawMode(GL_TRIANGLES);

    parallelepiped->InitFromData(vertices, indices);
    return parallelepiped;
}

Mesh* object3D_Tema3::CreateCone(const std::string& name, glm::vec3 center,
    float height, float radius,
    glm::vec3 color) {

    std::vector<VertexFormat> vertices = {center + glm::vec3(0, height, 0)};
    std::vector<unsigned int> indices = {0};


    for (float i = 0; i < 360; i++) {
        vertices.push_back(VertexFormat(center + glm::vec3(radius * cos(glm::radians(i)), 0, radius * sin(glm::radians(i))), color));
        indices.push_back(i + 2);
        indices.push_back(i + 1);
    }

    indices.pop_back();
    indices.pop_back();
    indices.push_back(1);
    indices.push_back(360);

    Mesh* cone = new Mesh(name);


    cone->SetDrawMode(GL_TRIANGLE_FAN);

    cone->InitFromData(vertices, indices);
    return cone;
}