#include "Wall_Tema3.h"

Wall_Tema3::Wall_Tema3(glm::vec3 corner, glm::vec3 color,
    float width, float depth, float height,
    Mesh* associatedMesh) {
    this->corner = corner;
    this->color = color;
    this->width = width;
    this->depth = depth;
    this->height = height;
    this->associatedMesh = associatedMesh;
}

Mesh* Wall_Tema3::getAssociatedMesh() {
    return this->associatedMesh;
}

float Wall_Tema3::getWidth() {
    return this->width;
}

float Wall_Tema3::getDepth() {
    return this->depth;
}

float Wall_Tema3::getHeight() {
    return this->height;
}

glm::vec3 Wall_Tema3::getColor() {
    return this->color;
}
glm::vec3 Wall_Tema3::getCorner() {
    return this->corner;
}

int Wall_Tema3::getGridCellOx() {
    return this->gridCellOx;
}

int Wall_Tema3::getGridCellOz() {
    return this->gridCellOz;
}

void Wall_Tema3::setGridCellOx(int gridCellOx) {
    this->gridCellOx = gridCellOx;
}

void Wall_Tema3::setGridCellOz(int gridCellOz) {
    this->gridCellOz = gridCellOz;
}