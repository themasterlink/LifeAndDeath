//
// Created by Maximilian Denninger on 2019-05-01.
//

#include "Population.h"

std::pair<dPoint2, double> Population::getRandomBorderPose(){
	unsigned int randNr = rand() % 4;
	switch(randNr){
		case 0:
			return std::pair<dPoint2, double>(dPoint2(0., m_map.getRandomPose(1)), 0.);
		case 1:
			return std::pair<dPoint2, double>(dPoint2(m_map.getRandomPose(0), 0.), M_PI*0.5);
		case 2:
			return std::pair<dPoint2, double>(dPoint2(m_map.getSize()[0] - 1., m_map.getRandomPose(1)), -M_PI);
		case 3:
			return std::pair<dPoint2, double>(dPoint2(m_map.getRandomPose(0), m_map.getSize()[1] - 1.), -M_PI *0.5);
		default:
			printWarning("This is not defined!");
			return std::pair<dPoint2, double>(dPoint2(), 0);
	}
}

Population::Population(unsigned int populationSize, Map& map) : m_map(map), m_populationSize(populationSize) {
	for(unsigned int i = 0; i < populationSize; ++i){
		addKrokiToPopulation(nullptr);
	}
}

void Population::addKrokiToPopulation(Kroki* kroki){
	double speed = 1., sense = 30., size = 1., goHomeEnergy = 200.;
	if(kroki != nullptr){
		speed = kroki->getSpeed();
		sense = kroki->getSense();
		size = kroki->getSize();
		goHomeEnergy = kroki->getGoHomeEnergyLevel();
	}
	std::normal_distribution<double> speedDist(speed, speed * 0.1);
	speed = speedDist(m_gen);
	std::normal_distribution<double> senseDist(sense, sense * 0.1);
	sense = senseDist(m_gen);
	std::normal_distribution<double> sizeDist(size, size * 0.1);
	size = std::max(0.1, sizeDist(m_gen));
	std::normal_distribution<double> goHomeEnergyDist(goHomeEnergy, goHomeEnergy * 0.1);
	goHomeEnergy = std::max(0., goHomeEnergyDist(m_gen));
	auto res = getRandomBorderPose();
	res.first = m_map.clipPointToMap(res.first);
	m_population.emplace_back(m_population.size(), m_map, res.first, res.second, speed, sense, size, goHomeEnergy, &m_population);
}

void Population::printStatistic(){
	std::list<double> speeds, sizes, senses;
	for(const auto& kroki : m_population){
		speeds.push_back(kroki.getSpeed());
		sizes.push_back(kroki.getSize());
		senses.push_back(kroki.getSense());
	}
	std::cout << "Population size: " << m_population.size() << std::endl;
	std::cout << "Speed: " << utils::average(speeds) << std::endl;
	std::cout << "Sizes: " << utils::average(sizes) << std::endl;
	std::cout << "Senses: " << utils::average(senses) << std::endl;
	std::cout << "--------------------------" << std::endl;

}

void Population::goToBed(){
	printStatistic();
	std::vector<Kroki> oldKrokis = std::move(m_population);
	m_population = std::vector<Kroki>();
	for(auto& oldKroki : oldKrokis){
		if(oldKroki.stillAlive() && oldKroki.gotHome()){
			for(unsigned int i = 1; i < oldKroki.getFoodCounter(); i++){
				addKrokiToPopulation(&oldKroki);
			}
		}
	}
}

bool Population::update(){
	bool stillEnergy = false;
	for(auto& kroki : m_population){
		if(kroki.update()){
			stillEnergy = true;
		}
	}
	return stillEnergy;
}
