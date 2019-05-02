//
// Created by Maximilian Denninger on 2019-05-01.
//

#ifndef LIFEANDDEATH_KROKI_H
#define LIFEANDDEATH_KROKI_H

#include <climits>
#include "Point2.h"
#include "Map.h"
#include "BaseTypes.h"

class Kroki {
public:

	Kroki(unsigned long id, Map& map, dPoint2 startPose, double startAngle,
			double speed, double sense, double size, std::vector<Kroki>* m_population);

	bool update();

	const std::vector<dPoint2>& getLastPositions() const {
		return m_lastPositions;
	}

	unsigned int getLastPositionsId() const {
		return m_currentPosInLastPositions;
	}

	dPoint2 getPose() const {
		return m_pos;
	}

	unsigned int getFoodCounter() const {
		return m_foodCounter;
	}

	double getAngle() const {
		return m_currentAngle;
	}

	double getSize() const {
		return m_size;
	}

	double getSpeed() const {
		return m_speed;
	}

	double getSense() const {
		return m_sense;
	}

	void kill() {
		m_isStillAlive = false;
	}

	bool stillAlive() const {
		return m_isStillAlive;
	}

	Kroki* getClosestKroki();

private:
	bool m_isStillAlive;

	double m_speed;
	double m_sense;
	double m_size;

	dPoint2 m_pos;
	double m_currentAngle;

	int m_foodCounter;

	unsigned long m_id;

	Map& m_map;

	double m_maxUpdateAngle;

	std::vector<dPoint2> m_lastPositions;
	unsigned int m_currentPosInLastPositions;
	bool m_lastPositionsBufferFilled;

	double m_energyLevel;

	std::vector<Kroki>& m_population;
};


#endif //LIFEANDDEATH_KROKI_H
