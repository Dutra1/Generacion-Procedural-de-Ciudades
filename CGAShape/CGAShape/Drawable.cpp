#include "stdafx.h"
#include "Drawable.h"
#include "Utilities.h"

void Drawable::recursiveProcess(const ShapeTree & tree, std::size_t depth, const glm::mat4 & transformation, const glm::mat4 & coordinateTransformation, const ShapeInfoHolder & shapeInfoHolder) {
	Shape shape = tree.getShape();
	glm::mat4 treeTransformation = shape.getMatrix();
	glm::mat4 newTransformation = transformation * treeTransformation;
	glm::mat4 newCoordinateTransformation;
	ShapeInfo shapeInfo = shapeInfoHolder.getShapeInfo(shape.getSymbol());

	if (shapeInfo.isTextureOrigin()) {
		newCoordinateTransformation = glm::mat4();
	} else {
		newCoordinateTransformation = coordinateTransformation * treeTransformation;
	}

	if (tree.isLeaf()) {
		Geometry geometry = shapeInfo.getGeometry();
		if (geometry != Geometry::NONE) {
			glm::mat4 scaledTransformation = transformation * glm::scale(treeTransformation, shape.getSize());
			std::string material = shapeInfo.getMaterial();
			GeometryDrawable & GD = GDs[material];

			std::vector<glm::vec4> shapePositions = getPositions(shape, scaledTransformation, geometry);
			std::vector<glm::vec4> shapeNormals = getNormals(shape, newTransformation, geometry);
			std::vector<glm::vec2> shapeCoords = getTextureCoordinates(shape, newCoordinateTransformation, shapeInfo);

			for (std::size_t i = 0; i < shapePositions.size(); ++i) {
				GD.insertPoint(glm::vec3(shapePositions[i]), glm::vec3(shapeNormals[i]), shapeCoords[i], depth);
			}

			std::vector<GLuint> shapeElements = getElements(shape, geometry);
			GD.addElements(shapeElements);
		}
	} else {
		for (std::size_t i = 0; i < tree.getChildrenCount(); ++i) {
			recursiveProcess(tree[i], depth + 1, newTransformation, newCoordinateTransformation, shapeInfoHolder);
		}
	}
}

std::size_t Drawable::getNumberOfElements(const Shape & shape, Geometry geometry) {
	if (geometry == Geometry::CUBE) {
		return 24;
	} else if (geometry == Geometry::PLANE) {
		return 4;
	} else if (geometry == Geometry::RAMP) {
		return 18;
	} else if (geometry == Geometry::TRIANGLE) {
		return 3;
	} else if (geometry == Geometry::POLYGON) {
		return shape.getFloorPointCount() > 3 ? shape.getFloorPointCount() - 1 : 0;	//The last is the same as the first, hence the - 1
	} else {
		return 0;
	}
}

