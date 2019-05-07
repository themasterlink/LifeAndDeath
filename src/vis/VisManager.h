//
// Created by Maximilian Denninger on 2019-05-05.
//

#ifndef LIFEANDDEATH_VISMANAGER_H
#define LIFEANDDEATH_VISMANAGER_H


#include "VisObjectShaderContainer.h"

class VisManager {
public:

	VisManager(int width, int height);

	void init();

	void deinit();

	bool run(){
		draw();
		return glfwGetKey(m_window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(m_window) == 0;
	}

	void addVisObjectShaderContainer(VisObjectShaderContainer* visObjectShaderContainer){
		if(visObjectShaderContainer != nullptr){
			m_list.emplace_back(visObjectShaderContainer);
		}
	}

private:
	void draw();

	void computeMatricesFromInputs();


private:
	std::list<VisObjectShaderContainer*> m_list;
	GLFWwindow* m_window{};
	int m_width;
	int m_height;
	GLuint m_vertexArrayID{};
	long m_frameCounter = -1;
	double m_lastTimeStamp;
	glm::vec3 m_lightPos;

	glm::mat4 m_projectionMat;
	glm::mat4 m_cameraMat;

	// mouse position in the last frame
	double m_xOldPos{}, m_yOldPos{};

	float mouseSpeed, speed, verticalAngle, horizontalAngle;
	glm::vec3 position;

};


#endif //LIFEANDDEATH_VISMANAGER_H
