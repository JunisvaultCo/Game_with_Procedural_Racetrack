#include "Tema2/RaceTrackGeneration.h"

using namespace std;

double getAngle(glm::dvec2 a, glm::dvec2 b, glm::dvec2 c) {
    double distAB = dist(a, b);
    double distAC = dist(a, c);
    double prod = scalarProduct(b - a, c - a);
    if (equals(0, distAB * distAC))
        return 0;
    double angle = prod / (distAB * distAC);
    if (angle > 1)
        angle -= EPS;
    else if (angle < -1)
        angle += EPS;
	if (equals(0, angle))
		return glm::acos(0);
	return glm::acos(angle);
}

double getAngleClock(glm::dvec2 a, glm::dvec2 b, glm::dvec2 c, bool isAntiClock) {
    double angle = getAngle(a, b, c);
	if (isAntiClockWise(b, a, c) == isAntiClock)
		return angle;
	else
		return 2 * M_PI - angle;
}

bool equals(double a, double b) {
    return a >= b - EPS && a <= b + EPS;
}

bool equalsBiggerErr(double a, double b) {
	return a >= b - BIGGER_EPS && a <= b + BIGGER_EPS;
}
bool equalsPoint(glm::dvec2 a, glm::dvec2 b) {
	return equals(a.x, b.x) && equals(a.y, b.y);
}
bool equalsPointBiggerErr(glm::dvec2 a, glm::dvec2 b) {
	return equalsBiggerErr(a.x, b.x) && equalsBiggerErr(a.y, b.y);
}
double makeZero(double a) {
	if (a >= -EPS && a <= EPS)
		return 0;
	return a;
}

glm::dvec2 getIntersection(glm::dvec2 l1_1, glm::dvec2 l1_2, glm::dvec2 l2_1, glm::dvec2 l2_2, bool &exists) {
    double x1 = l1_1.x;
	double x2 = l1_2.x;
	double y1 = l1_1.y;
	double y2 = l1_2.y;

	double x3 = l2_1.x;
	double x4 = l2_2.x;
	double y3 = l2_1.y;
	double y4 = l2_2.y;

	double a1 = makeZero(y2 - y1);
	double b1 = makeZero(x2 - x1);
	double c1 = makeZero(-x1 * y2 + y1 * x2);

	double a2 = makeZero(y4 - y3);
	double b2 = makeZero(x4 - x3);
	double c2 = makeZero(-x3 * y4 + y3 * x4);
	double y = makeZero((c1 * a2 / a1 - c2) / (a2 * b1 / a1 - b2));
	double x = makeZero((b1 * y - c1) / a1);
	if (y >= (min(y1, y2) - EPS) && y <= (max(y1, y2) + EPS)
	&& y >= (min(y3, y4) - EPS) && y <= (max(y3, y4) + EPS)
	&& x >= (min(x1, x2) - EPS) && x <= (max(x1, x2) + EPS)
	&& x >= (min(x3, x4) - EPS) && x <= (max(x3, x4) + EPS))
	{
        exists = true;
		return glm::dvec2(x, y);
	}
	x = (c1 / b1 * b2 - c2) / (a2 - a1 / b1 * b2);
	y = (a1 * x + c1) / b1;
	if (y >= (min(y1, y2) - EPS) && y <= (max(y1, y2) + EPS)
	&& y >= (min(y3, y4) - EPS) && y <= (max(y3, y4) + EPS)
	&& x >= (min(x1, x2) - EPS) && x <= (max(x1, x2) + EPS)
	&& x >= (min(x3, x4) - EPS) && x <= (max(x3, x4) + EPS))
	{
        exists = true;
		return glm::dvec2(x, y);
	}
    exists = false;
	return glm::dvec2(1, 1);
}
double dist(glm::dvec2 a, glm::dvec2 b) {
	return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}
