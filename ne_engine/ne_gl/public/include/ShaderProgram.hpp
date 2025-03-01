#ifndef SHADERPROGRAM_HPP
#define SHADERPROGRAM_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <sstream>
#include <string>

#include "utils.hpp"

class ShaderProgram {
private:
	unsigned int _programId;

public:

	[[nodiscard]] static Result<ShaderProgram, std::string> create(
		const char* vertex_shader_path,
		const char* fragment_shader_path
	);

	void bind();
	void cleanup();

	void setUniform1i(const char* name, int value);
	void setUniform2f(const char* name, const glm::vec2& vec);
	void setUniform3f(const char* name, const glm::vec3& vec);
	void setUniformMat4(const char* name, const glm::mat4& matrix);

private:
	explicit ShaderProgram(unsigned int programId) : _programId(programId) {};
};

#endif