#pragma once
#ifndef MATERIAL
#define MATERIAL

class Material {
private:
	GLuint textureId;
	std::string texturePath;
	glm::vec4 color;
public:
	Material(GLuint textureId, const std::string & texturePath, const glm::vec4 & color);

	GLuint getTextureId() const;
	std::string getTexturePath() const;
	glm::vec4 getColor() const;
};

#endif