std::vector<GLuint> Drawable::getElements(const Shape & shape, Geometry geometry) {
	std::vector<GLuint> result;
	if (geometry == Geometry::CUBE) {
		result = {
			0, 1, 2, 0, 2, 3,
			4, 5, 6, 4, 6, 7,
			8, 9, 10, 8, 10, 11,
			12, 13, 14, 12, 14, 15,
			16, 17, 18, 16, 18, 19,
			20, 21, 22, 20, 22, 23,
		}; 
	} else if (geometry == Geometry::PLANE) {
		result = {
			0, 1, 2, 0, 2, 3,
		};
	} else if (geometry == Geometry::RAMP) {
		result = {
			0, 1, 2, 0, 2, 3,
			4, 5, 6, 4, 6, 7,
			8, 9, 10, 8, 10, 11,
			12, 13, 14,
			15, 16, 17, 
		};
	} else if (geometry == Geometry::TRIANGLE) {
		result = {
			0, 1, 2,
		};
	} else if (geometry == Geometry::POLYGON) {
		std::size_t count = getNumberOfElements(shape, geometry);

		if (count > 0) {
			std::vector<std::size_t> pointsToAdd(count);
			std::iota(pointsToAdd.begin(), pointsToAdd.end(), 0);	//Filled sequentially

			std::size_t index = 0;
			std::size_t remainingPoints = pointsToAdd.size();
			while (remainingPoints > 3) {
				std::size_t elementBegin, elementMiddle, elementEnd;
				elementBegin = pointsToAdd[index];
				elementMiddle = pointsToAdd[(index + 1) % remainingPoints];
				elementEnd = pointsToAdd[(index + 2) % remainingPoints];

				//Check if it turns left
				glm::vec2 begin, middle, end;
				begin = shape.getFloorPoint(elementBegin);
				middle = shape.getFloorPoint(elementMiddle);
				end = shape.getFloorPoint(elementEnd);
				double orientation = (middle.y - begin.y) * (end.x - middle.x) - (middle.x - begin.x) * (end.y - middle.y);

				if (orientation > 0) {
					index = (index + 1) % remainingPoints;
					continue;
				}

				//Check if no other points are inside the triangle
				bool hasInside = false;
				for (std::size_t otherPointIndex = (index + 3) % remainingPoints; otherPointIndex != index; otherPointIndex = (otherPointIndex + 1) % remainingPoints) {
					glm::vec2 otherPoint = shape.getFloorPoint(pointsToAdd[otherPointIndex]);
					//Using barycentric coordinates
					float alpha = ((middle.y - end.y)*(otherPoint.x - end.x) + (end.x - middle.x)*(otherPoint.y - end.y)) /
						((middle.y - end.y)*(begin.x - end.x) + (end.x - middle.x)*(begin.y - end.y));
					float beta = ((end.y - begin.y)*(otherPoint.x - end.x) + (begin.x - end.x)*(otherPoint.y - end.y)) /
						((middle.y - end.y)*(begin.x - end.x) + (end.x - middle.x)*(begin.y - end.y));
					float gamma = 1.0f - alpha - beta;

					if (alpha > 0 && beta > 0 && gamma > 0) {
						hasInside = true;
						break;
					}
				}

				if (hasInside) {
					index = (index + 1) % remainingPoints;
					continue;
				}

				//Check if no lines intersect the diagonal
				bool intersection = false;
				for (std::size_t otherPointIndex = (index + 2) % remainingPoints; otherPointIndex != index; otherPointIndex = (otherPointIndex + 1) % remainingPoints) {
					glm::vec2 otherBegin = shape.getFloorPoint(pointsToAdd[otherPointIndex]);
					glm::vec2 otherEnd = shape.getFloorPoint(pointsToAdd[(otherPointIndex + 1) % remainingPoints]);

					if (Utilities::doSegmentsIntersect(begin, end, otherBegin, otherEnd)) {
						intersection = true;
						break;
					}
				}

				if (intersection) {
					index = (index + 1) % remainingPoints;
					continue;
				}

				//Passed all the tests, can put triangle
				result.insert(result.end(), { elementBegin, elementMiddle , elementEnd });
				pointsToAdd.erase(pointsToAdd.begin() + ((index + 1) % remainingPoints));
				remainingPoints = pointsToAdd.size();
				index = index % remainingPoints;
			}

			//Insert the last 3
			for (auto it = pointsToAdd.begin(); it != pointsToAdd.end(); ++it) {
				result.push_back(*it);
			}
		}
	}

	bool insideOut = false;
	if (shape.getSize(Axis::X) < 0) {
		insideOut = !insideOut;
	}
	if (shape.getSize(Axis::Y) < 0) {
		insideOut = !insideOut;
	}
	if (shape.getSize(Axis::Z) < 0) {
		insideOut = !insideOut;
	}

	if (insideOut) {
		for (int i = 0; i < result.size() - 2; i += 3) {
			std::swap(result[i + 1], result[i + 2]);
		}
	}

	return result;
}

