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

	m_ambient = {0.21, 0.12, 0.05};
	m_diffuse = {0.71, 0.42, 0.18};
	m_specular = {0.39, 0.27, 0.16};
	m_shininess = 0.6;

	m_lightColor = {1.0, 1.0, 1.0};
	m_lightPos = {0.0, -1.0, 0.0};

	//Ex1 chargement de la texture
	//m_texture = loadTexture2DFromFilePath("data/marble.png");

	//Ex2 chargement de la normal map
	m_texture_bump = loadTexture2DFromFilePath("data/bump.png");

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

	// TODO : Add your custom parameters here

	GLint lightColor = getUniform("lightColor");
	glUniform3fv(lightColor, 1, glm::value_ptr(m_lightColor));

	GLint lightPos = getUniform("lightPos");
	glUniform3fv(lightPos, 1, glm::value_ptr(m_lightPos));

	GLint ambient = getUniform("material.ambient");
	glUniform3fv(ambient, 1, glm::value_ptr(m_ambient));

	GLint diffuse = getUniform("material.diffuse");
	glUniform3fv(diffuse, 1, glm::value_ptr(m_diffuse));

	GLint specular = getUniform("material.specular");
	glUniform3fv(specular, 1, glm::value_ptr(m_specular));

	GLint shininess = getUniform("material.shininess");
	glUniform1f(shininess, m_shininess);

	glBindTexture(GL_TEXTURE_2D, m_texture_bump);
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(getUniform("normalTexture"), GL_TEXTURE1);

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