double scalarProduct(glm::dvec2 a, glm::dvec2 b) {
	return a.x * b.x + a.y * b.y;
}
bool isAntiClockWise(glm::dvec2 a, glm::dvec2 b, glm::dvec2 c) {
	return a.x * b.y + b.x * c.y + c.x * a.y - c.x * b.y - a.x * c.y - b.x * a.y < -EPS;
}
glm::dvec2 symmetry(glm::dvec2 a, glm::dvec2 b) {
	return glm::dvec2(b.x + 1,  b.y);
}

struct Comp : binary_function<glm::dvec3, glm::dvec3, bool>
{
    bool operator() (glm::dvec3 a, glm::dvec3 b) {
        if (!equals(a.x, b.x))
            return a.x < b.x;
        return a.y < b.y;
    }
};

vector<glm::dvec2> prepareConcaveHull(vector<glm::dvec2> poly) {
    vector<glm::dvec2> concaveHull = vector<glm::dvec2>();
    vector<glm::dvec3> toSort = vector<glm::dvec3>();
    for (unsigned int i = 0; i < poly.size(); i++)
    {
        toSort.push_back(glm::dvec3(poly[i], i));
    }
    sort(toSort.begin(), toSort.end(), Comp());
    //We use the last and first points to determine
    //the initial angle we start at and a point
    //that is certainly part of the concave hull.
    //We try thinking about lines that are from i1, j1 to i2, j2
    //and we are currently on point current.
	int i1 = toSort[0].z;
    int i2 = toSort[toSort.size() - 1].z;
    glm::dvec2 startP;
    glm::dvec2 startP2;
    int countt = 0;
    glm::dvec2 current = glm::dvec2(poly[i2].x, poly[i2].y);
	glm::dvec2 previous = symmetry(poly[i1], poly[i2]);
	bool isAntiClock = true;
    while (countt < TRACK_MAX_POINTS)
    {
        double bestAngle;
        bool hasBest = false;
        int next;
        int prev;
        //finding the line our current line intersects with such that
        //the point "current" is part of with the closest angle difference

        for (int i = 0; i < poly.size(); i++)
        {
            int ni = (i + 1) % poly.size();
            bool foundIntersect = false;
            glm::dvec2 intersect = getIntersection(previous, poly[i2], poly[i], poly[ni], foundIntersect);
            if (!foundIntersect || !equalsPoint(current, intersect))
                continue;
            double newAngle = getAngleClock(current, previous, poly[ni], isAntiClock);

            if (!equals(0, newAngle) && !equalsPoint(poly[ni], current)
            && (!hasBest || newAngle < bestAngle))
            {
                hasBest = true;
                bestAngle = newAngle;
                prev = i;
                next = ni;
            }
            newAngle = getAngleClock(current, previous, poly[i], isAntiClock);

            if (!equals(0, newAngle) && !equalsPoint(poly[i], current)
            && (!hasBest || newAngle < bestAngle))
            {
                hasBest = true;
                bestAngle = newAngle;
                prev = ni;
                next = i;
            }
        }
        // exceptionally rare case, seems to be only due to
        // putting them all aproximatively on a single line
        if (!hasBest) {
            return vector<glm::dvec2>();
        }
        //on this line, find the closest intersection
        double bestDist = dist(current, poly[next]);
        glm::dvec2 newCurrent = poly[next];
        for (int i = 0; i < poly.size(); i++)
        {
            int ni = (i + 1) % poly.size();
            bool foundIntersect = false;
            glm::dvec2 intersect = getIntersection(current, poly[next], poly[i], poly[ni], foundIntersect);
            if (!foundIntersect
            || (ni == next && i == prev)
            || (ni == prev && i == next))
            {
                continue;
            }
            double currentDist = dist(current, intersect);
            if (equals(currentDist, 0))
            {
                continue;
            }
            if (currentDist < bestDist + EPS)
            {
                bestDist = currentDist;
                i2 = next;
                newCurrent = glm::dvec2(intersect.x, intersect.y);
            }
        }
        if (countt == 0)
            startP = glm::dvec2(current.x, current.y);
        else if (countt == 1)
            startP2 = glm::dvec2(current.x, current.y);
        else if (equalsPoint(startP2, newCurrent) && equalsPoint(startP, current))
            break;
        countt++;
		previous = glm::dvec2(current.x, current.y);
        current = glm::dvec2(newCurrent.x, newCurrent.y);
        concaveHull.push_back(current);
    }
    return concaveHull;
}
vector<glm::dvec2> useBezier(vector<glm::dvec2> poly) {
    vector<glm::dvec2> curved = vector<glm::dvec2>();
	for (unsigned int i = 0; i < poly.size(); i++) {
		int prevv = i - 1;
		if (prevv < 0)
			prevv = poly.size() - 1;
		int nextt = i + 1;
		if (nextt >= poly.size())
			nextt = 0;
		double angle = getAngle(poly[i], poly[prevv], poly[nextt]);
		if (angle > M_PI)
			angle = 2 * M_PI - angle;
		double coef = 0.5;
		
		curved.push_back(bezier2(poly[prevv], poly[i], coef));
		curved.push_back(poly[i]);
		curved.push_back(bezier2(poly[nextt], poly[i], coef));
	}
    vector<glm::dvec2> result = vector<glm::dvec2>();
	for (unsigned int i = 0; i < curved.size(); i += 3) {
		int b2 = i + 1;
		int b3 = i + 2;
		for (double t = 0; t <= 1; t += BEZIER_STEP)
			result.push_back(bezier3(curved[i], curved[b2], curved[b3], t));
	}
    return result;
}
glm::dvec2 bezier2(glm::dvec2 p0, glm::dvec2 p1, double t) {
	return (1 - t) * p0 + t * p1;
}
glm::dvec2 bezier3(glm::dvec2 p0, glm::dvec2 p1, glm::dvec2 p2, double t) {
	return (1 - t) * (1 - t) * p0 + 2 * (1 - t) * t * p1 + t * t * p2;
}

