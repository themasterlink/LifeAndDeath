//
// Created by Maximilian Denninger on 2019-05-01.
//

#ifndef LIFEANDDEATH_FOODSOURCE_H
#define LIFEANDDEATH_FOODSOURCE_H


#include "Point2.h"

class FoodSource {

public:

	FoodSource(const dPoint2& point): m_pos(point){};

	const dPoint2& getPoint() const { return m_pos; }


private:

	dPoint2 m_pos;

};


#endif //LIFEANDDEATH_FOODSOURCE_H
