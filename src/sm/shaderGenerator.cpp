#include "shaderGenerator.h"

namespace sm {
	ShaderGenerator::ShaderGenerator(const std::string& vertex, const std::string& fragment) {
		vertCode = vertex;
		fragCode = fragment;
		generatedShader = sm::loadShader(vertex, fragment);
	}
}