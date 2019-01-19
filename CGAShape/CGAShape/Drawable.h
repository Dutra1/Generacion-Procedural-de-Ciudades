#pragma once
#ifndef DRAWABLE
#define DRAWABLE
#include "ShapeTree.h"
#include "Street.h"
#include "MaterialHolder.h"
#include "GeometryDrawable.h"
#include "ShapeInfoHolder.h"

class Drawable {
private:
	std::map<std::string, GeometryDrawable> GDs;

	void recursiveProcess(const ShapeTree & tree, std::size_t depth, const glm::mat4 & transformation, const glm::mat4 & coordinateTransformation, const ShapeInfoHolder & shapeInfoHolder);
	
	std::size_t getNumberOfElements(const Shape & shape, Geometry geometry);
	std::vector<GLuint> getElements(const Shape & shape, Geometry geometry);
	std::vector<glm::vec4> getPositions(const Shape & shape, const glm::mat4 & transformation, Geometry geometry);
	std::vector<glm::vec4> getNormals(const Shape & shape, const glm::mat4 & transformation, Geometry geometry);
	std::vector<glm::vec2> getTextureCoordinates(const Shape & shape, const glm::mat4 & transformation, const ShapeInfo & shapeInfo);

public:
	void addShapeTree(const ShapeTree & tree, const ShapeInfoHolder & shapeInfoHolder);
	void addStreet(const Street & tree);
	void sendToGPU(GLuint program);
	std::string toOBJ(const std::string & materialFile) const;

	void draw(MaterialHolder & materialHolder, GLuint uniColor) const;
	void quit();
};

#endif
