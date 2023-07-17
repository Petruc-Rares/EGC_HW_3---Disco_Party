#include "GridCell.h"

GridCell::GridCell(int sizeSide, glm::vec3 corner, glm::vec3 color, Mesh* associatedMesh) {
    this->sizeSide = sizeSide;
    this->corner = corner;
    this->color = color;
    this->associatedMesh = associatedMesh;

    this->lightSourcePos = glm::vec3(this->corner.x + (float) this->sizeSide / 2, 0.0f, this->corner.z - (float) this->sizeSide / 2);
}

Mesh *GridCell::getAssociatedMesh() {
    return this->associatedMesh;
}

glm::vec3 GridCell::getColor() {
    return this->color;
}

glm::vec3 GridCell::getCorner() {
    return this->corner;
}

glm::vec3 GridCell::getLightSourcePos() {
    return this->lightSourcePos;
}