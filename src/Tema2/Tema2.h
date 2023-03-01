#pragma once

#include "components/simple_scene.h"
#include "RaceTrackGeneration.h"
#include "tema_camera.h"
#include "util_tema2.h"
#include "GrassTerrain.h"
#include "TreeGeneration.h"
#include <time.h>
#include <ctime>
#include <chrono>
#include <deque>

#define FOV RADIANS(60)
#define CLOSEZ_P 0.01f
#define FARZ_P 200.0f
#define O_WIDTH2 25.0f
#define CAMERA_SIZE_STEP 5.0f
#define INITIAL_CAR_SIZE 0.15f
#define WIDTH_DIV 3
#define HEIGHT_DIV 3
#define TRACK_Y 0.1f
#define CAR_Y 0.1f
#define ENEMY_COUNT 14
#define ENEMY_SPEED 4.5f
#define CAR_Z_SIZE 3
#define CAR_SPEED 10.0f
#define CAR_ROTATION 2.0f
#define TREE_HEIGHT 3.0f
#define TREE_CROWN_RATIO 2
#define TREE_RADIUS 2.0f
#define CAMERA_DIST 4

using namespace tema2;

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

     private:

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix);
        void RenderScene();
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        
        void putCarOnTrack();
        bool isInsideTrack(glm::vec3 position);
        void generateEnemies();
        void updateEnemies(double deltaTime);
        glm::dvec2 getPlaceInTrack(int pos, double offset, double traveled, int dir);
        bool intersectsEnemies();

        Camera *perspectiveCamera;
        Camera *aboveCamera;
        glm::mat4 projectionMatrixP;
        glm::mat4 projectionMatrixO;

        Camera *currentCamera;
        glm::mat4 currentProjectionMatrix;

        Track track;
        glm::mat4 carModelMatrix;

        vector<int> enemySegment;
        vector<double> enemyOffsetX;
        vector<double> enemySegmentTraveled;
        vector<int> enemyDir;

        vector<glm::dvec2> trees;
        glm::vec3 COLORS[ENEMY_COUNT] = {
            GREEN,
            PURPLE,
            WHITE,
            BLACK,
            RED,
            YELLOW,
            BLUE,
            GREEN,
            PURPLE,
            WHITE,
            BLACK,
            RED,
            YELLOW,
            BLUE,
        };

        float speed = CAR_SPEED;
        float rotationSpeed = CAR_ROTATION;
        float carRotation = 0;
        float carSize = INITIAL_CAR_SIZE;
        double aboveCameraViewSize = O_WIDTH2;
    };
}   // namespace m1
