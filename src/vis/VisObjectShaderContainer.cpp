//
// Created by Maximilian Denninger on 2019-05-05.
//

#include "VisObjectShaderContainer.h"

void VisObjectShaderContainer::loadShaders(const std::string& vertexFilePath, const std::string& fragmentFilePath){
	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertexFilePath, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s!\n", vertexFilePath.c_str());
		getchar();
		return;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragmentFilePath, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertexFilePath.c_str());
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragmentFilePath.c_str());
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	m_programID = glCreateProgram();
	glAttachShader(m_programID, VertexShaderID);
	glAttachShader(m_programID, FragmentShaderID);
	glLinkProgram(m_programID);

	// Check the program
	glGetProgramiv(m_programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(m_programID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDetachShader(m_programID, VertexShaderID);
	glDetachShader(m_programID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);
}

void VisObjectShaderContainer::draw(glm::mat4 projectionMat, glm::mat4 cameraMat, glm::vec3 lightPos){
	glUseProgram(m_programID);
	// make lokal copies to make sure they exist during draw
	m_currentViewAndProjectionMat = projectionMat * cameraMat;
	m_currentCameraMat = cameraMat;
	m_currentLightPos = lightPos;
	glUniformMatrix4fv(m_viewMatrixID, 1, GL_FALSE, &m_currentCameraMat[0][0]);
	glUniformMatrix4fv(m_viewAndProjectionMatrixID, 1, GL_FALSE, &m_currentViewAndProjectionMat[0][0]);

	glUniform3f(m_lightID, m_currentLightPos.x, m_currentLightPos.y, m_currentLightPos.z);

	m_visObject.preDrawSetup();
	innerDraw();
	m_visObject.postDrawSetup();
}


void VisObjectShaderContainer::init(){
	loadShaders(m_vertexFilePath, m_fragmentFilePath);

	m_viewAndProjectionMatrixID = glGetUniformLocation(m_programID, "VP");
	m_viewMatrixID = glGetUniformLocation(m_programID, "V");
	m_modelMatrixID = glGetUniformLocation(m_programID, "M");

	m_lightID = glGetUniformLocation(m_programID, "LightPosition_worldspace");
	m_colorID = glGetUniformLocation(m_programID, "diffuse_color");
	m_visObject.init();
}

void VisObjectShaderContainer::setModelMat(const glm::vec3& pos, double angle, float size){
	glm::mat4 translationMatrix = glm::translate(glm::mat4(), pos);
	glm::mat4 scaleMatrix = glm::scale(glm::vec3(size, size, size));
	glm::mat4 rotationMatrix = glm::mat4(1.0); // = glm::rotate(float(kroki.getAngle()), glm::vec3(0., 1, 0));
	if(angle != 0.){
		rotationMatrix[0][0] = cos(angle);
		rotationMatrix[2][2] = cos(angle);
		rotationMatrix[0][2] = sin(angle);
		rotationMatrix[2][0] = -sin(angle);
	}
	m_currentModelMat = translationMatrix * rotationMatrix * scaleMatrix;
}

void VisObjectShaderContainer::setColor(const glm::vec3& color){
	m_currentColor = color;
}

VisObjectShaderContainer::VisObjectShaderContainer(const std::string& vertexFilePath,
												   const std::string& fragmentFilePath, const std::string& filePathObj)
		:
		m_vertexFilePath(vertexFilePath),
		m_fragmentFilePath(fragmentFilePath),
		m_visObject(filePathObj),
		m_programID(0){
	m_currentColor = glm::vec3(1., 0., 0.);
}
