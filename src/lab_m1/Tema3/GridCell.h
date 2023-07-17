#pragma once

#include <glm/glm.hpp>
#include <core/gpu/mesh.h>

class GridCell {
private:
    int sizeSide;
    // corner down left of each cell
    glm::vec3 corner;
    glm::vec3 lightSourcePos;
    glm::vec3 color;
    Mesh* associatedMesh;
public:
    GridCell(int sizeSide, glm::vec3 corner, glm::vec3 color, Mesh* associatedMesh);
    Mesh* getAssociatedMesh();
    glm::vec3 getColor();
    glm::vec3 getCorner();
    glm::vec3 getLightSourcePos();
};