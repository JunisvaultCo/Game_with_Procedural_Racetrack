#include "Tema2/GrassTerrain.h"

namespace tema2 {
    Mesh *generateGrass() {
        double sizee = (TRACK_SIZE + RACETRACK_WIDTH) * 1.2;
        
        vector<VertexFormat> vertices;
        vector<unsigned int> indices;
        int columnCount = 0;
        for (double x = -sizee; x < sizee; x += TRIANGLE_ONCE) {
            for (double z = -sizee; z < sizee; z += TRIANGLE_ONCE)
                vertices.push_back(VertexFormat(glm::vec3(x, 0, z), GREEN));
            columnCount++;
        }
        for (int i = 0; i < columnCount - 1; i++) {
            for (int j = 0; j < columnCount - 1; j++) {
                indices.push_back(i * columnCount + j);
                indices.push_back(i * columnCount + j + 1);
                indices.push_back((i + 1) * columnCount + j);
                
                indices.push_back(i * columnCount + j + 1);
                indices.push_back((i + 1) * columnCount + j);
                indices.push_back((i + 1) * columnCount + j + 1);
            }
        }

        return utiltema2::CreateMesh("grass", vertices, indices);
    }
}