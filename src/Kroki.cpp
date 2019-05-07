//
// Created by Maximilian Denninger on 2019-05-01.
//

#include "Kroki.h"

bool Kroki::update(){
	if(m_energyLevel > 0 && m_isStillAlive){
		const double maxUpdateAngle = m_maxUpdateAngle * m_speed * m_speed;
		if(isOnWayHome()){
			const auto homeDir = m_homePos - m_pos;
			const double homeDirLen = homeDir.squaredLength();
			if(homeDirLen > 2){
				const double homeDirDist = sqrt(homeDirLen);
				const dPoint2 dir = dPoint2(cos(m_currentAngle), sin(m_currentAngle));
				const double differentAngle = acos(homeDir.dot(dir) / homeDirDist);
				const int sign = -homeDir.turningSignTo(dir);
				const double updateAngle = std::min(maxUpdateAngle, differentAngle);
				m_currentAngle += sign * updateAngle;
				m_pos += dir * (m_speed / m_size);
				m_pos = m_map.clipPointToMap(m_pos);
				m_lastPositions[m_currentPosInLastPositions] = m_pos;
				++m_currentPosInLastPositions;
				if(m_currentPosInLastPositions == m_lastPositions.size()){
					m_lastPositionsBufferFilled = true;
					m_currentPosInLastPositions = 0;
				}
				m_energyLevel -= 0.5 * m_size * m_size * m_size * m_speed * m_speed + m_sense / 60.;
				return true;
			}
		}else{
			const unsigned int closestIdFoodSource = m_map.getClosestFoodSource(m_pos);
			Kroki* closestKroki = getClosestKroki();
			int chase = -1;
			double distKroki = DBL_MAX, distFood = DBL_MAX;
			if(closestKroki != nullptr){
				distKroki = closestKroki->getPose().squaredDist(m_pos);
			}
			if(closestIdFoodSource != UINT_MAX){
				const FoodSource& foodSource = m_map.getFoodSource(closestIdFoodSource);
				const dPoint2 foodSourcePos = foodSource.getPoint();
				distFood = foodSourcePos.squaredDist(m_pos);
			}
			if(closestKroki != nullptr && closestIdFoodSource != UINT_MAX){
				if(distFood < distKroki){
					if(distFood < m_sense * m_sense){
						chase = 0; // chase food
					}
				}else{
					if(distKroki < m_sense * m_sense){
						double sizeDiff = m_size - closestKroki->getSize();
						if(sizeDiff > 0.1 * std::max(m_size, closestKroki->getSize())){
							chase = 1; // interact with kroki
						}else if(distFood < m_sense * m_sense){ // size difference to small
							chase = 0; // chase food
						}
					}
				}
			}else if(closestKroki != nullptr){
				double sizeDiff = m_size - closestKroki->getSize();
				if(sizeDiff > 0.1 * std::max(m_size, closestKroki->getSize())){
					chase = 1; // interact with kroki
				}else if(distFood < m_sense * m_sense){
					chase = 0; // chase food
				}
			}else if(closestIdFoodSource != UINT_MAX){
				if(distFood < m_sense * m_sense){
					chase = 0; // chase food
				}
			}
			const dPoint2 dir = dPoint2(cos(m_currentAngle), sin(m_currentAngle));
			if(chase == 0){ // chase food
				const FoodSource& foodSource = m_map.getFoodSource(closestIdFoodSource);
				const dPoint2 foodSourceDir = foodSource.getPoint() - m_pos;
				const double foodSourceDist = foodSourceDir.norm();
				if(foodSourceDist < 2.5){
					// eat food source
					m_map.removeFoodSourceWithId(closestIdFoodSource);
					m_foodCounter += 5;
					return true;
				}
				const double differentAngle = acos(foodSourceDir.dot(dir) / foodSourceDist);
				const int sign = -foodSourceDir.turningSignTo(dir);
				const double updateAngle = std::min(maxUpdateAngle, differentAngle);
				m_currentAngle += sign * updateAngle;
			}else if(chase == 1){ // interact with kroki
				const dPoint2 krokiDir = closestKroki->getPose() - m_pos;
				const double distKrokiSqrt = sqrt(distKroki);
				const double differentAngle = acos(krokiDir.dot(dir) / distKrokiSqrt);
				int sign = -krokiDir.turningSignTo(dir);
				if(m_size < closestKroki->getSize()){ // smaller -> run away
					sign *= -1;
				}else{ // bigger might eat
					if(distKrokiSqrt < 2.5){
						// eat the other kroki
						closestKroki->kill();
						m_foodCounter += 1;
					}
				}

				const double updateAngle = std::min(maxUpdateAngle, differentAngle);
				m_currentAngle += sign * updateAngle;
			}else{ // random walk
				m_currentAngle += (rand() % 2 * 2 - 1) * M_PI / 45.;  // random 0.5 degree walk
			}
			const dPoint2 newDir = dPoint2(cos(m_currentAngle), sin(m_currentAngle));
			m_pos += newDir * (m_speed / m_size);
			m_pos = m_map.clipPointToMap(m_pos);
			// avoid map borders
			if(m_pos[0] < 10 && acos(newDir.dot(dPoint2(-1, 0))) < M_PI / 2.){
				m_currentAngle += M_PI / 6.0;
			}else if(m_pos[0] > m_map.getSize()[0] - 11 && acos(newDir.dot(dPoint2(1, 0))) < M_PI / 2.){
				m_currentAngle += M_PI / 6.0;
			}
			if(m_pos[1] < 10 && acos(newDir.dot(dPoint2(0, -1))) < M_PI / 2.){
				m_currentAngle += M_PI / 6.0;
			}else if(m_pos[1] > m_map.getSize()[1] - 11 && acos(newDir.dot(dPoint2(0, 1))) < M_PI / 2.){
				m_currentAngle += M_PI / 6.0;
			}

			m_lastPositions[m_currentPosInLastPositions] = m_pos;
			++m_currentPosInLastPositions;
			if(m_currentPosInLastPositions == m_lastPositions.size()){
				m_lastPositionsBufferFilled = true;
				m_currentPosInLastPositions = 0;
			}
			m_energyLevel -= 0.5 * m_size * m_size * m_size * m_speed * m_speed + m_sense / 60.;
			return true;
		}
	}
	return false;
}

Kroki* Kroki::getClosestKroki(){
	double dist = DBL_MAX;
	Kroki* closest = nullptr;
	for(unsigned int i = 0; i < m_population.size(); ++i){
		if(m_id != i && m_population[i].m_isStillAlive){
			double newDist = m_population[i].getPose().squaredDist(m_pos);
			if(newDist < dist){
				dist = newDist;
				closest = &m_population[i];
			}
		}
	}
	return closest;
}

Kroki::Kroki(unsigned long id, Map& map, dPoint2 startPose, double startAngle, double speed, double sense, double size,
		double goHomeEnergy, std::vector<Kroki>* population)
		: m_isStillAlive(true), m_speed(speed), m_sense(sense), m_size(size),
		  m_pos(startPose), m_homePos(startPose), m_currentAngle(startAngle),
		  m_foodCounter(0), m_id(id), m_map(map), m_currentPosInLastPositions(0),
		  m_population(*population){
	m_maxUpdateAngle = M_PI / 10.; // 18.0 degree
	m_lastPositions.resize(40);
	m_lastPositionsBufferFilled = false;
	m_energyLevel = 400.;
	m_goHomeEnergyLevel = goHomeEnergy;
}
