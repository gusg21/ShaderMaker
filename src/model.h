//
// Created by angus.goucher on 4/5/2024.
//

#ifndef SHADERMAKER_MODEL_H
#define SHADERMAKER_MODEL_H

#include <string>
#include <vector>
#include "mesh.h"

namespace sm {
    class Model {
    public:
        Model(const std::string& filePath);
        void draw();
    private:
        std::vector<sm::Mesh> m_meshes;
    };
};

#endif //SHADERMAKER_MODEL_H
