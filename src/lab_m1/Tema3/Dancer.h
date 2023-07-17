#pragma once

#include <glm/glm.hpp>
#include <core/gpu/mesh.h>
#include <stdlib.h>
#include <cmath>

#define FORWARD_LEFT 0
#define FORWARD_RIGHT 1
#define BACKWARD_LEFT 2
#define BACKWARD_RIGHT 3

#define FORWARD 0
#define BACKWARD 1
#define LEFT 2
#define RIGHT 3
#define MAX_TIME_DANCE 1.5

class Dancer {
private:
    // coords of center of dancer in xOz plane ( y = 0 )
    glm::vec3 coords;
    // coords equivalent in the dance floor
    glm::vec3 coordsMatrix;
    glm::vec3 color;
    float width;
    float depth;
    float height;
    int sizeSquareDisco;
    int sizeDisco;
    Mesh* associatedMesh;
    int orientation;
    float timeElapsedDance;
    float radiusDance;

    float initialOxDance;
    float initialOzDance;

    void computeMatrCoord();
public:
    Dancer(glm::vec3 coords, int sizeSquareDisco, int sizeDisco,
        float width, float depth, float height,
        glm::vec3 color, Mesh* assosiactedMesh);
    Mesh* getAssociatedMesh();
    glm::vec3 getCoords();
    glm::vec3 getColor();
    glm::vec3 getCoordsMatrix();
    float getWidth();
    float getDepth();
    float getHeight();
    float getTimeElapsedDance();
    void setTimeElapsedDance(float timeElapsedDance);

    void generateOrientation();
    void dance();
    void printOrientation();
};