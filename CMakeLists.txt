cmake_minimum_required(VERSION 3.14)
project(LifeAndDeath)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR /usr/local/Cellar/opencv/HEAD-37cbcf0_4/share/OpenCV)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )


find_package(OpenGL REQUIRED)

set(ALL_LIBS ${OPENGL_LIBRARY} glfw GLEW_1130)

# Compile external dependencies
add_subdirectory (external)

include_directories(
    external/glfw-3.1.2/include/
    external/glm-0.9.7.1/
    external/glew-1.13.0/include/
    .
)

add_executable(LifeAndDeath main.cpp src/Kroki.cpp src/Kroki.h src/Map.cpp src/Map.h src/FoodSource.cpp src/FoodSource.h src/Population.cpp src/Population.h src/Visualizer.h src/BaseTypes.cpp src/Visualizer.cpp src/vis/VisObject.cpp src/vis/VisObject.h src/vis/VisObjectShaderContainer.cpp src/vis/VisObjectShaderContainer.h src/vis/VisManager.cpp src/vis/VisManager.h src/vis/KrokiContainerInfo.cpp src/vis/KrokiContainerInfo.h src/vis/KrokiVisObjectShaderContainer.cpp src/vis/KrokiVisObjectShaderContainer.h src/vis/FoodVisObjectShaderContainer.cpp src/vis/FoodVisObjectShaderContainer.h)
target_link_libraries(LifeAndDeath ${OpenCV_LIBS} ${ALL_LIBS})
