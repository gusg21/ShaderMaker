//
// Created by angus.goucher on 4/2/2024.
//

#ifndef SHADERMAKER_SHADER_H
#define SHADERMAKER_SHADER_H

#include <string>

namespace sm {
    class Shader {
    public:
        Shader(const std::string &fragment, const std::string &vertex);

        void use();

    private:
        uint32_t program;
    };

    Shader* loadShader(const std::string &fragmentFile, const std::string &vertexFile);
} // sm

#endif //SHADERMAKER_SHADER_H