std::vector<glm::vec4> Drawable::getPositions(const Shape & shape, const glm::mat4 & transformation, Geometry geometry) {
	if (geometry == Geometry::CUBE) {
		glm::vec4 LDF = transformation * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 RDF = transformation * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 RUF = transformation * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		glm::vec4 LUF = transformation * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
		glm::vec4 LDB = transformation * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		glm::vec4 RDB = transformation * glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		glm::vec4 RUB = transformation * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec4 LUB = transformation * glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

		return{
			LDF, RDF, RUF, LUF, //Front
			RDF, RDB, RUB, RUF, //Right
			RDB, LDB, LUB, RUB, //Back
			LDB, LDF, LUF, LUB, //Left
			LUF, RUF, RUB, LUB, //Up
			LDB, RDB, RDF, LDF, //Down
		}; 
	} else if (geometry == Geometry::PLANE) {
		glm::vec4 LDF = transformation * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 RDF = transformation * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 RUF = transformation * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
		glm::vec4 LUF = transformation * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

		return{
			LDF, RDF, RUF, LUF,
		};
	} else if (geometry == Geometry::RAMP) {
		glm::vec4 LDF = transformation * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 RDF = transformation * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 LDB = transformation * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		glm::vec4 RDB = transformation * glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
		glm::vec4 RUB = transformation * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec4 LUB = transformation * glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);

		return{
			LDF, RDF, RUB, LUB, //Ramp
			RDB, LDB, LUB, RUB, //Back
			LDB, RDB, RDF, LDF, //Down
			LDB, LDF, LUB, //Left
			RDF, RDB, RUB, //Right
		};
	} else if (geometry == Geometry::TRIANGLE) {
		glm::vec4 LDF = transformation * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 RDF = transformation * glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		glm::vec4 RUF = transformation * glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

		return{
			LDF, RDF, RUF,
		};
	} else if (geometry == Geometry::POLYGON) {
		std::vector<glm::vec4> result;
		std::size_t floorPointCount  = shape.getFloorPointCount();

		for (std::size_t i = 0; i < getNumberOfElements(shape, geometry); ++i) {
			glm::vec2 point = shape.getFloorPoint(i);
			result.push_back(transformation * glm::vec4(point.x, point.y, 0.0f, 1.0f));
		}
		return result;
	} else {
		return {};
	}
}

std::vector<glm::vec4> Drawable::getNormals(const Shape & shape, const glm::mat4 & transformation, Geometry geometry) {
	if (geometry == Geometry::CUBE) {
		float xValue = shape.getSize(Axis::X) >= 0 ? 1.0f : -1.0f;
		glm::vec4 nX = transformation * glm::vec4(xValue, 0.0f, 0.0f, 0.0f);
		float yValue = shape.getSize(Axis::Y) >= 0 ? 1.0f : -1.0f;
		glm::vec4 nY = transformation * glm::vec4(0.0f, yValue, 0.0f, 0.0f);
		float zValue = shape.getSize(Axis::Z) >= 0 ? 1.0f : -1.0f;
		glm::vec4 nZ = transformation * glm::vec4(0.0f, 0.0f, zValue, 0.0f);

		return{
			-nZ, -nZ, -nZ, -nZ,		//Front
			nX,  nX,  nX,  nX,		//Right
			nZ,  nZ,  nZ,  nZ,		//Back
			-nX, -nX, -nX, -nX,		//Left
			nY,  nY,  nY,  nY,		//Up
			-nY, -nY, -nY, -nY,		//Down
		};
	} else if (geometry == Geometry::PLANE) {
		float zValue = shape.getSize(Axis::Z) >= 0 ? 1.0f : -1.0f;
		glm::vec4 nZ = transformation * glm::vec4(0.0f, 0.0f, zValue, 0.0f);

		return{
			-nZ, -nZ, -nZ, -nZ,
		};
	} else if (geometry == Geometry::RAMP) {
		float xValue = shape.getSize(Axis::X) >= 0 ? 1.0f : -1.0f;
		glm::vec4 nX = transformation * glm::vec4(xValue, 0.0f, 0.0f, 0.0f);
		float yValue = shape.getSize(Axis::Y) >= 0 ? 1.0f : -1.0f;
		glm::vec4 nY = transformation * glm::vec4(0.0f, yValue, 0.0f, 0.0f);
		float zValue = shape.getSize(Axis::Z) >= 0 ? 1.0f : -1.0f;
		glm::vec4 nZ = transformation * glm::vec4(0.0f, 0.0f, zValue, 0.0f);
		glm::vec4 nSlope = transformation * glm::normalize(glm::vec4(0.0f, yValue *shape.getSize(Axis::Z), -zValue * shape.getSize(Axis::Y), 0.0f));

		return{
			nSlope, nSlope, nSlope, nSlope,		//Ramp
			nZ,  nZ,  nZ,  nZ,					//Back
			-nY, -nY, -nY, -nY,					//Down
			-nX, -nX, -nX,						//Left
			nX,  nX,  nX,						//Right
		};
	} else if (geometry == Geometry::TRIANGLE) {
		float zValue = shape.getSize(Axis::Z) >= 0 ? 1.0f : -1.0f;
		glm::vec4 nZ = transformation * glm::vec4(0.0f, 0.0f, zValue, 0.0f);

		return{
			-nZ, -nZ, -nZ,
		};
	} else if (geometry == Geometry::POLYGON) {
		float zValue = shape.getSize(Axis::Z) >= 0 ? 1.0f : -1.0f;
		glm::vec4 nZ = transformation * glm::vec4(0.0f, 0.0f, zValue, 0.0f);

		return std::vector<glm::vec4>(getNumberOfElements(shape, geometry), -nZ);
	} else {
		return{};
	}
}

