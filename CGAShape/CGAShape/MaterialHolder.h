#pragma once
#ifndef MATERIAL_HOLDER
#define MATERIAL_HOLDER
#include "Material.h"

class MaterialHolder {
private:
	std::map<std::string, Material> materialMap;
	std::string currentMaterial;
public:
	MaterialHolder();
	void createMaterial(const std::string & name, const char * texturePath, const glm::vec4 & color);
	bool hasMaterial(const std::string & name) const;
	Material getMaterial(const std::string & name) const;
	void bindMaterial(const std::string & name, GLuint uniColor);

	void parseMaterials(const std::string & materialFile);
	std::string toMTL();

	void quit();
};

#endif