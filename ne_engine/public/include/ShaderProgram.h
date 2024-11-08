#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram {
private:
	unsigned int programId;

public:
	ShaderProgram(const char* vertex_shader_path, const char* fragment_shader_path);
	void bind();
	void cleanup();

	void setUniform1i(const char* name, int value);
	void setUniform2f(const char* name, const glm::vec2& vec);
	void setUniform3f(const char* name, const glm::vec3& vec);
	void setUniformMat4(const char* name, const glm::mat4& matrix);
};
