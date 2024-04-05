//
// Created by angus.goucher on 4/2/2024.
//

#ifndef SHADERMAKER_SHADER_H
#define SHADERMAKER_SHADER_H

#include <string>
#include <glm/glm.hpp>

namespace sm {
	class Shader {
	public:
		Shader(const std::string& vertex, const std::string& fragment);

		void use() const;

        void setInt(const std::string &name, int v) const;
        void setFloat(const std::string &name, float v) const;
        void setVec2(const std::string &name, float x, float y) const;
        void setVec2(const std::string& name, const glm::vec2& v) const;
        void setVec3(const std::string &name, float x, float y, float z) const;
        void setVec3(const std::string& name, const glm::vec3& v) const;
        void setVec4(const std::string &name, float x, float y, float z, float w) const;
        void setVec4(const std::string& name, const glm::vec4& v) const;
        void setMat4(const std::string& name, const glm::mat4& m) const;

	private:
		uint32_t program;
    };

	Shader* loadShader(const std::string& vertexFile, const std::string& fragmentFile);
} // sm

#endif //SHADERMAKER_SHADER_H
