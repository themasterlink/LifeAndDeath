//
// Created by Maximilian Denninger on 2019-05-02.
//

#include "BaseTypes.h"

double utils::average(const std::list<double>& list){
	double avg = 0.;
	double counter = 0.;
	for(const auto& ele : list){
		counter += 1;
		double fac = 1.0 / counter;
		avg = fac * ele + (1 - fac) * avg;
	}
	return avg;
}
