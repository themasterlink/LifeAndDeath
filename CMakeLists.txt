cmake_minimum_required(VERSION 3.14)
project(LifeAndDeath)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR /usr/local/Cellar/opencv/HEAD-37cbcf0_4/share/OpenCV)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(LifeAndDeath main.cpp src/Kroki.cpp src/Kroki.h src/Map.cpp src/Map.h src/FoodSource.cpp src/FoodSource.h src/Population.cpp src/Population.h src/Visualizer.h src/BaseTypes.cpp src/Visualizer.cpp)
target_link_libraries(LifeAndDeath ${OpenCV_LIBS} )
