#include "lab_m1/Tema3/Tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

#define sideDisco 8
#define sideSquareDisco 1
#define HEIGHT_ROOF sideSquareDisco * 3
#define NO_DANCERS 5
#define NO_REFLECTORS 4
#define GAME_DEVELOPMENT 0
#define GAME_IN_PROGRESS 1

#define ALL_LIGHTS_ON 0
#define ONLY_GLOBE 1
#define ONLY_REFLECTOR 2
#define ONLY_GRID 3
#define POSSIBLE_ILLUMINATING_MODES 4

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */

bool Tema3::coordsInGrid(int coordOx, int coordOz) {
    return ((coordOx >= 0) && (coordOz >= 0) &&
        (coordOx < sideDisco) && (coordOz < sideDisco));
}

std::vector<glm::vec3> Tema3::neighborsGridCell(int coordOx, int coordOz) {
    std::vector<glm::vec3> neighbors = {};

    for (int i = -1; i <= 1; i++) {
        // first check on coordOz - 1 row
        if (coordsInGrid(coordOx + i, coordOz - 1)) {
            neighbors.push_back(glm::vec3(coordOx + i, 0, coordOz - 1));
        }
        // secondly, check on coordOz row
        if (coordsInGrid(coordOx + i, coordOz)) {
            neighbors.push_back(glm::vec3(coordOx + i, 0, coordOz));
        }
        // lastly check on coordOz + 1 row
        if (coordsInGrid(coordOx + i, coordOz + 1)) {
            neighbors.push_back(glm::vec3(coordOx + i, 0, coordOz + 1));
        }
    }
    
    return neighbors;
}

Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    GetSceneCamera()->SetPosition(glm::vec3(0, 2, sideSquareDisco * (sideDisco) / 2 + sideSquareDisco * 3));

    danceFloor = (GridCell**)malloc(sizeof(GridCell*) * sideDisco);

    glm::vec3 cornerLeft = glm::vec3((float)-sideDisco / 2, HEIGHT_ROOF, (float)sideDisco / 2);

    Mesh* mesh_roof = object2D_Tema3::CreateSquare("roof", cornerLeft, sideDisco, glm::vec3(1, 1, 1), true);
    meshes[mesh_roof->GetMeshID()] = mesh_roof;

    // create the danceFloor
    for (int i = 0; i < sideDisco; i++) {
        danceFloor[i] = (GridCell*)malloc(sizeof(GridCell) * sideDisco);
    }

    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    for (int itRows = 0; itRows < sideDisco; itRows++) {
        for (int itCols = 0; itCols < sideDisco; itCols++) {
            glm::vec3 rand_color;
            rand_color[0] = (float)(rand() % 256) / 255;
            rand_color[1] = (float)(rand() % 256) / 255;
            rand_color[2] = (float)(rand() % 256) / 255;



            glm::vec3 cornerLeft = glm::vec3(sideSquareDisco * (itCols - sideDisco / 2), 0, sideSquareDisco * (itRows - sideDisco / 2 + 1));

            Mesh* mesh = object2D_Tema3::CreateSquare("GridCell" + itRows + itCols, cornerLeft, sideSquareDisco, rand_color, true);

            danceFloor[itRows][itCols] = GridCell(sideSquareDisco,
                cornerLeft,
                rand_color,
                mesh
            );
        }
    }

    walls = {};
    dancers = {};
    lightDirection = glm::vec3(0, 0, 0);
    reflectors = {};
    Mesh* mesh_2 = new Mesh("box");
    mesh_2->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh_2->GetMeshID()] = mesh_2;


    // push the left side walls
    for (int i = sideDisco - 1; i >= 0; i--) {
        glm::vec3 cornerDanceSquare = danceFloor[i][0].getCorner();
        glm::vec3 cornerWall = glm::vec3(cornerDanceSquare.x - 0.001f, 0, cornerDanceSquare.z);
        glm::vec3 colorWall = glm::vec3(0, 0, 0);

        // Mesh* mesh = object3D_Tema3::CreateParallelepiped("wall" + i + 0, cornerWall, 0.001, sideSquareDisco * 3, -sideSquareDisco, colorWall);
        Wall_Tema3 newWall = Wall_Tema3(cornerWall, colorWall, 0.001, sideSquareDisco,  HEIGHT_ROOF, mesh_2);
        newWall.setGridCellOx(0);
        newWall.setGridCellOz(i);

        walls.push_back(newWall);
    }

    // push the backward walls
    for (int j = 0; j < sideDisco; j++) {
        glm::vec3 cornerDanceSquare = danceFloor[0][j].getCorner();
        glm::vec3 cornerWall = glm::vec3(cornerDanceSquare.x, 0, cornerDanceSquare.z - sideSquareDisco);
        glm::vec3 colorWall = glm::vec3(0, 0, 0);

        //Mesh* mesh = object3D_Tema3::CreateParallelepiped("wall" + 0 + j, cornerWall, sideSquareDisco, sideSquareDisco * 3, -0.1, colorWall);
        Wall_Tema3 newWall = Wall_Tema3(cornerWall, colorWall, sideSquareDisco, 0.001, HEIGHT_ROOF, mesh_2);

        newWall.setGridCellOx(j);
        newWall.setGridCellOz(0);

        walls.push_back(newWall);
    }

    // push the right side walls
    for (int i = 0; i < sideDisco; i++) {
        glm::vec3 cornerDanceSquare = danceFloor[i][sideDisco - 1].getCorner();
        glm::vec3 cornerWall = glm::vec3(cornerDanceSquare.x + sideSquareDisco, 0, cornerDanceSquare.z);
        glm::vec3 colorWall = glm::vec3(0, 0, 0);

        //Mesh* mesh = object3D_Tema3::CreateParallelepiped("wall" + i + sideDisco - 1, cornerWall, 0.01, sideSquareDisco * 3, -sideSquareDisco, colorWall);
        Wall_Tema3 newWall = Wall_Tema3(cornerWall, colorWall, 0.001, sideSquareDisco, HEIGHT_ROOF, mesh_2);

        newWall.setGridCellOx(sideDisco - 1);
        newWall.setGridCellOz(i);

        walls.push_back(newWall);
    }

    float widthPlayer = 0.5f;
    float depthPlayer = 0.4f;
    float heightPlayer = 1.f;
    glm::vec3 centerPlayer = glm::vec3(-0.5, 0, -0.5);
    glm::vec3 colorPlayer = glm::vec3(1, 1, 1);

    // create the dancers
    for (int i = 0; i < NO_DANCERS; i++) {
        Mesh* mesh = object3D_Tema3::CreateParallelepiped("player" + i, glm::vec3(0, 0, 0) - glm::vec3(widthPlayer / 2, 0, depthPlayer / 2),
            widthPlayer, heightPlayer, depthPlayer, colorPlayer);


        Dancer dancer = Dancer(centerPlayer, sideSquareDisco, sideDisco,
            widthPlayer, depthPlayer, heightPlayer, colorPlayer, mesh_2);

        dancers.push_back(dancer);
    }

    // create the disco globe
    {
        Mesh* mesh = new Mesh("discoGlobe");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Shader* shader = new Shader("DanceFloorShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("DancerWallShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader_DancerWall.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader_DancerWall.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ReflectorShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader_Reflector.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader_Reflector.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("DiscoGlobeShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "VertexShader_DiscoGlobe.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema3", "shaders", "FragmentShader_DiscoGlobe.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }


    // create cons for reflectors
    for (int i = 0; i < NO_REFLECTORS; i++) {
        Mesh* mesh_1 = object3D_Tema3::CreateCone("cone" + i, glm::vec3(0, -1, 0), 1, 1, glm::vec3(1, 0, 0));
    

        Reflector reflector = Reflector(sideSquareDisco / 2, HEIGHT_ROOF, glm::vec3(0, 0, 0), glm::vec3(0, -1, 0), glm::vec3(1, 0, 0),
            15, mesh_1);

        reflectors.push_back(reflector);
    }

    noLightSources = 0;
    for (int i = 0; i < 9; i++) {
        pointLightPos[i] = glm::vec3(0, 0, 0);
        pointLightColor[i] = glm::vec3(0, 0, 0);
    }

    reflectors[0].setCoords(glm::vec3(-2, 0, -2));
    // for lila
    reflectors[0].setColor(glm::vec3((float)200 / 256, (float)162 / 256, (float)200 / 256));

    reflectors[1].setCoords(glm::vec3(2, 0, -2));
    // for orange
    reflectors[1].setColor(glm::vec3((float)255 / 256, (float)165 / 256, 0));

    reflectors[2].setCoords(glm::vec3(-2, 0, 2));
    // for yellow
    reflectors[2].setColor(glm::vec3((float)255 / 256, (float)255 / 256, 0));

    reflectors[3].setCoords(glm::vec3(2, 0, 2));
    // for turqoise
    reflectors[3].setColor(glm::vec3((float)64 / 256, (float)224 / 256, (float)208 / 256));

    for (int i = 0; i < NO_REFLECTORS; i++) {
        reflectorsColor[i] = reflectors[i].getColor();
        reflectorsPos[i] = reflectors[i].getCoords() + glm::vec3(0, reflectors[i].getHeight(), 0);
        reflectorsDirection[i] = glm::vec3(0, -1, 0);
    }

    // create texture for disco globe
    {
        mapTextures["random"] = CreateRandomTexture(16, 16);
    }

    Shininess = 30;
    Kd = 0.5;
    Ks = 0.5;
    gameState = GAME_IN_PROGRESS;
    noSpotlights = 0;
    cutoffAngle = 0;
    modeLights = ALL_LIGHTS_ON;

    reflectorsOn = 1;
    discoGlobeOn = 1;
    lightsGridOn = 1;
}

Texture2D* Tema3::CreateRandomTexture(unsigned int width, unsigned int height)
{
    GLuint textureID = 0;
    unsigned int channels = 3;
    unsigned int size = width * height * channels;
    unsigned char* data = new unsigned char[size];

    // TODO(student): Generate random a data
    for (int i = 0; i < size; i++) {
        data[i] = rand() % 255;
    }

    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER, MAG_FILTER and WRAPPING MODE) using glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();

    // Use glTexImage2D to set the texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
}