//TODO moving using only the size is only feasable when child has the same rotation as parent
std::vector<glm::vec2> Drawable::getTextureCoordinates(const Shape & shape, const glm::mat4 & transformation, const ShapeInfo & shapeInfo) {
	Geometry geometry = shapeInfo.getGeometry();
	TextureMapping textureMapping = shapeInfo.getTextureMapping();
	double texMult = 1 / shapeInfo.getTextureSize();
	
	if (geometry == Geometry::CUBE) {
		glm::vec2 OOx, OIx, IOx, IIx, OOy, OIy, IOy, IIy, OOz, OIz, IOz, IIz;
		if (textureMapping == TextureMapping::FULL) {
			OOx = OOy = OOz = glm::vec2(0, 0);
			OIx = OIy = OIz = glm::vec2(0, texMult);
			IOx = IOy = IOz = glm::vec2(texMult, 0);
			IIx = IIy = IIz = glm::vec2(texMult, texMult);
		} else if (textureMapping == TextureMapping::POSITION_BASED) {
			glm::vec4 position = transformation[3];
			glm::vec3 size = shape.getSize();
			glm::vec4 positionX = glm::translate(transformation, glm::vec3(size.x, 0, 0))[3];
			glm::vec4 positionY = glm::translate(transformation, glm::vec3(0, size.y, 0))[3];
			glm::vec4 positionZ = glm::translate(transformation, glm::vec3(0, 0, size.z))[3];

			OOx = glm::vec2(position.z * texMult, position.y * texMult);
			OIx = glm::vec2(position.z * texMult, positionY.y * texMult);
			IOx = glm::vec2(positionZ.z * texMult, position.y * texMult);
			IIx = glm::vec2(positionZ.z * texMult, positionY.y * texMult);

			OOy = glm::vec2(position.x * texMult, position.z * texMult);
			OIy = glm::vec2(position.x * texMult, positionZ.z * texMult);
			IOy = glm::vec2(positionX.x * texMult, position.z * texMult);
			IIy = glm::vec2(positionX.x * texMult, positionZ.z * texMult);

			OOz = glm::vec2(position.x * texMult, position.y * texMult);
			OIz = glm::vec2(position.x * texMult, positionY.y * texMult);
			IOz = glm::vec2(positionX.x * texMult, position.y * texMult);
			IIz = glm::vec2(positionX.x * texMult, positionY.y * texMult);
		}

		return{
			OOz, IOz, IIz, OIz, //Front
			OOx, IOx, IIx, OIx, //Right
			IOz, OOz, OIz, IIz, //Back
			IOx, OOx, OIx, IIx, //Left
			OOy, IOy, IIy, OIy, //Up
			IOy, OOy, OIy, IIy, //Down
		};

	} else if (geometry == Geometry::PLANE) {
		glm::vec2 OO, OI, IO, II;
		if (textureMapping == TextureMapping::FULL) {
			OO = glm::vec2(0, 0);
			OI = glm::vec2(0, texMult);
			IO = glm::vec2(texMult, 0);
			II = glm::vec2(texMult, texMult);
		} else if (textureMapping == TextureMapping::POSITION_BASED) {
			glm::vec4 position = transformation[3];
			glm::vec3 size = shape.getSize();
			glm::vec4 positionX = glm::translate(transformation, glm::vec3(size.x, 0, 0))[3];
			glm::vec4 positionY = glm::translate(transformation, glm::vec3(0, size.y, 0))[3];

			OO = glm::vec2(position.x * texMult, position.y * texMult);
			OI = glm::vec2(position.x * texMult, positionY.y * texMult);
			IO = glm::vec2(positionX.x * texMult, position.y * texMult);
			II = glm::vec2(positionX.x * texMult, positionY.y * texMult);
		}

		return{
			OO, IO, II, OI,
		};

	} else if (geometry == Geometry::RAMP) {
		glm::vec2 OOx, IOx, IIx, OOy, OIy, IOy, IIy, OOz, OIz, IOz, IIz, OOSlope, IOSlope, IISlope, OISlope;
		if (textureMapping == TextureMapping::FULL) {
			OOx = OOy = OOz = OOSlope = glm::vec2(0, 0);
			OIy = OIz = OISlope = glm::vec2(0, texMult);
			IOx = IOy = IOz = IOSlope = glm::vec2(texMult, 0);
			IIx = IIy = IIz = IISlope = glm::vec2(texMult, texMult);
		} else if (textureMapping == TextureMapping::POSITION_BASED) {
			glm::vec4 position = transformation[3];
			glm::vec3 size = shape.getSize();
			glm::vec4 positionX = glm::translate(transformation, glm::vec3(size.x, 0, 0))[3];
			glm::vec4 positionY = glm::translate(transformation, glm::vec3(0, size.y, 0))[3];
			glm::vec4 positionZ = glm::translate(transformation, glm::vec3(0, 0, size.z))[3];
			glm::vec4 positionYZ = glm::translate(transformation, glm::vec3(0, size.y, size.z))[3];

			OOx = glm::vec2(position.z * texMult, position.y * texMult);
			IOx = glm::vec2(positionZ.z * texMult, position.y * texMult);
			IIx = glm::vec2(positionZ.z * texMult, positionY.y * texMult);

			OOy = glm::vec2(position.x * texMult, position.z * texMult);
			OIy = glm::vec2(position.x * texMult, positionZ.z * texMult);
			IOy = glm::vec2(positionX.x * texMult, position.z * texMult);
			IIy = glm::vec2(positionX.x * texMult, positionZ.z * texMult);

			OOz = glm::vec2(position.x * texMult, position.y * texMult);
			OIz = glm::vec2(position.x * texMult, positionY.y * texMult);
			IOz = glm::vec2(positionX.x * texMult, position.y * texMult);
			IIz = glm::vec2(positionX.x * texMult, positionY.y * texMult);

			OOSlope = glm::vec2(position.x * texMult, Utilities::hypotenuse(position.y, position.z) * texMult);
			OISlope = glm::vec2(position.x * texMult, Utilities::hypotenuse(positionYZ.y, positionYZ.z) * texMult);
			IOSlope = glm::vec2(positionX.x * texMult, Utilities::hypotenuse(position.y, position.z) * texMult);
			IISlope = glm::vec2(positionX.x * texMult, Utilities::hypotenuse(positionYZ.y, positionYZ.z) * texMult);
		}

		return{
			OOSlope, IOSlope, IISlope, OISlope, //Slope
			IOz, OOz, OIz, IIz, //Back
			IOy, OOy, OIy, IIy, //Down
			IOx, OOx, IIx, //Left
			OOx, IOx, IIx, //Right
		};

	} else if (geometry == Geometry::TRIANGLE) {
		glm::vec2 OO, IO, II;
		if (textureMapping == TextureMapping::FULL) {
			OO = glm::vec2(0, 0);
			IO = glm::vec2(texMult, 0);
			II = glm::vec2(texMult, texMult);
		} else if (textureMapping == TextureMapping::POSITION_BASED) {
			glm::vec4 position = transformation[3];
			glm::vec3 size = shape.getSize();
			glm::vec4 positionX = glm::translate(transformation, glm::vec3(size.x, 0, 0))[3];
			glm::vec4 positionY = glm::translate(transformation, glm::vec3(0, size.y, 0))[3];

			OO = glm::vec2(position.x * texMult, position.y * texMult);
			IO = glm::vec2(positionX.x * texMult, position.y * texMult);
			II = glm::vec2(positionX.x * texMult, positionY.y * texMult);
		}

		return{
			OO, IO, II,
		};

	} else if (geometry == Geometry::POLYGON) {
		std::vector<glm::vec2> result;
		glm::vec4 position = transformation[3];
		glm::vec3 size = shape.getSize();
		glm::vec4 positionX = glm::translate(transformation, glm::vec3(size.x, 0, 0))[3];
		glm::vec4 positionY = glm::translate(transformation, glm::vec3(0, size.y, 0))[3];
		
		for (std::size_t i = 0; i < getNumberOfElements(shape, geometry); ++i) {
			glm::vec2 point = shape.getFloorPoint(i);
			result.push_back(glm::vec2((position.x + point.x * (positionX.x - position.x)) * texMult, (position.y + point.y * (positionY.y - position.y)) * texMult));
		}

		return result;

	} else {
		return {};
	}
}

