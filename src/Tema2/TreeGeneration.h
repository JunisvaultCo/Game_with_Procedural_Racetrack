#include "util_tema2.h"
#include "RaceTrackGeneration.h"

#define NON_TREE_RATIO 1.0f

//No, I did not try something easy to guarantee there is at least
//one tree visible at one point. This is, uh, for lore purposes.
//Yes. It's so that the drivers just hit the trees instead of killing
//in the viewers. There totally are viewers. They're just, uh, small.
//Yes. It is very safe.
namespace tema2 {
    vector<glm::dvec2> generateTrees(Track t, double treeRadius);
    bool intersectsTrack(Track t, double treeRadius, glm::dvec2 treePos);
}