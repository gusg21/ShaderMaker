//
// Created by angus.goucher on 4/2/2024.
//

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES
#include <string>
#include <fstream>
#include <sstream>
#include <SDL_opengl.h>
#include <OpenGL/gl.h>

#include "shader.h"

namespace sm {
    Shader::Shader(const std::string &fragment, const std::string &vertex) {
        GLuint vs, fs, program;
        uint32_t length;

        vs = glCreateShader(GL_VERTEX_SHADER);
        fs = glCreateShader(GL_FRAGMENT_SHADER);

        length = fragment.size();
        // glShaderSource();
    }

    /**
     * Loads a Shader from two files.
     * @param fragmentFile path to the fragment shader code.
     * @param vertexFile path to the vertex shader code.
     * @return a newly constructed Shader object.
     */
    Shader *loadShader(const std::string &fragmentFile, const std::string &vertexFile) {
        std::ifstream fragmentStream(fragmentFile);
        std::stringstream fragmentBuffer;
        fragmentBuffer << fragmentStream.rdbuf();

        std::ifstream vertexStream(vertexFile);
        std::stringstream vertexBuffer;
        vertexBuffer << vertexStream.rdbuf();

        return new Shader(fragmentBuffer.str(), vertexBuffer.str());
    }
} // sm