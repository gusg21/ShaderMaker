//
// Created by Nate Spielman on 4/22/2024.
//
#include "shaderGenerator.h"
#include <string>
#include <fstream>
#include <sstream>
#include <SDL.h>

#include "../external/glad.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace sm {
	ShaderGenerator::ShaderGenerator(const std::string& vertex, const std::string& fragment, const std::string& fileName) {
        templateFile = fragment;
        
        std::ifstream vertexStream(vertex);
        if (!vertexStream) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Couldn't read from %s", vertex.c_str());
        }
        std::stringstream vertexBuffer;
        vertexBuffer << vertexStream.rdbuf();

		std::ifstream fragmentStream(fragment);
        if (!fragmentStream) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Couldn't read from %s", fragment.c_str());
        }
        std::stringstream fragmentBuffer;
        fragmentBuffer << fragmentStream.rdbuf();

        vertCode = vertexBuffer.str();
        fragCode = fragmentBuffer.str();
        fragFile = fileName;

		generatedShader = sm::loadShader(vertex, fragment);
	}

	sm::Shader* ShaderGenerator::generateShader(std::string& outputCode) {
        sortCode(outputCode);

        std::ifstream fragmentStream(fragFile);
        if (!fragmentStream) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Couldn't read from %s", fragFile.c_str());
        }
        std::stringstream fragmentBuffer;
        fragmentBuffer << fragmentStream.rdbuf();

        return new Shader(vertCode, fragmentBuffer.str());
	}

	void ShaderGenerator::sortCode(std::string& outputCode) {
        std::ifstream in(templateFile);
        std::ofstream f(fragFile);
        while (!in.eof())
        {
            std::string line;

            std::getline(in, line);
            if (line == "void main() {") {
                f << line << std::endl;
                f << outputCode << std::endl;
            }
            else {
                f << line << std::endl;
            }
        }
	}
}