//
// Created by Maximilian Denninger on 2019-05-01.
//

#ifndef LIFEANDDEATH_POPULATION_H
#define LIFEANDDEATH_POPULATION_H

#include "Kroki.h"
#include "Map.h"
#include "BaseTypes.h"

class Population {
public:

	Population(unsigned int populationSize, Map& map);;

	std::pair<dPoint2, double> getRandomBorderPose();

	void addKrokiToPopulation(Kroki* kroki);

	void printStatistic();

	void goToBed();

	bool update();

	std::vector<Kroki>& getKrokis(){ return m_population; }

private:
	Map& m_map;

	unsigned int m_populationSize;

	std::vector<Kroki> m_population;
	std::default_random_engine m_gen;

};


#endif //LIFEANDDEATH_POPULATION_H
