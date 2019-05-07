//
// Created by Maximilian Denninger on 2019-05-05.
//

#ifndef LIFEANDDEATH_VISOBJECTSHADERCONTAINER_H
#define LIFEANDDEATH_VISOBJECTSHADERCONTAINER_H

#include <fstream>
#include "VisObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class VisObjectShaderContainer {
public:

	VisObjectShaderContainer(const std::string& vertexFilePath, const std::string& fragmentFilePath,
							 const std::string& filePathObj);

	void loadShaders(const std::string& vertexFilePath, const std::string& fragmentFilePath);

	void init();

	void draw(glm::mat4 projectionMat, glm::mat4 cameraMat, glm::vec3 lightPos);

	void setModelMat(const glm::vec3& pos, double angle, float size);

	void setColor(const glm::vec3& color);

protected:
	virtual void innerDraw(){
		glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, &m_currentModelMat[0][0]);
		glUniform3f(m_colorID, m_currentColor.r, m_currentColor.g, m_currentColor.b);
		m_visObject.draw();
	}

	GLuint m_modelMatrixID{}, m_colorID{};
	VisObject m_visObject;
	glm::vec3 m_currentLightPos, m_currentColor;

private:
	const std::string m_vertexFilePath;
	const std::string m_fragmentFilePath;

	GLuint m_programID;
	GLuint m_viewMatrixID{}, m_viewAndProjectionMatrixID{}, m_lightID{};

	glm::mat4 m_currentModelMat;

	glm::mat4 m_currentViewAndProjectionMat;
	glm::mat4 m_currentCameraMat;
};


#endif //LIFEANDDEATH_VISOBJECTSHADERCONTAINER_H