double betterRandom(double maximumVal) {
    double a = ((double) rand()) / RAND_MAX;
    return a * maximumVal;
}

bool isConvex(vector<glm::dvec2> poly) {
    int antiClockwises = 0;
    for (int i = 0; i < poly.size(); i++)
        antiClockwises += isAntiClockWise(poly[i], poly[(i + 1) % poly.size()], poly[(i + 2) % poly.size()]);
    return antiClockwises == 0 || antiClockwises == poly.size();
}

vector<glm::dvec2> initialTrackPoints() {
    static int generations = 0;
    // list of whacky:
    //1670936069
    //1670937029
    //1670937402

    //tracks that have to be skipped (they generate
    //aproximately on a single line):
    //Consider this an unimplemented unit test
    //1670249263;
    //1670289096;
    //1670298106;
    //1670943382;
    
    //1670943409??
    //1671231898    

    //other tracks that have to be skipped
    //1670950876;
    //1670956448;
    //1670950998;
    //1670950917;

    long long a = 1672473586;//time(NULL);//1672629293
    a += generations;
    generations++;
    printf("%d\n", a);
    srand(a);
    vector<glm::dvec2> poly = vector<glm::dvec2>();
    for (int i = 0; i < INITIAL_TRACK_SIZE; i++) {
        while (true) {
            double x = betterRandom(TRACK_SIZE);
            double y = betterRandom(TRACK_SIZE);
            if (i >= 1) {
                bool found = false;
                for (int j = 0; j < i; j++)
                    if (equalsPointBiggerErr(glm::dvec2(x, y), poly[j])) {
                        found = true;
                        break;
                    }
                if (found)
                    continue;
            }
            if (i >= 2) {
                double angle = getAngle(poly[i - 1], poly[i - 2], glm::dvec2(x, y));
                if ((angle / M_PI) * 180 < MIN_ANGLE)
                    continue;
                if ((angle / M_PI) * 180 > 180 - BIGGEST_EPS)
                    continue;
            }
            if (i == INITIAL_TRACK_SIZE - 1) {
                double angle = getAngle(glm::dvec2(x, y), poly[0], poly[i - 1]);
                if ((angle / M_PI) * 180 > 180 - BIGGEST_EPS)
                    continue;
            }
            poly.push_back(glm::dvec2(x, y));
            break;
        }
    }
    poly.push_back(poly[0]);
    return poly;
}

