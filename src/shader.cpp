//
// Created by angus.goucher on 4/2/2024.
//

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES
#include <string>
#include <fstream>
#include <sstream>
#include <SDL.h>
#include <SDL_opengl.h>
#include <OpenGL/gl.h>

#include "shader.h"

namespace sm {
    Shader::Shader(const std::string &vertex, const std::string &fragment) {
        GLuint vs, fs;
        int32_t length;
        int32_t status;

        vs = glCreateShader(GL_VERTEX_SHADER);
        fs = glCreateShader(GL_FRAGMENT_SHADER);

        // Compile vertex shader
        length = vertex.size();
        const char* vertexCstr = vertex.c_str();
        glShaderSource(vs, 1, (const GLchar **)&vertexCstr, &length);
        glCompileShader(vs);

        glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            GLint infoLogLength;
            glGetShaderiv(vs, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar* infoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(vs, infoLogLength, NULL, infoLog);

            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "%s\n%s", "Failed to compile vertex shader.", infoLog);

            delete[] infoLog;
        }

        // Compile fragment shader
        length = fragment.size();
        const char* fragmentCstr = fragment.c_str();
        glShaderSource(fs, 1, (const GLchar **)&fragmentCstr, &length);
        glCompileShader(fs);

        glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            GLint infoLogLength;
            glGetShaderiv(fs, GL_INFO_LOG_LENGTH, &infoLogLength);

            GLchar* infoLog = new GLchar[infoLogLength + 1];
            glGetShaderInfoLog(fs, infoLogLength, NULL, infoLog);

            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "%s\n%s", "Failed to compile fragment shader.", infoLog);

            delete[] infoLog;
        }

        // Link
        this->program = glCreateProgram();
        glAttachShader(this->program, vs);
        glAttachShader(this->program, fs);
        glLinkProgram(this->program);

    }

    /**
     * Loads a Shader from two files.
     * @param fragmentFile path to the fragment shader code.
     * @param vertexFile path to the vertex shader code.
     * @return a newly constructed Shader object.
     */
    Shader *loadShader(const std::string &vertexFile, const std::string &fragmentFile) {
        std::ifstream fragmentStream(fragmentFile);
        if (!fragmentStream) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Couldn't read from %s", fragmentFile.c_str());
        }
        std::stringstream fragmentBuffer;
        fragmentBuffer << fragmentStream.rdbuf();

        std::ifstream vertexStream(vertexFile);
        if (!vertexStream) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Couldn't read from %s", vertexFile.c_str());
        }
        std::stringstream vertexBuffer;
        vertexBuffer << vertexStream.rdbuf();

        return new Shader(fragmentBuffer.str(), vertexBuffer.str());
    }
} // sm