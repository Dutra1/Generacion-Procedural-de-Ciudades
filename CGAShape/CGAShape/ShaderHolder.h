#pragma once
#ifndef SHADER_HOLDER
#define SHADER_HOLDER

class ShaderHolder {
private:
	std::map<std::string, GLuint> shaderMap;
	GLuint compileShader(const std::string & name, const char * path, GLenum type);
public:
	GLuint compileVertexShader(const std::string & name, const char * path);
	GLuint compileFragmentShader(const std::string & name, const char * path);
	bool hasShader(const std::string & name) const;
	GLuint getShader(const std::string & name) const;
	void quit();
};

#endif
