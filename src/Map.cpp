//
// Created by Maximilian Denninger on 2019-05-01.
//

#include "Map.h"

void Map::initFoodSources(){
	m_foodSources.clear();
	for(unsigned int i = 0; i < m_amountOfFoodSources; ++i){
		dPoint2 pos;
		for(unsigned int j = 0; j < 2; ++j){
			pos[j] = getRandomPose(j);
			pos[j] = std::max(0., pos[j]);
			pos[j] = std::min(double(m_size[j]), pos[j]);
		}
		pos = clipPointToMap(pos);
		m_foodSources.emplace_back(pos);
	}
}

double Map::getRandomPose(unsigned int axis){
	const double randNr = m_dist(m_gen);
	return randNr * m_size[axis] + m_size[axis]  * 0.5;
}

dPoint2 Map::clipPointToMap(dPoint2 point){
	dPoint2 res;
	res[0] = std::min(std::max(point[0], 0.), double(m_size[0] - 1));
	res[1] = std::min(std::max(point[1], 0.), double(m_size[1] - 1));
	return res;
}

unsigned int Map::getClosestFoodSource(const dPoint2& position) const{
	unsigned int minNr = UINT_MAX;
	double minVal = DBL_MAX;
	for(unsigned int i = 0; i < m_foodSources.size(); ++i){
		const double dist = m_foodSources[i].getPoint().squaredDist(position);
		if(minVal > dist){
			minVal = dist;
			minNr = i;
		}
	}
	return minNr;
}

Map::Map(iPoint2 size, unsigned int amountOfFoodSources) : m_size(size), m_amountOfFoodSources(amountOfFoodSources), m_dist(0., 0.2){
	initFoodSources();
}
