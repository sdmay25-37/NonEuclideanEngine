#include <iostream>
#include <fstream>
#include "ShaderProgram.h"
#include "glad/glad.h"

std::string fileToString(const char* filePath);

ShaderProgram::ShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath) {

	std::string vertexShaderString = fileToString(vertexShaderPath);
	std::string fragmentShaderString = fileToString(fragmentShaderPath);

	const char* vertexShaderSource = vertexShaderString.c_str();
	const char* fragmentShaderSource = fragmentShaderString.c_str();

	int success;
	char infoLog[512];

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	// check compile result
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return;
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// check compile result
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return;
	}

	programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);
	glLinkProgram(programId);

	// check link result
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(programId, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return;
	}

	// shaders can be deleted after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void ShaderProgram::bind() {
	glUseProgram(programId);
}

void ShaderProgram::cleanup() {
	glDeleteProgram(programId);
}

void ShaderProgram::setUniform1i(const char *name, int value) {
	glUniform1i(glGetUniformLocation(programId, name), value);
}

void ShaderProgram::setUniform2f(const char *name, const glm::vec2 &vec) {
	glUniform2f(glGetUniformLocation(programId, name), vec.x, vec.y);
}

void ShaderProgram::setUniform3f(const char *name, const glm::vec3& vec) {
	glUniform3f(glGetUniformLocation(programId, name), vec.x, vec.y, vec.z);
}

void ShaderProgram::setUniformMat4(const char *name, const glm::mat4 &matrix) {
	glUniformMatrix4fv(glGetUniformLocation(programId, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

std::string fileToString(const char* filePath) {
	std::ifstream file(filePath, std::ios::binary | std::ios::ate);

	if(!file.is_open()) {
		std::cerr << "Failed to open file: " << filePath << std::endl;
		return nullptr;
	}

	size_t fileSize = file.tellg();
	file.seekg(0, std::ios::beg); // return to beginning

	std::string buffer(fileSize, '\0');

	if(!file.read(&buffer[0], fileSize)) {
		std::cerr << "Failed to read file: " << filePath << std::endl;
		return nullptr;
	}

	file.close();
	return buffer;
}
