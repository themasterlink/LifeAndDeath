//
// Created by Maximilian Denninger on 2019-05-05.
//

#include <zconf.h>
#include "VisManager.h"

VisManager::VisManager(int width, int height) : m_height(height), m_width(width){
	position = glm::vec3( 500/2, 300, 500/2 );
	m_lightPos = glm::vec3(500/2,20,500/2);
	horizontalAngle = 0;
	verticalAngle = -M_PI / 1.999;

	speed = 50.0f; // 3 units / second
	mouseSpeed = 0.005f;
	m_lastTimeStamp = glfwGetTime();
}

void VisManager::init(){
	if(!glfwInit()){
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		exit(1);
	}

	glfwWindowHint(GLFW_SAMPLES, 8); // 4 x antialising
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a m_window and create its OpenGL context
	m_window = glfwCreateWindow(m_width, m_height, "Life and Death", nullptr, nullptr);
	if(m_window == nullptr){
		printError("Failed to open GLFW m_window.");
		getchar();
		glfwTerminate();
		exit(1);
	}
	glfwMakeContextCurrent(m_window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if(glewInit() != GLEW_OK){
		printError("Failed to initialize GLEW");
		getchar();
		glfwTerminate();
		exit(1);
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(m_window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);

	// create VBO
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	m_frameCounter = 0;

}

void VisManager::deinit(){
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

void VisManager::draw(){
	// Measure speed
	double currentTime = glfwGetTime();
	++m_frameCounter;
	if(currentTime - m_lastTimeStamp >= 1.0){
		std::cout << 1000.0 / double(m_frameCounter) << "ms/frame" << std::endl;
		m_frameCounter = 0;
		m_lastTimeStamp += 1.0;
	}
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	computeMatricesFromInputs();
	for(auto& visObject : m_list){
		visObject->draw(m_projectionMat, m_cameraMat, m_lightPos);
	}
	// Swap buffers
	glfwSwapBuffers(m_window);
	glfwPollEvents();
}

void VisManager::computeMatricesFromInputs(){
	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(m_window, &xpos, &ypos);



	// Compute new orientation
	if(m_xOldPos != -1){
		horizontalAngle += mouseSpeed * float(m_xOldPos - xpos);
		verticalAngle += mouseSpeed * float(m_yOldPos - ypos);
	}
	if(xpos < 1 || xpos > m_width - 1 || ypos < 1 || ypos > m_height - 1){
		glfwSetCursorPos(m_window, m_width / 2, m_height /2);
		glfwGetCursorPos(m_window, &xpos, &ypos);
	}
	m_xOldPos = xpos;
	m_yOldPos = ypos;

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
	);

	// Right vector
	glm::vec3 right = glm::vec3(
			sin(horizontalAngle - M_PI/2.0f),
			0,
			cos(horizontalAngle - M_PI/2.0f)
	);

	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( m_window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( m_window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( m_window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( m_window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	float FoV = 90.;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	m_projectionMat = glm::perspective(glm::radians(FoV), m_width / float(m_height), 0.5f, 1000.0f);
	// Camera matrix
	m_cameraMat = glm::lookAt(
			position,           // Camera is here
			position+direction, // and looks here : at the same position, plus "direction"
			up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
