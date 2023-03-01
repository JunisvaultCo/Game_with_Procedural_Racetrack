#include "Tema2/Tema2.h"
#include "RaceTrackGeneration.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}



void Tema2::Init()
{
    perspectiveCamera = new tema2::Camera();
    aboveCamera = new tema2::Camera();
    //stop clipping
    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    //creating C A R
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-1, 0,  CAR_Z_SIZE), CYAN),
            VertexFormat(glm::vec3( 1, 0,  CAR_Z_SIZE), CYAN),
            VertexFormat(glm::vec3(-1, 2,  CAR_Z_SIZE), CYAN),
            VertexFormat(glm::vec3( 1, 2,  CAR_Z_SIZE), CYAN),
            VertexFormat(glm::vec3(-1, 0, -CAR_Z_SIZE), CYAN),
            VertexFormat(glm::vec3( 1, 0, -CAR_Z_SIZE), CYAN),
            VertexFormat(glm::vec3(-1, 2, -CAR_Z_SIZE), CYAN),
            VertexFormat(glm::vec3( 1, 2, -CAR_Z_SIZE), CYAN),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,    // indices for first triangle
            1, 3, 2,    // indices for second triangle
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4,
        };

        meshes["car"] = utiltema2::CreateMesh("car", vertices, indices);
        
        for (int i = 0; i < ENEMY_COUNT; i++) {
            char c[9] = "";
            sprintf(c, "car%d", i);
            vector<VertexFormat> vertices2;
            for (auto j: vertices) {
                vertices2.push_back(VertexFormat(j.position, COLORS[i]));
            }
            meshes[c] = utiltema2::CreateMesh(c, vertices2, indices);
        }
    }

  //  for (int i = 0; i < 25000; i++) {
        track = createTrack(TRACK_Y);
  //  }
    meshes["track"] = track.m;
    putCarOnTrack();
    
    meshes["grass"] = tema2::generateGrass();
    {
        double sizee = TREE_RADIUS / 2;
        double y = TREE_HEIGHT;
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-sizee, y,          sizee), DARK_GREEN),
            VertexFormat(glm::vec3( sizee, y,          sizee), DARK_GREEN),
            VertexFormat(glm::vec3(-sizee, y + sizee,  sizee), DARK_GREEN),
            VertexFormat(glm::vec3( sizee, y + sizee,  sizee), DARK_GREEN),
            VertexFormat(glm::vec3(-sizee, y,         -sizee), DARK_GREEN),
            VertexFormat(glm::vec3( sizee, y,         -sizee), DARK_GREEN),
            VertexFormat(glm::vec3(-sizee, y + sizee, -sizee), DARK_GREEN),
            VertexFormat(glm::vec3( sizee, y + sizee, -sizee), DARK_GREEN),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,    // indices for first triangle
            1, 3, 2,    // indices for second triangle
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4,
        };
        meshes["tree_crown"] = utiltema2::CreateMesh("tree_crown", vertices, indices);
    }
    {
        double sizee = TREE_RADIUS / (2 * TREE_CROWN_RATIO);
        double h = TREE_HEIGHT;
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-sizee, 0,  sizee), BROWN),
            VertexFormat(glm::vec3( sizee, 0,  sizee), BROWN),
            VertexFormat(glm::vec3(-sizee, h,  sizee), BROWN),
            VertexFormat(glm::vec3( sizee, h,  sizee), BROWN),
            VertexFormat(glm::vec3(-sizee, 0, -sizee), BROWN),
            VertexFormat(glm::vec3( sizee, 0, -sizee), BROWN),
            VertexFormat(glm::vec3(-sizee, h, -sizee), BROWN),
            VertexFormat(glm::vec3( sizee, h, -sizee), BROWN),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,    // indices for first triangle
            1, 3, 2,    // indices for second triangle
            2, 3, 7,
            2, 7, 6,
            1, 7, 3,
            1, 5, 7,
            6, 7, 4,
            7, 5, 4,
            0, 4, 1,
            1, 4, 5,
            2, 6, 4,
            0, 2, 4,
        };
        meshes["tree_stalk"] = utiltema2::CreateMesh("tree_stalk", vertices, indices);
    }
    // Create a shader program for drawing face polygon with the color of the normal
    {
        Shader *shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src", "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src", "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
    
    projectionMatrixP = glm::perspective(FOV, window->props.aspectRatio, CLOSEZ_P, FARZ_P);
    float left = -aboveCameraViewSize * window->props.aspectRatio;
    float right = aboveCameraViewSize * window->props.aspectRatio;
    float bottom = -aboveCameraViewSize;
    float top = aboveCameraViewSize;
    projectionMatrixO = glm::ortho(left, right, bottom, top, CLOSEZ_P, FARZ_P);

}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(SKY_COLOR);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}


