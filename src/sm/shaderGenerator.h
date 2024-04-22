#ifndef SHADERMAKER_SHADER_GENERATOR
#define SHADERMAKER_SHADER_GENERATOR
#include "shader.h"

namespace sm{
	class ShaderGenerator {
	public:
		ShaderGenerator(const std::string& vertex, const std::string& fragment);

		//sm::Shader generateShader(std::string& outputCode);
	private:
		//void sortCode(std::string& outputCode);

		sm::Shader* generatedShader;
		std::string vertCode;
		std::string fragCode;
	};
}
#endif //SHADERMAKER_SHADER_GENERATOR