#pragma once
#ifndef PROGRAM_HOLDER
#define PROGRAM_HOLDER
#include "ShaderHolder.h"

class ProgramHolder {
private:
	std::map<std::string, GLuint> programMap;
	GLuint currentProgram;
public:
	ProgramHolder();
	GLuint createProgram(const std::string & name, GLuint vertexShader, GLuint fragmentShader);
	GLuint createProgram(const std::string & name, const ShaderHolder & shaderHolder, const std::string & vertexShader, const std::string & fragmentShader);
	bool hasProgram(const std::string & name) const;
	GLuint getProgram(const std::string & name) const;
	void useProgram(const std::string & name);
	void quit();
};

#endif