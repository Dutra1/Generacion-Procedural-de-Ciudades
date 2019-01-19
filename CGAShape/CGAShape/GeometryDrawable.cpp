#include "stdafx.h"
#include "GeometryDrawable.h"

GeometryDrawable::GeometryDrawable() : uniqueElements(0), nextElement(0) {}

void GeometryDrawable::insertPoint(const glm::vec3 & position, const glm::vec3 & normal, const glm::vec2 & coord, float depth) {
	positions.insert(positions.end(), { position.x, position.y, position.z });
	normals.insert(normals.end(), { normal.x, normal.y, normal.z });
	coords.insert(coords.end(), { coord.x, coord.y });
	depths.push_back(depth);
	++uniqueElements;
}

void GeometryDrawable::addElements(const std::vector<GLuint> shapeElements) {
	GLuint startElement = nextElement;
	for (const GLuint & element : shapeElements) {
		GLuint newElement = startElement + element;
		nextElement = std::max(nextElement, newElement + 1);
		elements.push_back(newElement);
	}
}

void GeometryDrawable::createBuffers(GLuint program) {
	//VAO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	//Positions
	glGenBuffers(1, &vboPosition);
	glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), positions.data(), GL_STATIC_DRAW);

	GLint posAttrib = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Depths
	glGenBuffers(1, &vboDepth);
	glBindBuffer(GL_ARRAY_BUFFER, vboDepth);
	glBufferData(GL_ARRAY_BUFFER, depths.size() * sizeof(GLfloat), depths.data(), GL_STATIC_DRAW);

	GLint depthAttrib = glGetAttribLocation(program, "depth");
	glEnableVertexAttribArray(depthAttrib);
	glVertexAttribPointer(depthAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

	//Normals
	glGenBuffers(1, &vboNormal);
	glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(GLfloat), normals.data(), GL_STATIC_DRAW);

	GLint normAttrib = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(normAttrib);
	glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Coords
	glGenBuffers(1, &vboCoords);
	glBindBuffer(GL_ARRAY_BUFFER, vboCoords);
	glBufferData(GL_ARRAY_BUFFER, coords.size() * sizeof(GLfloat), coords.data(), GL_STATIC_DRAW);

	GLint coordAttrib = glGetAttribLocation(program, "texCoords");
	glEnableVertexAttribArray(coordAttrib);
	glVertexAttribPointer(coordAttrib, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//Elements
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLuint), elements.data(), GL_STATIC_DRAW);	
}

void GeometryDrawable::draw() const {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_INT, 0);
}

std::string GeometryDrawable::vertexesToOBJ() const {
	std::ostringstream buffer;
	std::size_t i = 0;
	std::size_t size = positions.size();
	if (size > 0) {
		while (i <= size - 3) {
			buffer << "v " << positions[i] << " " << positions[i + 2] << " " << positions[i + 1] << "\n";
			i += 3;
		}
	}
	return buffer.str();
}

std::string GeometryDrawable::coordsToOBJ() const {
	std::ostringstream buffer;
	std::size_t i = 0;
	std::size_t size = coords.size();
	if (size > 0) {
		while (i <= coords.size() - 2) {
			buffer << "vt " << coords[i] << " " << coords[i + 1] << "\n";
			i += 2;
		}
	}
	return buffer.str();
}

std::string GeometryDrawable::normalsToOBJ() const {
	std::ostringstream buffer;
	std::size_t i = 0;
	std::size_t size = normals.size();
	if (size > 0) {
		while (i <= size - 3) {
			buffer << "vn " << normals[i] << " " << normals[i + 2] << " " << normals[i + 1] << "\n";
			i += 3;
		}
	}
	return buffer.str();
}

std::string GeometryDrawable::facesToOBJ(std::size_t previousAmount, const std::string & material) const {
	std::ostringstream buffer;
	buffer << "usemtl " << material << "\n";

	std::size_t i = 0;
	std::size_t size = elements.size();
	if (size > 0) {
		while (i <= size - 3) {
			buffer << "f ";
			std::size_t first = elements[i] + 1 + previousAmount;
			std::size_t second = elements[i + 1] + 1 + previousAmount;
			std::size_t third = elements[i + 2] + 1 + previousAmount;

			buffer << first << "/" << first << "/" << first << " ";
			buffer << second << "/" << second << "/" << second << " ";
			buffer << third << "/" << third << "/" << third << "\n";
			i += 3;
		}
	}
	return buffer.str();
}

std::size_t GeometryDrawable::getUniqueElements() const {
	return uniqueElements;
}

void GeometryDrawable::quit() {
	glDeleteBuffers(1, &vboPosition);
	glDeleteBuffers(1, &vboNormal);
	glDeleteBuffers(1, &vboDepth);
	glDeleteBuffers(1, &vboCoords);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}