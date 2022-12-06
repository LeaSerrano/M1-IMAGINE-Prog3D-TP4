#include "Mesh.h"
// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>

Mesh::Mesh(): mNumVertices(0), mNumFaces(0), positionsBuffer(0),
normalsBuffer(0), tangentsBuffer(0), uv0Buffer(0), elementsBuffer(0) {

}

Mesh::~Mesh() {
	clear();
}

void Mesh::draw() {
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, positionsBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, normalsBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, tangentsBuffer);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, uv0Buffer);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);// 3 because assimp 
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementsBuffer);
	glDrawElements(GL_TRIANGLES, 3 * mNumFaces, GL_UNSIGNED_INT, (void*)0);
}

void Mesh::clear() {
	if (positionsBuffer != 0) {
		glDeleteBuffers(1, &positionsBuffer);
	}
	if (normalsBuffer != 0) {
		glDeleteBuffers(1, &normalsBuffer);
	}
	if (tangentsBuffer != 0) {
		glDeleteBuffers(1, &tangentsBuffer);
	}
	if (uv0Buffer != 0) {
		glDeleteBuffers(1, &uv0Buffer);
	}
	if (elementsBuffer != 0) {
		glDeleteBuffers(1, &elementsBuffer);
	}
}