Track createTrack(double altitude) {
    int attempts = 1;
    int bigAttempts = 1;
    while (true) {
        vector<VertexFormat> vertices;
        vector<glm::dvec2> initialPoints = initialTrackPoints();
        
        /*
        int countt = 1;
        for (auto j: initialPoints) {
            printf("a_{%d} = (%llf,%llf)\n", countt, j.x, j.y);
            countt++;
        }//*/
        vector<glm::dvec2> concavePoints = prepareConcaveHull(initialPoints);
        
        if (concavePoints.size() == 0)
            continue;
        bool canContinue = true;
        int concaveN = concavePoints.size();
        for (int i = 0; i < concaveN; i++) {
            glm::dvec2 prevv = concavePoints[(i - 1 + concaveN) % concaveN];
            glm::dvec2 nextt = concavePoints[(i + 1) % concaveN];
            double angle = getAngle(concavePoints[i], prevv, nextt);
            if ((angle / M_PI) * 180 < MIN_ANGLE) {
                canContinue = false;
            }
            if (equalsPointBiggerErr(nextt, concavePoints[i]))
                canContinue = false;
            if (equalsPointBiggerErr(prevv, concavePoints[i]))
                canContinue = false;
        }
        attempts += !canContinue;
        if (!canContinue) continue;
        vector<glm::dvec2> curvedInside = /*concavePoints;//*/useBezier(concavePoints);
        vector<glm::dvec2> curvedOutside;
        int n = curvedInside.size();
        bool isAntiClock = true;
        for (int i = 0; i < n; i++) {
            glm::dvec2 prevv = curvedInside[(i - 1 + n) % n];
            glm::dvec2 nextt = curvedInside[(i + 1) % n];
            glm::dvec2 current = curvedInside[i];
            glm::dvec2 bisector = glm::normalize(glm::normalize(prevv - current) + glm::normalize(nextt - current));
            glm::dvec2 pointOnBis = bisector + current;
            glm::dvec2 delta = ((double)RACETRACK_WIDTH) * bisector * (1 / sin(getAngle(current, pointOnBis, prevv)));
            if (isAntiClockWise(prevv, pointOnBis, current) == isAntiClock) {
                delta = -delta;
            }
            glm::dvec2 newPoint = delta + current;
            curvedOutside.push_back(newPoint);
        }
        for (int i = 0; i < n && canContinue; i++) {
            for (int j = i + 1; j < n && canContinue; j++) {
                bool foundIntersection = false;
                glm::vec2 intersect = getIntersection(curvedInside[i], curvedOutside[i],
                                    curvedInside[j], curvedOutside[j], foundIntersection);
                if (foundIntersection)
                    canContinue = false;
            }
        }
        bigAttempts += !canContinue;
        if (!canContinue)
            continue;

        for (int i = 0; i < n; i++) {
            vertices.push_back(VertexFormat(glm::dvec3(curvedInside[i].x, altitude, curvedInside[i].y), GRAY));
            vertices.push_back(VertexFormat(glm::dvec3(curvedOutside[i].x, altitude, curvedOutside[i].y), GRAY));
        }
        int vsize = 2 * n;
        vector<unsigned int> indices;
        for (int i = 0; i < vsize; i += 2) {
            indices.push_back(i);
            indices.push_back((i + 1) % vsize);
            indices.push_back((i + 2) % vsize);
            
            indices.push_back((i + 1) % vsize);
            indices.push_back((i + 3) % vsize);
            indices.push_back((i + 2) % vsize);
        }
        if (isConvex(curvedInside)) {
            bigAttempts++;
            continue;
        }
        printf("attempts for this track: %d\n", attempts);
        printf("BIG attempts for this track: %d\n", bigAttempts);
        static int trackCount = 0;
        printf("track count: %d\n", trackCount);
        trackCount++;
        Mesh *m = utiltema2::CreateMesh("racetrack", vertices, indices);
        return {m, curvedInside, curvedOutside, indices};
    }
}