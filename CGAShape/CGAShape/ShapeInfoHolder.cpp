#include "stdafx.h"
#include "ShapeInfoHolder.h"

void ShapeInfoHolder::createShapeInfo(const std::string & symbol, Geometry geometry, TextureMapping textureMapping, double textureSize, bool setsTextureOrigin, const std::string & material) {
	ShapeInfo info = ShapeInfo(geometry, textureMapping, textureSize, setsTextureOrigin, material);
	infoMap[symbol] = info;
}

bool ShapeInfoHolder::hasShapeInfo(const std::string & symbol) const {
	return infoMap.count(symbol) > 0;
}

ShapeInfo ShapeInfoHolder::getShapeInfo(const std::string & symbol) const {
	if (hasShapeInfo(symbol)) {
		return infoMap.at(symbol);
	} else {
		return ShapeInfo();
	}
	
}

void ShapeInfoHolder::parseShapeInfos(const std::string & shapeInfoFile) {
	pugi::xml_document doc;
	doc.load_file(shapeInfoFile.c_str());

	auto root = doc.child("shapeInfos");

	for (auto justTextureOrigin = root.child("justTextureOrigin"); justTextureOrigin; justTextureOrigin = justTextureOrigin.next_sibling("justTextureOrigin")) {
		createShapeInfo(justTextureOrigin.attribute("symbol").as_string(), Geometry::NONE, TextureMapping::NONE, 0, true, "");
	}

	for (auto shapeInfo = root.child("shapeInfo"); shapeInfo; shapeInfo = shapeInfo.next_sibling("shapeInfo")) {
		Geometry geometry;
		std::string geometryString = shapeInfo.attribute("geometry").as_string();
		if (geometryString == "CUBE") {
			geometry = Geometry::CUBE;
		} else if (geometryString == "PLANE") {
			geometry = Geometry::PLANE;
		} else if (geometryString == "RAMP") {
			geometry = Geometry::RAMP;
		} else if (geometryString == "TRIANGLE") {
			geometry = Geometry::TRIANGLE;
		} else if (geometryString == "POLYGON") {
			geometry = Geometry::POLYGON;
		} else {
			geometry = Geometry::NONE;
		}

		TextureMapping textureMapping;
		std::string textureMappingString = shapeInfo.attribute("textureMapping").as_string();
		if (textureMappingString == "FULL") {
			textureMapping = TextureMapping::FULL;
		} else if (textureMappingString == "POSITION_BASED") {
			textureMapping = TextureMapping::POSITION_BASED;
		} else {
			textureMapping = TextureMapping::NONE;
		}
		
		createShapeInfo(
			shapeInfo.attribute("symbol").as_string(),
			geometry,
			textureMapping,
			shapeInfo.attribute("textureSize").as_double(),
			shapeInfo.attribute("setsTextureOrigin").as_bool(),
			shapeInfo.attribute("material").as_string()
		);
	}
}