void Tema2::Update(float deltaTimeSeconds)
{
    updateEnemies(deltaTimeSeconds);
}

void Tema2::RenderScene()
{
    // Render the C A R
    carModelMatrix = glm::mat4(1);
    carModelMatrix = glm::translate(carModelMatrix, perspectiveCamera->GetTargetPosition());
    carModelMatrix = glm::scale(carModelMatrix, glm::vec3(carSize, carSize, carSize));
    carModelMatrix = glm::rotate(carModelMatrix, carRotation, glm::vec3(0, 1, 0));
    RenderSimpleMesh(meshes["car"], shaders["TemaShader"], carModelMatrix);
    RenderSimpleMesh(meshes["track"], shaders["TemaShader"], glm::mat4(1));
    RenderSimpleMesh(meshes["grass"], shaders["TemaShader"], glm::mat4(1));
    for (int i = 0; i < ENEMY_COUNT; i++) {
        char c[9] = "";
        sprintf(c, "car%d", i);
        glm::vec2 place = getPlaceInTrack(enemySegment[i], enemyOffsetX[i], enemySegmentTraveled[i], enemyDir[i]);
        glm::mat4 matrix = glm::translate(glm::mat4(1),
                glm::vec3(place.x, CAR_Y, place.y));
        matrix = glm::scale(matrix, glm::vec3(carSize, carSize, carSize));
        int pos = enemySegment[i];
        int n = track.inner.size();
        double angle = -getAngleClock(track.inner[pos], track.inner[pos] + glm::dvec2(1, 0), track.outer[pos], true);
        matrix = glm::rotate(matrix, (float)angle, glm::vec3(0, 1, 0));
        RenderSimpleMesh(meshes[c], shaders["TemaShader"], matrix);
    }
    for (int i = 0; i < (int)trees.size(); i++) {
        glm::mat4 matrix = glm::translate(glm::mat4(1), glm::vec3(trees[i].x, 0, trees[i].y));
        RenderSimpleMesh(meshes["tree_crown"], shaders["TemaShader"], matrix);
        RenderSimpleMesh(meshes["tree_stalk"], shaders["TemaShader"], matrix);
    }
}


void Tema2::FrameEnd()
{
    currentProjectionMatrix = projectionMatrixP;
    currentCamera = perspectiveCamera;
    
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
    RenderScene();
    int width = resolution.x / WIDTH_DIV;
    int height = resolution.y / HEIGHT_DIV;
    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(resolution.x - width, 5, width, height);

    // render the scene again, in the new viewport
    currentProjectionMatrix = projectionMatrixO;
    currentCamera = aboveCamera;
    RenderScene();
}


