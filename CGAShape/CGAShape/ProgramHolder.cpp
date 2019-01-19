#include "stdafx.h"
#include "ProgramHolder.h"

ProgramHolder::ProgramHolder(): currentProgram(-1) {}

GLuint ProgramHolder::createProgram(const std::string & name, GLuint vertexShader, GLuint fragmentShader) {
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	//Set all attributes
	glBindFragDataLocation(program, 0, "outColor");
	
	glLinkProgram(program);

	GLint isLinked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE) {
		GLint maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> infoLog(maxLength);
		glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

		std::cerr << "Program link error: ";
		for (std::vector<char>::const_iterator i = infoLog.begin(); i != infoLog.end(); ++i) {
			std::cerr << *i;
		}
		std::cerr << std::endl;

		glDeleteProgram(program);
		return -1;
	}

	programMap[name] = program;
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	return program;
}

GLuint ProgramHolder::createProgram(const std::string & name, const ShaderHolder & shaderHolder, const std::string & vertexShader, const std::string & fragmentShader) {
	if (shaderHolder.hasShader(vertexShader) && shaderHolder.hasShader(fragmentShader)) {
		return createProgram(name, shaderHolder.getShader(vertexShader), shaderHolder.getShader(fragmentShader));
	}
}

bool ProgramHolder::hasProgram(const std::string & name) const {
	return programMap.count(name) > 0;
}

GLuint ProgramHolder::getProgram(const std::string & name) const {
	return programMap.at(name);
}

void ProgramHolder::useProgram(const std::string & name) {
	if (hasProgram(name)) {
		GLuint program = getProgram(name);
		if (program != currentProgram) {
			currentProgram = program;
			glUseProgram(program);
		}
	}
}

void ProgramHolder::quit() {
	for (const auto & entry : programMap) {
		glDeleteProgram(entry.second);
	}
}