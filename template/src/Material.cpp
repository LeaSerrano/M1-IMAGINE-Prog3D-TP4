// Local includes
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
// GLM includes
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
// OPENGL includes
#include <GL/glew.h>
#include <GL/glut.h>

Material::~Material() {
	if (m_program != 0) {
		glDeleteProgram(m_program);
	}
}

void Material::init() {
	// TODO : Change shader by your
	m_program = load_shaders("shaders/unlit/vertex.glsl", "shaders/unlit/fragment.glsl");
	check();
	// TODO : set initial parameters
	m_color = {1.0, 1.0, 1.0, 1.0};

	m_diffuse = {1.0f, 1.0f, 1.0f};
	m_ambient = {1.0, 0.0, 0.0};
	m_specular = {0.5, 0.5, 0.5};
	m_shininess = 128;

	m_light = {1, 0, 0};

	//m_texture = loadTexture2DFromFilePath("data/marble.png");
	//m_texture_bump = loadTexture2DFromFilePath("data/bump.png");
}

void Material::clear() {
	// nothing to clear
	// TODO: Add the texture or buffer you want to destroy for your material
}

void Material::bind() {
	check();
	glUseProgram(m_program);
	internalBind();
}

void Material::internalBind() {
	// bind parameters
	GLint color = getUniform("color");
	glUniform4fv(color, 1, glm::value_ptr(m_color));
	if (m_texture != 0) {
		glBindTexture(GL_TEXTURE_2D, m_texture);
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(getUniform("colorTexture"), GL_TEXTURE0);
	}

	/*glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_ambient);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_shininess);*/

	GLint lightPos = getUniform("lightPos");
	glUniform4fv(lightPos, 1, glm::value_ptr(m_light));

	// TODO : Add your custom parameters here
}

void Material::setMatrices(glm::mat4& projectionMatrix, glm::mat4& viewMatrix, glm::mat4& modelMatrix)
{
	check();
	glUniformMatrix4fv(getUniform("projection"), 1, false, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(getUniform("view"), 1, false, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(getUniform("model"), 1, false, glm::value_ptr(modelMatrix));
}

GLint Material::getAttribute(const std::string& in_attributeName) {
	check();
	return glGetAttribLocation(m_program, in_attributeName.c_str());
}

GLint Material::getUniform(const std::string& in_uniformName) {
	check();
	return glGetUniformLocation(m_program, in_uniformName.c_str());
}
