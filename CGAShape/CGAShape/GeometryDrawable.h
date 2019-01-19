#pragma once
#ifndef GEOMETRY_DRAWABLE
#define GEOMETRY_DRAWABLE

class GeometryDrawable {
private:
	GLuint vao, ebo, vboPosition, vboNormal, vboDepth, vboCoords;
	
	std::vector<GLfloat> positions, normals, depths, coords;
	std::vector<GLuint> elements;
	std::size_t uniqueElements, nextElement;
public:
	GeometryDrawable();

	void insertPoint(const glm::vec3 & position, const glm::vec3 & normal, const glm::vec2 & coord, float depth);
	void addElements(const std::vector<GLuint> shapeElements);

	void createBuffers(GLuint program);
	void draw() const;

	std::string vertexesToOBJ() const;
	std::string coordsToOBJ() const;
	std::string normalsToOBJ() const;
	std::string facesToOBJ(std::size_t previousAmount, const std::string & material) const;
	std::size_t getUniqueElements() const;
	void quit();
};

#endif