#pragma once
#include "components/simple_scene.h"
#include "util_tema2.h"
#include <vector>
#include <functional>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#define EPS 1e-6
#define BIGGER_EPS 1
#define BIGGEST_EPS 5
#define BEZIER_STEP 0.03
#define INITIAL_TRACK_SIZE 10
#define TRACK_SIZE 80
#define TRACK_INITIAL_POINTS 10
#define TRACK_MAX_POINTS 100
#define MIN_ANGLE 45
#define RACETRACK_WIDTH 4

using namespace std;

struct Track {
    Mesh *m;
    vector<glm::dvec2> inner;
    vector<glm::dvec2> outer;
    vector<unsigned int> triangles;
};

double getAngle(glm::dvec2 a, glm::dvec2 b, glm::dvec2 c);
double getAngleClock(glm::dvec2 a, glm::dvec2 b, glm::dvec2 c, bool isAntiClock);
bool equals(double a, double b);
bool equalsBiggerErr(double a, double b);
bool equalsPoint(glm::dvec2 a, glm::dvec2 b);
bool equalsPointBiggerErr(glm::dvec2 a, glm::dvec2 b);
double makeZero(double a);
glm::dvec2 getIntersection(glm::dvec2 l1_1, glm::dvec2 l1_2, glm::dvec2 l2_1, glm::dvec2 l2_2, bool &exists);
double dist(glm::dvec2 a, glm::dvec2 b);
double scalarProduct(glm::dvec2 a, glm::dvec2 b);
bool isAntiClockWise(glm::dvec2 a, glm::dvec2 b, glm::dvec2 c);
glm::dvec2 symmetry(glm::dvec2 a, glm::dvec2 b);
vector<glm::dvec2> prepareConcaveHull(vector<glm::dvec2> poly);
vector<glm::dvec2> useBezier(vector<glm::dvec2> initial);
glm::dvec2 bezier2(glm::dvec2 p0, glm::dvec2 p1, double t);
glm::dvec2 bezier3(glm::dvec2 p0, glm::dvec2 p1, glm::dvec2 p2, double t);
vector<glm::dvec2> initialTrackPoints();
double betterRandom(double maximumVal);
bool isConvex(vector<glm::dvec2> poly);
Track createTrack(double altitude);