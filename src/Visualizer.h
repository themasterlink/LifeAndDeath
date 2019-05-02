//
// Created by Maximilian Denninger on 2019-04-27.
//

#ifndef MOBILE_ROBOTIC_VISUALIZER_H
#define MOBILE_ROBOTIC_VISUALIZER_H


#include "Map.h"
#include "Population.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Visualizer {

public:
	Visualizer(Map& map, Population& population): m_map(map), m_population(population){};

	void run();

	void drawPopulation();

	void drawFood();

	void drawIteration(int id);

private:

	Map& m_map;
	Population& m_population;

	cv::Mat m_currentFrame;

};


#endif //MOBILE_ROBOTIC_VISUALIZER_H
