#include "stdafx.h"
#include "Map.h"
#include "Utilities.h"
#include "Config.h"

Map::Map() : buildingHeight(Config::buildingHeight) {}

void Map::setBuildingHeight(const ShapeArgument & function) {
	buildingHeight = function;
}

double Map::getBuildingHeight(ShapeParameters) {
	return buildingHeight(ShapePassParameters);
}

void Map::addBuilding(const Building & building) {
	buildings.push_back(building);
}

void Map::growAll(const RulePool & rulepool, double lod) {
	for (auto & building : buildings) {
		building.grow(rulepool, lod);
	}
}

void Map::addStreet(const std::vector<glm::vec2> & coords, double width) {
	Street street = Street(width);
	for (auto it = coords.begin(); it != coords.end(); ++it) {
		glm::vec2 coord = *it;
		street.addPoint(glm::vec3(coord.x, 0, coord.y));
	}
	streets.push_back(street);
}

void Map::parseMap(const std::string & mapFIle, bool rectangleize) {
	//Using doubles, float are too small to handle coordinates accurately
	pugi::xml_document doc;

	doc.load_file(mapFIle.c_str());

	//Get bounds
	auto root = doc.child("osm");
	auto bounds = root.child("bounds");
	double minlat = bounds.attribute("minlat").as_double();
	double minlon = bounds.attribute("minlon").as_double();
	double latMid = (bounds.attribute("minlat").as_double() + bounds.attribute("maxlat").as_double()) / 2;
	double mPerLat = 111132.954f - 559.822f * cos(2 * latMid) + 1.175f * cos(4 * latMid);
	double mPerLong = 111132.954f * cos(latMid);

	//Save Nodes
	std::map<std::size_t, glm::vec2> nodes;
	for (pugi::xml_node node = root.child("node"); node; node = node.next_sibling("node")) {
		std::size_t id = node.attribute("id").as_ullong();
		double latitud = (node.attribute("lat").as_double() - minlat) * mPerLat;
		double longitud = (node.attribute("lon").as_double() - minlon) * mPerLong;

		nodes[id] = glm::vec2(latitud, longitud);
	}

	//Get elements
	for (pugi::xml_node way = root.child("way"); way; way = way.next_sibling("way")) {
		bool isBuilding = false;
		bool isStreet = false;
		bool hasHeight = false;
		double height = false;
		for (pugi::xml_node tag = way.child("tag"); tag; tag = tag.next_sibling("tag")) {
			std::string tagK = tag.attribute("k").value();
			if (tagK == "building") {
				isBuilding = true;
			} else if (tagK == "height") {
				hasHeight = true;
				height = tag.attribute("v").as_double();
			} else if (tagK == "highway") {
				std::string tagV = tag.attribute("v").value();
				if (tagV == "motorway" || tagV == "trunk" || tagV == "primary" || tagV == "secondary" || tagV == "terciary") {
					isStreet = true;
				}
			}

			if (isBuilding && hasHeight && isStreet) {
				break;
			}
		}

		if (isBuilding) {
			std::vector<glm::vec2> coordinates;
			for (pugi::xml_node nd = way.child("nd"); nd; nd = nd.next_sibling("nd")) {
				auto ref = nd.attribute("ref").as_ullong();
				coordinates.push_back(nodes[ref]);
			}
			//Check clockwiseness
			if (Utilities::checkClockwiseness(coordinates)) {
				std::reverse(coordinates.begin(), coordinates.end());
			}

			if (rectangleize) {
				//Find centroid
				glm::vec2 centroid = Utilities::findCentroid(coordinates);
				if (Utilities::isInsidePolygon(coordinates, centroid)) {

					//Find width
					glm::vec2 closestPoint = Utilities::findClosestPoint(coordinates, centroid);
					double distance = Utilities::distance(centroid, closestPoint);
					double halfWidth = distance;

					double angle = atan2((closestPoint.y - centroid.y), (closestPoint.x - centroid.x));
					double drawAngle = atan2((closestPoint.y - centroid.y), -(closestPoint.x - centroid.x)) + PI;

					glm::vec2 left = glm::vec2(centroid.x - halfWidth * cos(angle), centroid.y - halfWidth * sin(angle));
					glm::vec2 right = glm::vec2(centroid.x + halfWidth * cos(angle), centroid.y + halfWidth * sin(angle));

					//Make sure the square fits
					while (halfWidth > distance / sqrt(2) &&
						(Utilities::intersectsSome(coordinates,
							glm::vec2(left.x - halfWidth * sin(angle), left.y + halfWidth * cos(angle)),		//Left lenght
							glm::vec2(left.x + halfWidth * sin(angle), left.y - halfWidth * cos(angle))) ||
							Utilities::intersectsSome(coordinates,
								glm::vec2(right.x - halfWidth * sin(angle), right.y + halfWidth * cos(angle)),		//Right lenght
								glm::vec2(right.x + halfWidth * sin(angle), right.y - halfWidth * cos(angle))))) {
						halfWidth *= 0.95;

						left = glm::vec2(centroid.x - halfWidth * cos(angle), centroid.y - halfWidth * sin(angle));
						right = glm::vec2(centroid.x + halfWidth * cos(angle), centroid.y + halfWidth * sin(angle));
					}

					//Find length
					double halfLength = halfWidth;

					while (!Utilities::intersectsSome(coordinates,
						glm::vec2(centroid.x - halfLength * sin(angle), centroid.y + halfLength * cos(angle)),		//Middle lenght
						glm::vec2(centroid.x + halfLength * sin(angle), centroid.y - halfLength * cos(angle))) &&
						!Utilities::intersectsSome(coordinates,
							glm::vec2(left.x - halfLength * sin(angle), left.y + halfLength * cos(angle)),				//Top width
							glm::vec2(right.x - halfLength * sin(angle), right.y + halfLength * cos(angle))) &&
						!Utilities::intersectsSome(coordinates,
							glm::vec2(left.x + halfLength * sin(angle), left.y - halfLength * cos(angle)),				//Bottom width
							glm::vec2(right.x + halfLength * sin(angle), right.y - halfLength * cos(angle)))) {
						halfLength += halfWidth;
					}

					while (halfLength > halfWidth &&
						(Utilities::intersectsSome(coordinates,
							glm::vec2(centroid.x - halfLength * sin(angle), centroid.y + halfLength * cos(angle)),		//Middle lenght
							glm::vec2(centroid.x + halfLength * sin(angle), centroid.y - halfLength * cos(angle))) ||
							Utilities::intersectsSome(coordinates,
								glm::vec2(left.x - halfLength * sin(angle), left.y + halfLength * cos(angle)),				//Top width
								glm::vec2(right.x - halfLength * sin(angle), right.y + halfLength * cos(angle))) ||
							Utilities::intersectsSome(coordinates,
								glm::vec2(left.x + halfLength * sin(angle), left.y - halfLength * cos(angle)),				//Bottom width
								glm::vec2(right.x + halfLength * sin(angle), right.y - halfLength * cos(angle))))) {
						halfLength -= halfWidth / 10;
					}

					Shape building = Shape();
					building.setSymbol(Config::buildingDefaultSymbol);
					
					static auto defaultRandomEngine = std::default_random_engine();

					building.translate(centroid.x, 0, centroid.y);
					building.rotate(Axis::Y, glm::degrees(drawAngle));
					building.translate(-halfWidth, 0, -halfLength);
					building.resize(2 * halfWidth, hasHeight ? height : getBuildingHeight(building, defaultRandomEngine, {}), 2 * halfLength);
					Building build = Building(building);

					int random = std::uniform_int_distribution<int>(1, 4)(build.getGenerator());
					if (random == 0) {
						building.view(Axis::X, true, Axis::Y, true);
					} else if (random == 1) {
						building.view(Axis::X, false, Axis::Y, true);
					} else if (random == 2) {
						building.view(Axis::Z, true, Axis::Y, true);
					}

					addBuilding(build);
				}

			} else {
				Floorplan floorplan = Floorplan(coordinates);

				Shape building = Shape();
				building.setSymbol(Config::polygonizedBuildingDefaultSymbol);
				building.applyFloorplan(floorplan);

				glm::vec2 & position = floorplan.getPosition();
				building.translate(glm::vec3(position.x,
					0,
					position.y));
				building.resize(glm::vec3(floorplan.getSize().x,
					hasHeight ? height : getBuildingHeight(building, std::default_random_engine(), {}),
					floorplan.getSize().y));
				addBuilding(Building(building));
			}
		} else if (isStreet) {
			std::vector<glm::vec2> coordinates;
			for (pugi::xml_node nd = way.child("nd"); nd; nd = nd.next_sibling("nd")) {
				auto ref = nd.attribute("ref").as_ullong();
				coordinates.push_back(nodes[ref]);
			}
			addStreet(coordinates, Config::streetWidth);
		}
	}
}

void Map::buildDrawable(GLuint program, const ShapeInfoHolder & shapeInfoHolder) {
	drawable = std::make_unique<Drawable>();
	for (auto & building : buildings) {
		drawable->addShapeTree(building.getTree(), shapeInfoHolder);
	}
	for (auto & street : streets) {
		drawable->addStreet(street);
	}
	drawable->sendToGPU(program);
}

std::string Map::toOBJ(const std::string & materialFile) const {
	if (drawable) {
		return drawable->toOBJ(materialFile);
	} else {
		return "";
	}
}

bool Map::prepareDraw(ProgramHolder & programHolder) {
	if (programHolder.hasProgram("singleColor3D")) {
		programHolder.useProgram("singleColor3D");
		return true;
	} else {
		return false;
	}
}

void Map::draw(MaterialHolder & materialHolder, GLuint uniColor) const {
	if (drawable) {
		drawable->draw(materialHolder, uniColor);
	}
}

void Map::quit() {
	if (drawable) {
		drawable->quit();
	}
}
