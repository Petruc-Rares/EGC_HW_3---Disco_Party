#include "Reflector.h"

Reflector::Reflector(float radius, float height,
    glm::vec3 center, glm::vec3 lightDirection,
    glm::vec3 color,
    float cutoffAngle, Mesh* associatedMesh) {

    this->radius = radius;
    this->height = height;
    this->center = center;
    this->lightDirection = lightDirection;
    this->color = color;
    this->coords = center + glm::vec3(0, height, 0);
    this->cutoffAngle = cutoffAngle;
    this->associatedMesh = associatedMesh;
    this->angleOx = 0;
    this->angleOy = 0;
    this->angleOz = 0;
    this->elapsedMoveTime = 0;
    generateMovement();
}

glm::vec3 Reflector::getLightDirection() {
    return this->lightDirection;
}

float Reflector::getAngleOx() {
    return this->angleOx;
}

float Reflector::getAngleOy() {
    return this->angleOy;
}

float Reflector::getAngleOz() {
    return this->angleOz;
}

float Reflector::getCutoffAngle() {
    return this->cutoffAngle;
}

Mesh* Reflector::getAssociatedMesh() {
    return this->associatedMesh;
}

float Reflector::getHeight() {
    return this->height;
}

glm::vec3 Reflector::getCoords() {
    return this->coords;
}

glm::vec3 Reflector::getColor() {
    return this->color;
}

void Reflector::setCoords(glm::vec3 coords) {
    this->coords = coords;
}

void Reflector::setColor(glm::vec3 color) {
    this->color = color;
}

void Reflector::setAngleOx(float angleOx) {
    this->angleOx = angleOx;
}


void Reflector::setAngleOy(float angleOy) {
    this->angleOy = angleOy;
}

void Reflector::setAngleOz(float angleOz) {
    this->angleOz = angleOz;
}

void Reflector::updateLightDirection() {
    glm::mat4 matrix = glm::mat4(1);
    matrix = glm::rotate(matrix, angleOx, glm::vec3(1, 0, 0));
    matrix = glm::rotate(matrix, angleOy, glm::vec3(0, 1, 0));
    matrix = glm::rotate(matrix, angleOz, glm::vec3(0, 0, 1));
    
    glm::vec3 localLightDirection = glm::vec3(0, -1, 0);
    lightDirection = glm::vec3(matrix * glm::vec4(localLightDirection, 0));

   // printAnglesAndDirection();
}

void Reflector::move(float deltaTime) {
    float speed = deltaTime * 0.2;
    this->elapsedMoveTime += deltaTime;

    if (this->elapsedMoveTime >= MAX_MOVE_TIME) {
        this->elapsedMoveTime = 0;
        generateMovement();
    }

    switch (orientation) {
        case OX_RIGHT:
            angleOx = (angleOx + speed > M_PI_4) ? angleOx : angleOx + speed;
            break;
        case OX_LEFT:
            angleOx = (angleOx - speed < -M_PI_4) ? angleOx : angleOx - speed;
            break;
        case OY_RIGHT:
            angleOy += speed;
            break;
        case OY_LEFT:
            angleOy -= speed;
            break;
        case OZ_RIGHT:
            angleOz = (angleOz + speed > M_PI_4) ? angleOz : angleOz + speed;
            break;
        case OZ_LEFT:
            angleOz = (angleOz - speed < -M_PI_4) ? angleOz : angleOz - speed;
            break;
        default:
            break;
    }
    updateLightDirection();
}

void Reflector::generateMovement() {
    this->orientation = rand() % NO_MOVEMENTS;
   // printAnglesAndDirection();
    return;
}

void Reflector::printAnglesAndDirection() {
    printf("angleOx: %f, angleOy: %f, angleOz: %f\n", angleOx, angleOy, angleOz);
    if (orientation == OX_LEFT) {
        printf("OX_LEFT");
    } else if (orientation == OX_RIGHT) {
        printf("OX_RIGHT");
    } else if (orientation == OY_LEFT) {
        printf("OY_LEFT");
    } else if (orientation == OY_RIGHT) {
        printf("OY_RIGHT");
    } else if (orientation == OZ_LEFT) {
        printf("OZ_LEFT");
    } else if (orientation == OZ_RIGHT) {
        printf("OZ_RIGHT");
    }
    printf("\n");
}