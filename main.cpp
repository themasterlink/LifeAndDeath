#include <iostream>
#include "src/Map.h"
#include "src/Kroki.h"
#include "src/Population.h"
#include "src/Visualizer.h"

int main(){
	unsigned int size = 500;
	Map map(iPoint2(size, size), 200);
	Population population(50, map);
	Visualizer vis(map, population);
	vis.run();
	std::cout << "Hello, World!" << std::endl;
	return 0;
}