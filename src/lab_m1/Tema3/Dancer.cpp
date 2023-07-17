#include "Dancer.h"

Dancer::Dancer(glm::vec3 coords,
                int sizeSquareDisco, int sizeDisco,
                float width, float depth, float height,
                glm::vec3 color, Mesh* associatedMesh) {
    this->coords = coords;
    this->sizeSquareDisco = sizeSquareDisco;
    this->sizeDisco = sizeDisco;
    this->width = width;
    this->depth = depth;
    this->height = height;
    this->color = color;
    this->associatedMesh = associatedMesh;
    this->timeElapsedDance = 0;
    radiusDance = sizeSquareDisco;


    this->coordsMatrix.y = 0;
    computeMatrCoord();
    generateOrientation();
}

void Dancer::computeMatrCoord() {
    this->coordsMatrix.x = ((this->coords.x > 0) ? sizeDisco / 2 + (int) (this->coords.x / sizeSquareDisco) 
                                                 : sizeDisco / 2 - 1 + (int) (this->coords.x / sizeSquareDisco));

    this->coordsMatrix.z = ((this->coords.z > 0) ? sizeDisco / 2 + (int) (this->coords.z / sizeSquareDisco)
                                                 : sizeDisco / 2 - 1 + (int) (this->coords.z / sizeSquareDisco));
}

Mesh* Dancer::getAssociatedMesh() {
    return this->associatedMesh;
}

glm::vec3 Dancer::getColor() {
    return this->color;
}

glm::vec3 Dancer::getCoordsMatrix() {
    return this->coordsMatrix;
}

glm::vec3 Dancer::getCoords() {
    return this->coords;
}

float Dancer::getTimeElapsedDance() {
    return this->timeElapsedDance;
}

float Dancer::getWidth() {
    return this->width;
}

float Dancer::getHeight() {
    return this->height;
}

float Dancer::getDepth() {
    return this->depth;
}


void Dancer::setTimeElapsedDance(float timeElapsedDanc) {
    this->timeElapsedDance = timeElapsedDanc;
}

void Dancer::dance() {
    if (timeElapsedDance >= MAX_TIME_DANCE) {
        // TODO generate radius distance as well
        generateOrientation();
        //printOrientation();
        timeElapsedDance = 0;
    }

    switch (orientation) {
        case FORWARD_LEFT:
            coords.x = initialOxDance + radiusDance * cos(timeElapsedDance);
            coords.z = initialOzDance + radiusDance * sin(timeElapsedDance);
            break;
        case FORWARD_RIGHT:
            coords.x = initialOxDance - radiusDance * cos(timeElapsedDance);
            coords.z = initialOzDance + radiusDance * sin(timeElapsedDance);
            break;
        case BACKWARD_LEFT:
            coords.x = initialOxDance + radiusDance * cos(timeElapsedDance);
            coords.z = initialOzDance - radiusDance * sin(timeElapsedDance);
            break;
        case BACKWARD_RIGHT:
            coords.x = initialOxDance - radiusDance * cos(timeElapsedDance);
            coords.z = initialOzDance - radiusDance * sin(timeElapsedDance);
            break;
        default:
            break;
    }
    // update corresponding matr coords
    computeMatrCoord();
}

void Dancer::generateOrientation() {
   // printOrientation();

    if ((coordsMatrix.x == 0) && (coordsMatrix.z == 0)) {
        orientation = FORWARD_RIGHT;
        initialOxDance = coords.x + radiusDance;
        initialOzDance = coords.z;
    }
    else if ((coordsMatrix.x == 0) && (coordsMatrix.z == sizeDisco - 1)) {
        orientation = BACKWARD_RIGHT;
        initialOxDance = coords.x + radiusDance;
        initialOzDance = coords.z;
    }
    else if ((coordsMatrix.x == sizeDisco - 1) && (coordsMatrix.z == sizeDisco - 1)) {
        orientation = BACKWARD_LEFT;
        initialOxDance = coords.x - radiusDance;
        initialOzDance = coords.z;
    }
    else if ((coordsMatrix.x == sizeDisco - 1) && (coordsMatrix.z == 0)) {
        orientation = FORWARD_LEFT;
        initialOxDance = coords.x - radiusDance;
        initialOzDance = coords.z;
    }
    else if (coordsMatrix.x <= 0) {
        // it is dangerous to go left, as you may leave the disco space
        int rand_no = rand() % 2;

        if (rand_no == FORWARD) {
            orientation = FORWARD_RIGHT;
            initialOxDance = coords.x + radiusDance;
            initialOzDance = coords.z;
        }
        else if (rand_no == BACKWARD) {
            orientation = BACKWARD_RIGHT;
            initialOxDance = coords.x + radiusDance;
            initialOzDance = coords.z;
        }
    }
    else if (coordsMatrix.x >= sizeDisco - 1) {
        // it is dangerous to go right, as you may leave the disco space
        int rand_no = rand() % 2;

        if (rand_no == FORWARD) {
            orientation = FORWARD_LEFT;
            initialOxDance = coords.x - radiusDance;
            initialOzDance = coords.z;
        }
        else if (rand_no == BACKWARD) {
            orientation = BACKWARD_LEFT;
            initialOxDance = coords.x - radiusDance;
            initialOzDance = coords.z;
        }
    }
    else if (coordsMatrix.z <= 0) {
        // it is dangerous to go behind, as you may leave the disco space
        int rand_no = BACKWARD_LEFT - FORWARD_LEFT + rand() % 2;

        if (rand_no == LEFT) {
            orientation = FORWARD_LEFT;
            initialOxDance = coords.x - radiusDance;
            initialOzDance = coords.z;
        }
        else if (rand_no == RIGHT) {
            orientation = FORWARD_RIGHT;
            initialOxDance = coords.x + radiusDance;
            initialOzDance = coords.z;
        }
    }
    else if (coordsMatrix.z >= sizeDisco - 1) {
        // it is dangerous to go forward, as you may leave the disco space
        int rand_no = BACKWARD_LEFT - FORWARD_LEFT + rand() % 2;

        if (rand_no == LEFT) {
            orientation = BACKWARD_LEFT;
            initialOxDance = coords.x - radiusDance;
            initialOzDance = coords.z;
        }
        else if (rand_no == RIGHT) {
            orientation = BACKWARD_RIGHT;
            initialOxDance = coords.x + radiusDance;
            initialOzDance = coords.z;
        }
    }
    else {
        // dancer can go anywhere;
        orientation = rand() % 4;
        if (orientation == BACKWARD_LEFT) {
            initialOxDance = coords.x - radiusDance;
            initialOzDance = coords.z;
        }
        else if (orientation == BACKWARD_RIGHT) {
            initialOxDance = coords.x + radiusDance;
            initialOzDance = coords.z;
        }
        else if (orientation == FORWARD_RIGHT) {
            initialOxDance = coords.x + radiusDance;
            initialOzDance = coords.z;
        }
        else if (orientation == FORWARD_LEFT) {
            initialOxDance = coords.x - radiusDance;
            initialOzDance = coords.z;
        }

    }
}

void Dancer::printOrientation() {
    if (orientation == FORWARD_LEFT) {
        printf("FORWARD LEFT\n");
    } else if (orientation == FORWARD_RIGHT) {
        printf("FORWARD RIGHT\n");
    } else if (orientation == BACKWARD_RIGHT) {
        printf("BACKWARD RIGHT\n");
    } else if (orientation == BACKWARD_LEFT) {
        printf("BACKWARD LEFT\n");
    }
    else {
        printf("\n\n\n\nUNKNOWN ORIENTATION\n\n\n\n\n");
    }
}