void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema3::Update(float deltaTimeSeconds)
{

    if (gameState == GAME_DEVELOPMENT) {
        for (int i = 0; i < NO_REFLECTORS; i++) {
            Reflector crtReflector = reflectors.at(i);
            
            
            // se vor desena doar fatetele fata
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);

            // aceasta directiva este folosita pentru nu se scrie in depth buffer
            glDepthMask(GL_FALSE);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            // desenare conuri
            RenderSimpleMesh(crtReflector.getAssociatedMesh(), shaders["ReflectorShader"], glm::mat4(1), crtReflector.getColor());
                
        
        }

        
        // se dezactiveaza actiunile tuturor directivelor apelate anterior
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);
        
    }
    else if (gameState == GAME_IN_PROGRESS) {
        glm::mat4 modelMatrix = glm::mat4(1);

        if (discoGlobeOn != 0) {
            glm::mat4 modelMatrixGlobe = glm::translate(modelMatrix, glm::vec3(0, HEIGHT_ROOF - 0.5f, 0));
            textureToShader = mapTextures["random"];
            RenderSimpleMesh(meshes["discoGlobe"], shaders["DiscoGlobeShader"], modelMatrixGlobe, glm::vec3(1, 0, 0));
        }

        textureToShader = NULL;
        textureToShader = mapTextures["random"];

        for (int itRows = 0; itRows < sideDisco; itRows++) {
            for (int itCols = 0; itCols < sideDisco; itCols++) {
                Mesh* mesh = danceFloor[itRows][itCols].getAssociatedMesh();
                glm::vec3 color = danceFloor[itRows][itCols].getColor();

                noLightSources = 0;
                RenderSimpleMesh(mesh, shaders["DanceFloorShader"], modelMatrix, color);
            }
        }
        textureToShader = NULL;

        // print walls
        for (int i = 0; i < walls.size(); i++) {
            Wall_Tema3 crtWall = walls.at(i);
            glm::vec3 color = crtWall.getColor();

            int gridCellOx = crtWall.getGridCellOx();
            int gridCellOz = crtWall.getGridCellOz();

            pointLightPos[0] = danceFloor[gridCellOz][gridCellOx].getLightSourcePos();
            pointLightColor[0] = danceFloor[gridCellOz][gridCellOx].getColor();

            glm::mat4 modelMatrixWall;
            if ((i < sideDisco) || (i >= walls.size() - sideDisco)) {
                modelMatrixWall = glm::translate(glm::mat4(1), crtWall.getCorner() + glm::vec3(0, 0, -(float)sideSquareDisco / 2));
            }
            else {
                modelMatrixWall = glm::translate(glm::mat4(1), crtWall.getCorner() + glm::vec3((float)sideSquareDisco / 2, 0, 0));
            }
            modelMatrixWall = glm::translate(modelMatrixWall, glm::vec3(0, crtWall.getHeight() / 2, 0));
            modelMatrixWall = glm::scale(modelMatrixWall, glm::vec3(crtWall.getWidth(), crtWall.getHeight(), crtWall.getDepth()));


            noLightSources = 1;
            noSpotlights = NO_REFLECTORS;
            textureToShader = mapTextures["random"];
            RenderSimpleMesh(crtWall.getAssociatedMesh(), shaders["DancerWallShader"], modelMatrixWall, color);
        }

        // print dancers
        for (int i = 0; i < NO_DANCERS; i++) {
            Dancer crtDancer = dancers.at(i);

            // coords of the grid cells around (maximum 9)
            std::vector<glm::vec3> neighbors = neighborsGridCell(crtDancer.getCoordsMatrix().x, crtDancer.getCoordsMatrix().z);

            noSpotlights = NO_REFLECTORS;
            noLightSources = neighbors.size();

            //cout << noLightSources << std::endl;
            for (int it = 0; it < noLightSources; it++) {
                glm::vec3 crtNeighCoords = neighbors.at(it);
                int ox = crtNeighCoords[0];
                int oz = crtNeighCoords[2];

                pointLightPos[it] = danceFloor[oz][ox].getLightSourcePos();
                pointLightColor[it] = danceFloor[oz][ox].getColor();

                //  cout << "iluminare de la pozitia: " << pointLightPos[it] << " " << "cu culoarea: " << pointLightColor[it] << std::endl;
            }
            //cout << std::endl;

            glm::mat4 modelMatrixDancer = glm::translate(glm::mat4(1), glm::vec3(0, crtDancer.getHeight() / 2, 0));
            modelMatrixDancer = glm::translate(modelMatrixDancer, crtDancer.getCoords());
            modelMatrixDancer = glm::scale(modelMatrixDancer, glm::vec3(crtDancer.getWidth(), crtDancer.getHeight(), crtDancer.getDepth()));
            RenderSimpleMesh(crtDancer.getAssociatedMesh(), shaders["DancerWallShader"], modelMatrixDancer, crtDancer.getColor());

            dancers.at(i).setTimeElapsedDance(dancers.at(i).getTimeElapsedDance() + deltaTimeSeconds);
            dancers.at(i).dance();
        }

        textureToShader = NULL;

        if (reflectorsOn != 0) {

            for (int i = 0; i < NO_REFLECTORS; i++) {
                Reflector crtReflector = reflectors.at(i);


                // se vor desena doar fatetele fata
                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);

                // aceasta directiva este folosita pentru nu se scrie in depth buffer
                glDepthMask(GL_FALSE);

                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                // desenare conuri
                glm::mat4 modelMatrixReflector = glm::mat4(1);
                modelMatrixReflector = glm::translate(modelMatrixReflector, crtReflector.getCoords() + glm::vec3(0, crtReflector.getHeight(), 0));
                modelMatrixReflector = glm::rotate(modelMatrixReflector, crtReflector.getAngleOx(), glm::vec3(1, 0, 0));
                modelMatrixReflector = glm::rotate(modelMatrixReflector, crtReflector.getAngleOy(), glm::vec3(0, 1, 0));
                modelMatrixReflector = glm::rotate(modelMatrixReflector, crtReflector.getAngleOz(), glm::vec3(0, 0, 1));
                modelMatrixReflector = glm::scale(modelMatrixReflector, glm::vec3(tan(glm::radians(crtReflector.getCutoffAngle())), 1, tan(glm::radians(crtReflector.getCutoffAngle()))) * crtReflector.getHeight());
                cutoffAngle = crtReflector.getCutoffAngle();

                RenderSimpleMesh(crtReflector.getAssociatedMesh(), shaders["ReflectorShader"], modelMatrixReflector, crtReflector.getColor());

                reflectors.at(i).move(deltaTimeSeconds);
                reflectorsDirection[i] = crtReflector.getLightDirection();
                //cout << reflectorsDirection[0] << std::endl;
            }
        }


        // se dezactiveaza actiunile tuturor directivelor apelate anterior
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);

        modelMatrix = glm::mat4(1);
        textureToShader = mapTextures["random"];
        // draw the roof
        RenderSimpleMesh(meshes["roof"], shaders["DanceFloorShader"], modelMatrix, glm::vec3(0, 0, 0));
    }
}


