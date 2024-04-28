//
// Created by Nate Spielman on 4/22/2024.
//
#ifndef SHADERMAKER_SHADER_GENERATOR_H
#define SHADERMAKER_SHADER_GENERATOR_H
#include "shader.h"
#include <sstream>

namespace sm{
	class ShaderGenerator {
	public:
		ShaderGenerator(const std::string& vertex, const std::string& fragment);

		sm::Shader* generateShader(std::string& outputCode);
		sm::Shader* generatedShader;
		bool hasCode = false;

	private:
		void sortCode(std::string& outputCode);
		std::string templateFile;
		std::stringstream vertSStream;
		std::stringstream fragSStream;
	};
}
#endif //SHADERMAKER_SHADER_GENERATOR_H