void Tema2::RenderSimpleMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(currentCamera->GetViewMatrix()));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(currentProjectionMatrix));
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    GLint loc_car_pos = glGetUniformLocation(shader->program, "car_position");
    glm::vec3 position = perspectiveCamera->GetTargetPosition();
    glUniform3fv(loc_car_pos, 1, glm::value_ptr(position));

    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    if (!intersectsEnemies())
    {
        if (window->KeyHold(GLFW_KEY_W)) {
            glm::vec3 old = perspectiveCamera->GetTargetPosition();
            perspectiveCamera->MoveForward(speed * deltaTime);
            if (isInsideTrack(perspectiveCamera->GetTargetPosition())) {
                glm::vec3 delta = perspectiveCamera->GetTargetPosition() - old;
                aboveCamera->TranslateUpward(-delta.z);
                aboveCamera->TranslateRight(delta.x);
            } else {
                perspectiveCamera->MoveForward(- speed * deltaTime);
            }
        }

        if (window->KeyHold(GLFW_KEY_A)) {
            perspectiveCamera->RotateThirdPerson_OY(-deltaTime * rotationSpeed);
            carRotation += deltaTime * rotationSpeed;
        }

        if (window->KeyHold(GLFW_KEY_S)) {
            glm::vec3 old = perspectiveCamera->GetTargetPosition();
            perspectiveCamera->MoveForward(-speed * deltaTime);
            if (isInsideTrack(perspectiveCamera->GetTargetPosition())) {
                glm::vec3 delta = perspectiveCamera->GetTargetPosition() - old;
                aboveCamera->TranslateUpward(-delta.z);
                aboveCamera->TranslateRight(delta.x);
            } else {
                perspectiveCamera->MoveForward(+speed * deltaTime);
            }
        }

        if (window->KeyHold(GLFW_KEY_D)) {
            perspectiveCamera->RotateThirdPerson_OY(+deltaTime * rotationSpeed);
            carRotation -= deltaTime * rotationSpeed;
        }
    }
    if (window->KeyHold(GLFW_KEY_E))
        perspectiveCamera->RotateThirdPerson_OX(+deltaTime * rotationSpeed);
    if (window->KeyHold(GLFW_KEY_Q))
        perspectiveCamera->RotateThirdPerson_OX(-deltaTime * rotationSpeed);

    if (window->KeyHold(GLFW_KEY_X)) {
        aboveCameraViewSize += deltaTime * CAMERA_SIZE_STEP;
        float left = -aboveCameraViewSize * window->props.aspectRatio;
        float right = aboveCameraViewSize * window->props.aspectRatio;
        float bottom = -aboveCameraViewSize;
        float top = aboveCameraViewSize;
        projectionMatrixO = glm::ortho(left, right, bottom, top, CLOSEZ_P, FARZ_P);
    }
    if (window->KeyHold(GLFW_KEY_Z)) {
        aboveCameraViewSize -= deltaTime * CAMERA_SIZE_STEP;
        float left = -aboveCameraViewSize * window->props.aspectRatio;
        float right = aboveCameraViewSize * window->props.aspectRatio;
        float bottom = -aboveCameraViewSize;
        float top = aboveCameraViewSize;
        projectionMatrixO = glm::ortho(left, right, bottom, top, CLOSEZ_P, FARZ_P);
    }
}


void Tema2::OnKeyPress(int key, int mods) {
    if (key == (GLFW_KEY_O)) {
        track = createTrack(TRACK_Y);
        meshes["track"] = track.m;
        putCarOnTrack();
    }
}

void Tema2::putCarOnTrack() {
    double x = (track.inner[0].x + track.outer[0].x) / 2;
    double y = (track.inner[0].y + track.outer[0].y) / 2;
    glm::vec3 position = glm::vec3(x, CAR_Y, y);
    
    aboveCamera->Set(position + glm::vec3(0, 0, 1), position, glm::vec3(0, 1, 0), CAMERA_DIST);
    aboveCamera->TranslateForward(-CAMERA_DIST + 1);
    aboveCamera->RotateThirdPerson_OX(RADIANS(90));

    double angle = getAngleClock(track.inner[0], track.inner[0] + glm::dvec2(1, 0), track.outer[0], true);
    perspectiveCamera->Set(position + glm::vec3(0, 0, 1), position, glm::vec3(0, 1, 0), CAMERA_DIST);
    perspectiveCamera->TranslateForward(-CAMERA_DIST + 1);
    perspectiveCamera->RotateThirdPerson_OX(RADIANS(30));
    perspectiveCamera->RotateThirdPerson_OY(angle);

    carRotation = -angle;
    enemyOffsetX.clear();
    enemySegment.clear();
    enemySegmentTraveled.clear();
    enemyDir.clear();
    generateEnemies();
    trees = generateTrees(track, TREE_RADIUS + RACETRACK_WIDTH);
}