void Tema3::FrameEnd()
{
    DrawCoordinateSystem();
}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

        
    // direction of spotlights
    int reflectorsDirection_location = glGetUniformLocation(shader->program, "reflectorsDirection");
    glUniform3fv(reflectorsDirection_location, 4, glm::value_ptr(reflectorsDirection[0]));;

    // location of multiple light positions
    int pointLightPos_location = glGetUniformLocation(shader->program, "pointLightPos");
    glUniform3fv(pointLightPos_location, 9, glm::value_ptr(pointLightPos[0]));

    // color of multiple light sources
    int pointLightColor_location = glGetUniformLocation(shader->program, "pointLightColor");
    glUniform3fv(pointLightColor_location, 9, glm::value_ptr(pointLightColor[0]));

    // location of multiple spotlights
    int reflectorsPos_location = glGetUniformLocation(shader->program, "reflectorsPos");
    glUniform3fv(reflectorsPos_location, 4, glm::value_ptr(reflectorsPos[0]));

    // color of multiple spotlight color
    int reflectorsColor_location = glGetUniformLocation(shader->program, "reflectorsColor");
    glUniform3fv(reflectorsColor_location, 4, glm::value_ptr(reflectorsColor[0]));

    // number of spotlights
    int noSpotlights_location = glGetUniformLocation(shader->program, "noSpotlights");
    glUniform1i(noSpotlights_location, noSpotlights);

    // number of light sources
    int noLightSources_location = glGetUniformLocation(shader->program, "noLightSources");
    glUniform1i(noLightSources_location, noLightSources);

    // cutoff angle (if spotlight applies)
    int cutoffAngle_location = glGetUniformLocation(shader->program, "cutoff_angle");
    glUniform1f(cutoffAngle_location, cutoffAngle);

    // Set eye position (camera position) uniform
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    int object_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3f(object_color, color.r, color.g, color.b);

    // Set material property uniforms (shininess, kd, ks, object color) 
    int material_shininess_loc = glGetUniformLocation(shader->program, "material_shininess");
    glUniform1i(material_shininess_loc, Shininess);

    int material_kd_loc = glGetUniformLocation(shader->program, "material_kd");
    glUniform1f(material_kd_loc, Kd);

    int material_ks_loc = glGetUniformLocation(shader->program, "material_ks");
    glUniform1f(material_ks_loc, Ks);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // send texture
    if (textureToShader) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureToShader->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

        float timeElapsed = Engine::GetElapsedTime();
        int loc_time = glGetUniformLocation(shader->program, "time");
        glUniform1f(loc_time, timeElapsed);
    }

    // send disco ball position
    int loc_discoBall = glGetUniformLocation(shader->program, "disco_ball_position");
    glUniform3f(loc_discoBall, 0, HEIGHT_ROOF - (float) 1 / 2, 0);

    int loc_globeOn = glGetUniformLocation(shader->program, "globeOn");
    glUniform1i(loc_globeOn, discoGlobeOn);

    int loc_reflectorsOn = glGetUniformLocation(shader->program, "reflectorsOn");
    glUniform1i(loc_reflectorsOn, reflectorsOn);

    int loc_gridLightsOn = glGetUniformLocation(shader->program, "gridLightsOn");
    glUniform1i(loc_gridLightsOn, lightsGridOn);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    float speed = 2;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));

        // Control light position using on W, A, S, D, E, Q
        if (window->KeyHold(GLFW_KEY_W)) lightPosition -= forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_A)) lightPosition -= right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_S)) lightPosition += forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_D)) lightPosition += right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_E)) lightPosition += up * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_Q)) lightPosition -= up * deltaTime * speed;

        // TODO(student): Set any other keys that you might need
        if (window->KeyHold(GLFW_KEY_H)) cutoff_angle -= 5 * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_K)) cutoff_angle += 5 * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_U)) lightDirection.z -= 2 * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_J)) lightDirection.z += 2 * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_G)) lightDirection.x -= 2 * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_L)) lightDirection.x += 2 * deltaTime * speed;
        lightDirection = glm::normalize(lightDirection);

    }
}


