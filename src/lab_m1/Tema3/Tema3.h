#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include "GridCell.h"
#include "Wall_Tema3.h"
#include "Dancer.h"
#include "Reflector.h"
#include "lab_m1/Tema3/object2D_Tema3.h"

namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color = glm::vec3(1));

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        bool coordsInGrid(int coordOx, int coordOz);
        std::vector<glm::vec3> neighborsGridCell(int coordOx, int coordOz);
        Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);

        glm::vec3 lightPosition;
        glm::vec3 lightDirection;
        unsigned int materialShininess;
        float materialKd;
        float materialKs;

        // TODO(student): If you need any other class variables, define them here.
        int is_spotlight;
        float cutoff_angle;
        float angleOx;
        float angleOy;

        GridCell **danceFloor;
        std::vector<Wall_Tema3> walls;
        std::vector<Dancer> dancers;
        std::vector<Reflector> reflectors;
        glm::vec3 reflectorsPos[4];
        glm::vec3 reflectorsColor[4];
        glm::vec3 reflectorsDirection[4];
        glm::vec3 pointLightPos[9];
        glm::vec3 pointLightColor[9];

        int noLightSources;
        int noSpotlights;

        unsigned int Shininess;
        float Kd;
        float Ks;
        int gameState;
        float cutoffAngle;
        std::unordered_map<std::string, Texture2D*> mapTextures;
        Texture2D* textureToShader;

        int reflectorsOn;
        int discoGlobeOn;
        int lightsGridOn;
        int modeLights;
    };
}   // namespace m1
