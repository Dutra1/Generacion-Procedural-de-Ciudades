#include "stdafx.h"
#include "Material.h"

Material::Material(GLuint textureId, const std::string & texturePath, const glm::vec4 & color): textureId(textureId), texturePath(texturePath), color(color) {};

GLuint Material::getTextureId() const {
	return textureId;
}

std::string Material::getTexturePath() const {
	return texturePath;
}
glm::vec4 Material::getColor() const {
	return color;
}