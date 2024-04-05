//
// Created by angus.goucher on 4/2/2024.
//

#ifndef SHADERMAKER_SHADER_H
#define SHADERMAKER_SHADER_H

#include <string>

namespace sm {
	class Shader {
	public:
		Shader(const std::string& vertex, const std::string& fragment);

		void use();

	private:
		uint32_t program;
	};

	Shader* loadShader(const std::string& vertexFile, const std::string& fragmentFile);
} // sm

#endif //SHADERMAKER_SHADER_H
