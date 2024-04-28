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
	ShaderGenerator::ShaderGenerator(const std::string& vertex, const std::string& fragment) {
        //Saves template file name in order to be able to read from the template later
        templateFile = fragment;
        
        //Reads from the vertex file and puts it into a string stream
        std::ifstream vertexStream(vertex);
        if (!vertexStream) {
            SDL_LogCritical(SDL_LOG_CATEGORY_ERROR, "Couldn't read from %s", vertex.c_str());
        }
        vertSStream << vertexStream.rdbuf();

        //Loads placeholder shader
		generatedShader = sm::loadShader(vertex, fragment);
	}

	sm::Shader* ShaderGenerator::generateShaderLit(std::string& outputCode) {
        compileFragCode(outputCode, true);

        return new Shader(vertSStream.str(), fragSStream.str());
	}

    sm::Shader* ShaderGenerator::generateShaderPost(std::string& outputCode) {
        compileFragCode(outputCode, false);

        return new Shader(vertSStream.str(), fragSStream.str());
    }

	void ShaderGenerator::compileFragCode(std::string& outputCode, bool isLit) {
        std::ifstream in(templateFile); //Reads from template file
        fragSStream.str(""); //Clears fragment string stream
        std::ostream& outStream = fragSStream; //sets out stream to fragment stream
        
        if (isLit) {
            //If this shader is lit then we know to not override anything in main
            while (!in.eof())
            {
                std::string line;

                std::getline(in, line);
                //gets line, if the line is main then our generated output code gets placed into main in the frag shader code, 
                //otherwise the next line is simply written to the fragment shader
                if (line == "void main() {") {
                    outStream << line << std::endl;
                    outStream << outputCode << std::endl;
                }
                else {
                    outStream << line << std::endl;
                }
            }
        }
        else if (!isLit) {
            //If this shader is post process instead of lit then we override the default code.
            while (!in.eof())
            {
                std::string line;

                std::getline(in, line);
                //gets line, if the line is main then our generated output code gets placed into main in the frag shader code, 
                //otherwise the next line is simply written to the fragment shader
                if (line == "void main() {") {
                    outStream << line << std::endl;
                    std::getline(in, line);
                    std::getline(in, line);
                    std::getline(in, line);
                    outStream << outputCode << std::endl;
                }
                else {
                    outStream << line << std::endl;
                }
            }
        }
	}
}