void Drawable::addShapeTree(const ShapeTree & tree, const ShapeInfoHolder & shapeInfoHolder) {
	recursiveProcess(tree, 0, glm::mat4(), glm::mat4(), shapeInfoHolder);
}

void Drawable::addStreet(const Street & street) {
	std::size_t count = street.getPointCount();
	for (std::size_t i = 0; i < count - 1; ++i) {
		glm::vec3 begin = street.getPoint(i);
		glm::vec3 end = street.getPoint(i + 1);

		double distance = glm::distance(glm::vec2(begin.x, begin.z), glm::vec2(end.x, end.z));
		float angle = atan2((end.z - begin.z), (end.x - begin.x));
		float verticalAngle = atan2((end.y - begin.y), distance);

		glm::mat4 transformation = glm::translate(begin);
		transformation = glm::rotate(transformation, -angle, glm::vec3(0, 1, 0));
		transformation = glm::rotate(transformation, verticalAngle, glm::vec3(0, 0, 1));

		double streetWidth = street.getWidth();
		double extraLenght = street.getWidth();

		glm::vec3 point1 = glm::vec3(transformation * glm::vec4(-extraLenght, 0, streetWidth, 1));
		glm::vec3 point2 = glm::vec3(transformation * glm::vec4(-extraLenght, 0, -streetWidth, 1));
		glm::vec3 point3 = glm::vec3(transformation * glm::vec4(distance + extraLenght, 0, -streetWidth, 1));
		glm::vec3 point4 = glm::vec3(transformation * glm::vec4(distance + extraLenght, 0, streetWidth, 1));
		glm::vec3 normal = glm::vec3(transformation * glm::vec4(0, 1, 0, 0));

		GDs["Street"].insertPoint(point1, normal, glm::vec2(point1.x / 20, point1.z / 20), 0);
		GDs["Street"].insertPoint(point2, normal, glm::vec2(point2.x / 20, point2.z / 20), 0);
		GDs["Street"].insertPoint(point3, normal, glm::vec2(point3.x / 20, point3.z / 20), 0);
		GDs["Street"].insertPoint(point4, normal, glm::vec2(point4.x / 20, point4.z / 20), 0);

		GDs["Street"].addElements({ 0, 1, 2, 0, 2, 3 });
	}
}

void Drawable::sendToGPU(GLuint program) {
	for (auto it = GDs.begin(); it != GDs.end(); ++it) {
		it->second.createBuffers(program);
	}
}

void Drawable::draw(MaterialHolder & materialHolder, GLuint uniColor) const {
	for (auto it = GDs.begin(); it != GDs.end(); ++it) {
		materialHolder.bindMaterial(it->first, uniColor);
		it->second.draw();
	}
}

std::string Drawable::toOBJ(const std::string & materialFile) const {
	std::string intro, vertexes, coords, normals, faces;
	intro = "mtllib " + materialFile + "\n";
	
	std::size_t vertexAmount = 0;
	for (auto it = GDs.begin(); it != GDs.end(); ++it) {
		const GeometryDrawable & GD = it->second;
		vertexes += GD.vertexesToOBJ();
		coords += GD.coordsToOBJ();
		normals += GD.normalsToOBJ();
		faces += GD.facesToOBJ(vertexAmount, it->first);
		vertexAmount += GD.getUniqueElements();
	}

	return intro + vertexes + coords + normals + faces;
}

void Drawable::quit() {
	for (auto it = GDs.begin(); it != GDs.end(); ++it) {
		it->second.quit();
	}
}