bool Tema2::isInsideTrack(glm::vec3 position) {
    glm::dvec2 position2D = glm::dvec2((double)position.x, (double)position.z);
    for (int i = 0; i < track.triangles.size(); i += 3) {
        int place1 = track.triangles[i];
        int place2 = track.triangles[i + 1];
        int place3 = track.triangles[i + 2];
        glm::dvec2 point1 = (place1 % 2 == 0) ? track.inner[place1 / 2] : track.outer[place1 / 2];
        glm::dvec2 point2 = (place2 % 2 == 0) ? track.inner[place2 / 2] : track.outer[place2 / 2];
        glm::dvec2 point3 = (place3 % 2 == 0) ? track.inner[place3 / 2] : track.outer[place3 / 2];
        int isAntiClockCount = isAntiClockWise(position2D, point1, point2);
        isAntiClockCount += isAntiClockWise(position2D, point2, point3);
        isAntiClockCount += isAntiClockWise(position2D, point3, point1);
        if (isAntiClockCount == 3 || isAntiClockCount == 0) {
            return true;
        }
    }
    return false;
}


void Tema2::generateEnemies() {
    for (int i = 0; i < ENEMY_COUNT; i++) {
        enemySegment.push_back(rand() % (int)track.inner.size());
        enemyOffsetX.push_back(betterRandom(RACETRACK_WIDTH));
        enemySegmentTraveled.push_back(0);
        enemyDir.push_back((rand() % 2 == 0) ? -1 : 1);
    }
}

glm::dvec2 Tema2::getPlaceInTrack(int pos, double offset, double traveled, int dir)
{
    double x = (track.inner[pos] - track.outer[pos]).x;
    double y = (track.inner[pos] - track.outer[pos]).y;
    int n = track.inner.size();
    glm::dvec2 direction = glm::normalize(track.inner[(pos + dir + n) % n] - track.inner[pos]);
    return offset * (glm::normalize(track.inner[pos] - track.outer[pos]))
            + track.outer[pos] + direction * traveled;
}

void Tema2::updateEnemies(double deltaTime) {
    int n = track.inner.size();
    for (int i = 0; i < ENEMY_COUNT; i++) {
        enemySegmentTraveled[i] += deltaTime * ENEMY_SPEED;
        while (true) {
            glm::dvec2 point1 = getPlaceInTrack(enemySegment[i],
                    enemyOffsetX[i], 0, enemyDir[i]);
            glm::dvec2 point2 = getPlaceInTrack((enemySegment[i] + enemyDir[i] + n) % n,
                    enemyOffsetX[i], 0, enemyDir[i]);
            if (enemySegmentTraveled[i] < dist(point1, point2))
                break;
            enemySegmentTraveled[i] = enemySegmentTraveled[i] - dist(point1, point2);
            enemySegment[i] = (enemySegment[i] + enemyDir[i] + n) % n;
        }
    }
}
bool Tema2::intersectsEnemies() {
    for (int i = 0; i < ENEMY_COUNT; i++) {
        glm::dvec3 playerPos = perspectiveCamera->GetTargetPosition(); 
        if (dist(glm::dvec2(playerPos.x, playerPos.z),
            getPlaceInTrack(enemySegment[i],
                enemyOffsetX[i], enemySegmentTraveled[i], enemyDir[i]
            ))
                < (carSize * CAR_Z_SIZE * 2) - EPS) {
            return true;
        }
    }
    return false;
}
void Tema2::OnKeyRelease(int key, int mods){}
void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}
void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}
void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}
void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}
void Tema2::OnWindowResize(int width, int height) {}