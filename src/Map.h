//
// Created by Maximilian Denninger on 2019-05-01.
//

#ifndef LIFEANDDEATH_MAP_H
#define LIFEANDDEATH_MAP_H


#include "Point2.h"
#include "FoodSource.h"
#include <vector>
#include <float.h>
#include <random>

class Map {
public:

	Map(iPoint2 size, unsigned int amountOfFoodSources);

	void initFoodSources();

	iPoint2 getSize(){
		return m_size;
	}

	double getRandomPose(unsigned int axis);

	dPoint2 clipPointToMap(dPoint2 point);

	unsigned int getClosestFoodSource(const dPoint2& position) const;

	const FoodSource& getFoodSource(const unsigned int id){
		return m_foodSources[id];
	}

	const std::vector<FoodSource>& getAllFoodSources() const { return m_foodSources; }

	unsigned int getAmountOfFoodSources(){
		return m_foodSources.size();
	}

	void removeFoodSourceWithId(const unsigned int id){
		m_foodSources.erase(m_foodSources.begin() + id);
	}


private:

	std::vector<FoodSource> m_foodSources;

	iPoint2 m_size;

	unsigned int m_amountOfFoodSources;

	std::default_random_engine m_gen;
	std::normal_distribution<double> m_dist; //(0.0,0.33);
};


#endif //LIFEANDDEATH_MAP_H