void Tema3::OnKeyPress(int key, int mods)
{
    // Add key press even

    if (key == GLFW_KEY_Y) {
        // OX
        reflectors.at(0).setAngleOx(reflectors.at(0).getAngleOx() - 0.1);
        reflectors.at(0).updateLightDirection();
    } 
    if (key == GLFW_KEY_I) {
        // OZ
        reflectors.at(0).setAngleOz(reflectors.at(0).getAngleOz() - 0.1);
        reflectors.at(0).updateLightDirection();
    }
    if (key == GLFW_KEY_U) {
        // OX
        reflectors.at(0).setAngleOx(reflectors.at(0).getAngleOx() + 0.1);
        reflectors.at(0).updateLightDirection();
    }
    if (key == GLFW_KEY_H) {
        // OY
        reflectors.at(0).setAngleOy(reflectors.at(0).getAngleOy() - 0.1);
        reflectors.at(0).updateLightDirection();
    }
    if (key == GLFW_KEY_J) {
        // OZ
        reflectors.at(0).setAngleOz(reflectors.at(0).getAngleOz() + 0.1);
        reflectors.at(0).updateLightDirection();
    }
    if (key == GLFW_KEY_K) {
        // OY
        reflectors.at(0).setAngleOy(reflectors.at(0).getAngleOy() + 0.1);
        reflectors.at(0).updateLightDirection();
    }

    if (key == GLFW_KEY_P) {
        switch (modeLights) {
            case ALL_LIGHTS_ON:
                modeLights = ONLY_GLOBE;
                discoGlobeOn = 1;
                reflectorsOn = 0;
                lightsGridOn = 0;
                break;
            case ONLY_GLOBE:
                modeLights = ONLY_REFLECTOR;
                discoGlobeOn = 0;
                reflectorsOn = 1;
                lightsGridOn = 0;
                break;
            case ONLY_REFLECTOR:
                modeLights = ONLY_GRID;
                discoGlobeOn = 0;
                reflectorsOn = 0;
                lightsGridOn = 1;
                break;
            case ONLY_GRID:
                modeLights = ALL_LIGHTS_ON;
                discoGlobeOn = 1;
                reflectorsOn = 1;
                lightsGridOn = 1;
                break;
            default:
                break;
        }
    }

}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
