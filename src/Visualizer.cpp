//
// Created by Maximilian Denninger on 2019-05-02.
//

#include "Visualizer.h"

void Visualizer::run(){
	cv::Mat startImage(m_map.getSize()[0],m_map.getSize()[1], CV_8UC3, cv::Scalar(255,255,255));
	int counter = 0;
	for(unsigned int i = 0; i < 100; ++i){
		bool stillEnergy = true;
		int innerCounter = 0;
		while(m_map.getAmountOfFoodSources() > 5 && stillEnergy){
			if(innerCounter % 8 == 0 and innerCounter > 0){
				startImage.copyTo(m_currentFrame);
				drawPopulation();
				drawFood();

				drawIteration(counter++);
				std::stringstream day;
				day << i;
				cv::imshow("Population Simuation", m_currentFrame);
				char input = (char) cv::waitKey(1);
				if(input == 27){
					break;
				}
			}
			++innerCounter;
			stillEnergy = m_population.update();
		}
		m_population.goToBed();
		m_map.initFoodSources();
	}
}

void Visualizer::drawFood(){
	for(const auto& food: m_map.getAllFoodSources()){
		iPoint2 pos = iPoint2(food.getPoint());
		cv::circle(m_currentFrame, pos.cv(), 2, CV_RGB(0, 255, 0), -1);
	}
}

void Visualizer::drawPopulation(){
	for(const auto& kroki: m_population.getKrokis()){
		iPoint2 pos = iPoint2(kroki.getPose());
		double angle = kroki.getAngle();
		const dPoint2 dir = dPoint2(cos(angle), sin(angle))*2;
		auto color = CV_RGB(255, 0, 0);
		if(!kroki.stillAlive()){
			color = CV_RGB(128, 0, 0);
		}

		cv::circle(m_currentFrame, pos.cv(), 2, color, 2);
		const int foodCounter = kroki.getFoodCounter();
		auto& lastPositions = kroki.getLastPositions();
		unsigned char shade = 100;
		int currentPosInLastPos = kroki.getLastPositionsId() - 8;
		for(unsigned int i = 0; i < foodCounter; ++i){
			if(currentPosInLastPos < 0){
				currentPosInLastPos = lastPositions.size() - 1;
			}
			const auto newPos = lastPositions[currentPosInLastPos];
			cv::circle(m_currentFrame, (newPos + dPoint2(-1, 1)).cv(), 2, CV_RGB(shade, shade, shade), -1);
			cv::circle(m_currentFrame, newPos.cv(), 2, color, -1);
			currentPosInLastPos -= 4;
		}
		cv::circle(m_currentFrame, (pos + dir + dPoint2(-1, 1)).cv(), 2, CV_RGB(shade, shade, shade), -1);
		cv::circle(m_currentFrame, (pos + dir).cv(), 2, CV_RGB(0, 0, 255), -1);
	}
}

void Visualizer::drawIteration(int id){
	std::stringstream stringstream;
	stringstream << id << ", " << m_map.getAllFoodSources().size() << ", " << m_population.getKrokis().size();
	int fontFace = cv::FONT_HERSHEY_SCRIPT_SIMPLEX;
	cv::putText(m_currentFrame, stringstream.str(), iPoint2(5, m_map.getSize()[1] - 4).cv(), fontFace, 0.3, CV_RGB(0, 0, 0));
}

