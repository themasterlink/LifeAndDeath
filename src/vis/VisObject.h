//
// Created by Maximilian Denninger on 2019-05-05.
//

#ifndef LIFEANDDEATH_VISOBJECT_H
#define LIFEANDDEATH_VISOBJECT_H

#include <string>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <map>
#include <src/BaseTypes.h>

class VisObject {
public:

	VisObject(const std::string& filePath) : m_filePath(filePath){}

	void init();

	bool loadData(std::vector<glm::vec3>& vertices, std::vector<glm::vec3>& normals);

	void indexVBO(std::vector<glm::vec3>& in_vertices, std::vector<glm::vec3>& in_normals);

	void draw();

	void preDrawSetup(){
		// 1st attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, m_vertexbuffer);
		glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*) 0            // array buffer offset
		);
		// 2rd attribute buffer : normals
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, m_normalbuffer);
		glVertexAttribPointer(
				1,                                // attribute
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*) 0                          // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_elementbuffer);
	}

	void postDrawSetup(){
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
	}

private:

	std::string m_filePath;

	std::vector<unsigned short> m_indices;
	std::vector<glm::vec3> m_indexedVertices;
	std::vector<glm::vec3> m_indexedNormals;

	GLuint m_vertexbuffer{};
	GLuint m_normalbuffer{};
	GLuint m_elementbuffer{};
};


#endif //LIFEANDDEATH_VISOBJECT_H
