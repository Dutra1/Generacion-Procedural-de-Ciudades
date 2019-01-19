#pragma once
#ifndef SHAPE_INFO
#define SHAPE_INFO

enum class Geometry {
	NONE, CUBE, PLANE, RAMP, TRIANGLE, POLYGON
};

enum class TextureMapping {
	NONE, FULL, POSITION_BASED
};

class ShapeInfo {
private:
	Geometry geometry;
	TextureMapping textureMapping;
	double textureSize;
	bool isTexOrigin;
	std::string material;
public:
	ShapeInfo();
	ShapeInfo(Geometry geometry, TextureMapping textureMapping, double textureSize, bool setsTextureOrigin, const std::string & material);

	Geometry getGeometry() const;
	TextureMapping getTextureMapping() const;
	double getTextureSize() const;
	bool isTextureOrigin() const;
	std::string getMaterial() const;
};

#endif