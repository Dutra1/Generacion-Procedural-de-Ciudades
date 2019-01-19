#include "stdafx.h"
#include "MaterialHolder.h"

MaterialHolder::MaterialHolder(): currentMaterial("Invalid") {
	glActiveTexture(GL_TEXTURE0);
}

void MaterialHolder::createMaterial(const std::string & name, const char * texturePath, const glm::vec4 & color) {
	SDL_Surface* surface = IMG_Load(texturePath);
	if (surface == NULL) {
		std::cerr << "Texture load error: " << SDL_GetError() << std::endl;
		return;
	}

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
	materialMap.emplace(name, Material(texture, texturePath, color));

	SDL_FreeSurface(surface);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

bool MaterialHolder::hasMaterial(const std::string & name) const {
	return materialMap.count(name) > 0;
}

Material MaterialHolder::getMaterial(const std::string & name) const {
	return materialMap.at(name);
}

void MaterialHolder::bindMaterial(const std::string & name, GLuint uniColor) {
	if (hasMaterial(name) && currentMaterial != name) {
		Material material = getMaterial(name);
		glBindTexture(GL_TEXTURE_2D, material.getTextureId());
		glm::vec4 color = material.getColor();
		glUniform4f(uniColor, color.r, color.g, color.b, color.a);
	}
}

void MaterialHolder::parseMaterials(const std::string & materialFile) {
	pugi::xml_document doc;
	doc.load_file(materialFile.c_str());

	auto root = doc.child("materials");

	for (auto material = root.child("material"); material; material = material.next_sibling("material")) {
		createMaterial(
			material.attribute("name").as_string(),
			material.attribute("file").as_string(),
			glm::vec4(
				material.attribute("r").as_float(),
				material.attribute("g").as_float(),
				material.attribute("b").as_float(),
				material.attribute("a").as_float()
			)
		);
	}
}

std::string MaterialHolder::toMTL() {
	std::ostringstream buffer;

	for (auto it = materialMap.begin(); it != materialMap.end(); ++it) {
		const Material & material = it->second;
		const std::string & name = it->first;
		std::string texturePath = material.getTexturePath();
		glm::vec4 color = material.getColor();

		buffer << "newmtl " << name << "\n";
		buffer << "Ka " << color.r << " " << color.g << " " << color.b << "\n";
		buffer << "Kd " << color.r << " " << color.g << " " << color.b << "\n";
		buffer << "Ks " << color.r << " " << color.g << " " << color.b << "\n";
		buffer << "d " << color.a << "\n";
		buffer << "map_Ka " << texturePath << "\n";
		buffer << "map_Kd " << texturePath << "\n";
		buffer << "map_Ks " << texturePath << "\n";
	}

	return buffer.str();
}

void MaterialHolder::quit() {
	for (const auto &entry : materialMap) {
		GLuint id = entry.second.getTextureId();
		glDeleteTextures(1, &id);
	}
}