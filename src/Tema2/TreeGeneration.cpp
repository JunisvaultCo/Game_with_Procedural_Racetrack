#include "TreeGeneration.h"

namespace tema2 { 
    vector<glm::dvec2> generateTrees(Track t, double treeRadius) {
        vector<glm::dvec2> trees;
        for (int j = 0; j < 2; j++) {
            bool isFirstTree = true;
            glm::dvec2 lastTree;
            for (int i = 0; i < (int) t.inner.size(); i++) {
                glm::dvec2 newVector = t.inner[i] - t.outer[i];
                if (j == 1)
                    newVector = t.outer[i] - t.inner[i];
                glm::dvec2 newTree = normalize(newVector) * treeRadius;
                newTree = newTree + ((j == 1) ? t.inner[i] : t.outer[i]);
                if (intersectsTrack(t, treeRadius, newTree)) continue;
                if (isFirstTree || dist(lastTree, newTree) > treeRadius * NON_TREE_RATIO) {
                    isFirstTree = false;
                    lastTree = newTree;
                    trees.push_back(lastTree);
                }
            }
        }
        return trees;
    }
    bool intersectsTrack(Track t, double treeRadius, glm::dvec2 treePos) {
        for (int i = 0; i < (int) t.inner.size(); i++) {
            if (dist(t.inner[i], treePos) > treeRadius - EPS)
                continue;
            if (dist(t.outer[i], treePos) > treeRadius - EPS)
                continue;
            return true;
        }
        return false;
    }
};