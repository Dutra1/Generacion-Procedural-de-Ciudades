#pragma once
#ifndef SHAPE_INFO_HOLDER
#define SHAPE_INFO_HOLDER
#include "ShapeInfo.h"

class ShapeInfoHolder {
private:
	std::map<std::string, ShapeInfo> infoMap;
public:
	void createShapeInfo(const std::string & symbol, Geometry geometry, TextureMapping textureMapping, double textureSize, bool setsTextureOrigin, const std::string & material);
	bool hasShapeInfo(const std::string & symbol) const;
	ShapeInfo getShapeInfo(const std::string & symbol) const;

	void parseShapeInfos(const std::string & shapeInfoFile);
};

#endif
