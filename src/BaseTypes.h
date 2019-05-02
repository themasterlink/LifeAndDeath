//
// Created by Maximilian Denninger on 2019-04-27.
//

#ifndef MOBILE_ROBOTIC_BASETYPES_H
#define MOBILE_ROBOTIC_BASETYPES_H

#include <vector>
#include <list>
#include <string>
#include <iostream>

#include "Point2.h"

#define printWarning(msg) std::cout << "Warning in " << __PRETTY_FUNCTION__ << ":" << __LINE__ << ": " << msg << std::endl;


namespace utils{
	double average(const std::list<double>& list);
}

#endif //MOBILE_ROBOTIC_BASETYPES_H
