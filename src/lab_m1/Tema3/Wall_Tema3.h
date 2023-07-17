#pragma once

#include <glm/glm.hpp>
#include <core/gpu/mesh.h>

class Wall_Tema3 {
private:
    int sizeSide;
    glm::vec3 corner;
    glm::vec3 color;
    float width;
    float depth;
    float height;
    int gridCellOx;
    int gridCellOz;
    Mesh* associatedMesh;
public:
    Wall_Tema3(glm::vec3 corner, glm::vec3 color,
          float width, float depth, float height,  
          Mesh* associatedMesh);
    Mesh* getAssociatedMesh();
    glm::vec3 getColor();
    glm::vec3 getCorner();
    int getGridCellOx();
    int getGridCellOz();
    float getWidth();
    float getDepth();
    float getHeight();

    void setGridCellOx(int gridCellOx);
    void setGridCellOz(int gridCellOz);
};