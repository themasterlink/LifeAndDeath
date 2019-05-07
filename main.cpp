#include <iostream>
#include "src/Map.h"
#include "src/Kroki.h"
#include "src/Population.h"
#include "src/Visualizer.h"

int main(){
	unsigned int size = 500;
	Map map(iPoint2(size, size), 150);

	Population population(1000, map);
	Visualizer vis(map, population);
	vis.run();
	return 0;
}