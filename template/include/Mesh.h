#ifndef MESH_H_DEFINED
#define MESH_H_DEFINED

#include <GL/glew.h>

#include <glm/glm.hpp>

struct Mesh {
	glm::vec3 center;// use to lookAt specific mesh
	// counts
	GLint mNumVertices;
	GLint mNumFaces;
	// array buffers
	GLuint positionsBuffer;
	GLuint normalsBuffer;
	GLuint tangentsBuffer;
	GLuint uv0Buffer;
	// elementBuffer
	GLuint elementsBuffer;

	Mesh();
	~Mesh();

	void clear();
	void draw();
};


#endif
