#pragma once

#include <glm/glm.hpp>
#include <core/gpu/mesh.h>

#define OX_RIGHT 0
#define OX_LEFT 1
#define OY_RIGHT 2
#define OY_LEFT 3
#define OZ_RIGHT 4
#define OZ_LEFT 5
#define NO_MOVEMENTS 6
#define MAX_MOVE_TIME M_PI/2

class Reflector {
private:
    float radius;
    float height;
    glm::vec3 center;
    glm::vec3 lightDirection;
    glm::vec3 color;
    glm::vec3 coords;
    float cutoffAngle;
    Mesh* associatedMesh;
    float angleOx;
    float angleOy;
    float angleOz;
    int orientation;
    float elapsedMoveTime;
    void printAnglesAndDirection();
public:
    Reflector(float radius, float height,
        glm::vec3 center, glm::vec3 lightDirection,
        glm::vec3 color,
        float cutOffAngle, Mesh* assosiactedMesh);
    Mesh* getAssociatedMesh();
    glm::vec3 getColor();
    glm::vec3 getCoords();
    float getHeight();
    float getAngleOx();
    float getAngleOy();
    float getAngleOz();
    float getCutoffAngle();
    glm::vec3 getLightDirection();
    void generateMovement();
    void setCoords(glm::vec3 coords);
    void setColor(glm::vec3 color);
    void setAngleOx(float angleOx);
    void setAngleOy(float angleOy);
    void setAngleOz(float angleOz);
    void updateLightDirection();
    void move(float speed);
};