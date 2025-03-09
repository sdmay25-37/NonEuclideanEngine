#include <iostream>
#include <fstream>
#include "ShaderProgram.hpp"
#include "glad/glad.h"

Result<std::string, Error<std::string>> fileToString(const char* filePath);

Result<ShaderProgram, std::string> ShaderProgram::create(const char* vertexShaderPath, const char* fragmentShaderPath) {
	using Result = Result<ShaderProgram, std::string>;

	// Read vertex shader into string
	auto vertexShaderStringResult = fileToString(vertexShaderPath);
	if(vertexShaderStringResult.is_error()) {
		return Result::Error(vertexShaderStringResult.error().value);
	}
	std::string vertexShaderString = vertexShaderStringResult.ok();

	// Read fragment shader into string
	auto fragmentShaderStringResult = fileToString(fragmentShaderPath);
	if(fragmentShaderStringResult.is_error()) {
		return Result::Error(fragmentShaderStringResult.error().value);
	}
	std::string fragmentShaderString = fragmentShaderStringResult.ok();

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

		std::stringstream ss;
		ss << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << '\n';
		return Result::Error(ss.str());
	}

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	// check compile result
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);

		std::stringstream ss;
		ss << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << '\n';
		return Result::Error(ss.str());
	}

	unsigned int programId = glCreateProgram();
	glAttachShader(programId, vertexShader);
	glAttachShader(programId, fragmentShader);
	glLinkProgram(programId);

	// check link result
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(programId, 512, nullptr, infoLog);

		std::stringstream ss;
		ss << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << '\n';
		return Result::Error(ss.str());
	}

	// shaders can be deleted after linking
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return Result::Ok(ShaderProgram(programId));
}

void ShaderProgram::bind() {
	glUseProgram(_programId);
}

void ShaderProgram::cleanup() {
	glDeleteProgram(_programId);
}

void ShaderProgram::setUniform1i(const char *name, int value) {
	glUniform1i(glGetUniformLocation(_programId, name), value);
}

void ShaderProgram::setUniform2f(const char *name, const glm::vec2 &vec) {
	glUniform2f(glGetUniformLocation(_programId, name), vec.x, vec.y);
}

void ShaderProgram::setUniform3f(const char *name, const glm::vec3& vec) {
	glUniform3f(glGetUniformLocation(_programId, name), vec.x, vec.y, vec.z);
}

void ShaderProgram::setUniformMat4(const char *name, const glm::mat4 &matrix) {
	glUniformMatrix4fv(glGetUniformLocation(_programId, name), 1, GL_FALSE, glm::value_ptr(matrix));
}

Result<std::string, Error<std::string>> fileToString(const char* filePath) {
	using Result = Result<std::string, Error<std::string>>;

	std::ifstream file(filePath, std::ios::binary | std::ios::ate);

	if(!file.is_open()) {
		std::stringstream ss;
		ss << "Failed to open file: " << filePath << '\n';
		return Result::Error(ss.str());
	}

	size_t fileSize = file.tellg();
	file.seekg(0, std::ios::beg); // return to beginning

	std::string buffer(fileSize, '\0');

	if(!file.read(&buffer[0], fileSize)) {
		std::stringstream ss;
		ss << "Failed to read file: " << filePath << '\n';
		return Result::Error(ss.str());
	}

	file.close();
	return Result::Ok(buffer);
}
