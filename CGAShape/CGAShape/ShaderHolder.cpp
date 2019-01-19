#include "stdafx.h"
#include "ShaderHolder.h"

GLuint ShaderHolder::compileShader(const std::string & name, const char * source, GLenum type) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE) {
		GLint maxLength = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);

		std::cerr << "Shader compile error: ";
		for (std::vector<char>::const_iterator i = errorLog.begin(); i != errorLog.end(); ++i) {
			std::cerr << *i;
		}
		std::cerr << std::endl;

		glDeleteShader(shader);
		return -1;
	}

	shaderMap[name] = shader;
	return shader;
}

GLuint ShaderHolder::compileVertexShader(const std::string & name, const char * path) {
	return compileShader(name, path, GL_VERTEX_SHADER);
}

GLuint ShaderHolder::compileFragmentShader(const std::string & name, const char * path) {
	return compileShader(name, path, GL_FRAGMENT_SHADER);
}

bool ShaderHolder::hasShader(const std::string & name) const {
	return shaderMap.count(name) > 0;
}

GLuint ShaderHolder::getShader(const std::string & name) const {
	return shaderMap.at(name);
}

void ShaderHolder::quit() {
	for (const auto &entry : shaderMap) {
		glDeleteShader(entry.second);
	}
}