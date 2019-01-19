#include "stdafx.h"
#include "ShapeInfo.h"

ShapeInfo::ShapeInfo() : ShapeInfo(Geometry::NONE, TextureMapping::NONE, 0, false, "") {}

ShapeInfo::ShapeInfo(Geometry geometry, TextureMapping textureMapping, double textureSize, bool isTextureOrigin, const std::string & material) :
	geometry(geometry), textureMapping(textureMapping), textureSize(textureSize), isTexOrigin(isTextureOrigin), material(material) {}

Geometry ShapeInfo::getGeometry() const {
	return geometry;
}

TextureMapping ShapeInfo::getTextureMapping() const {
	return textureMapping;
}

double ShapeInfo::getTextureSize() const {
	return textureSize;
}

bool ShapeInfo::isTextureOrigin() const {
	return isTexOrigin;
}

std::string ShapeInfo::getMaterial() const {
	return material;
}
