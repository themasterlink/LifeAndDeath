//
// Created by Maximilian Denninger on 2019-04-27.
//

#ifndef MOBILE_ROBOTIC_POINT2_H
#define MOBILE_ROBOTIC_POINT2_H

#include <cmath>
#include <opencv2/core/core.hpp>

template<typename Type>
class Point2 {
public:

	Point2(){
		m_values[0] = m_values[1] = Type(0);
	}

	Point2(Type first, Type second){
		m_values[0] = first;
		m_values[1] = second;
	}

	template<typename Type2>
	Point2(Point2<Type2> point){
		m_values[0] = Type(point[0]);
		m_values[1] = Type(point[1]);
	}

	Type operator[](int pos) const{
		return m_values[pos];
	}

	Type& operator[](int pos){
		return m_values[pos];
	}

	cv::Point2d cv() const{
		return cv::Point2d(m_values[0], m_values[1]);
	}

	double norm() const{
		return std::hypot(m_values[0], m_values[1]);
	}

	double squaredLength() const {
		return m_values[0] * m_values[0] + m_values[1] * m_values[1];
	}

	template<typename Type2>
	double dot(const Point2<Type2>& point2) const {
		return m_values[0] * point2[0] + m_values[1] * point2[1];
	}

	template<typename Type2>
	int turningSignTo(const Point2<Type2> point2) const {
		return m_values[0] * point2[1] - m_values[1] * point2[0] > 0 ? 1 : -1;
	}

	template<typename Type2>
	Point2<Type> operator+(const Point2<Type2>& point) const {
		return Point2<Type>(m_values[0] + point[0], m_values[1] + point[1]);
	}

	template<typename Type2>
	Point2<Type> operator-(const Point2<Type2>& point) const {
		return Point2<Type>(m_values[0] - point[0], m_values[1] - point[1]);
	}

	Point2<Type> operator*(double scalar) const {
		return Point2<Type>(m_values[0] * scalar, m_values[1] * scalar);
	}

	template<typename Type2>
	Point2<Type>& operator+=(const Point2<Type2>& point){
		m_values[0] += point[0];
		m_values[1] += point[1];
		return *this;
	}

	template<typename Type2>
	Point2<Type>& operator=(const Point2<Type2>& point){
		m_values[0] = Type(point[0]);
		m_values[1] = Type(point[1]);
		return *this;
	}

	template<typename Type2>
	double squaredDist(const Point2<Type2>& point2) const {
		const double x = m_values[0] - point2[0];
		const double y = m_values[1] - point2[1];
		return x * x + y * y;
	}


private:
	Type m_values[2];

};

template <typename Type>
std::ostream& operator<<(std::ostream& stream, const Point2<Type>& point){
	stream << point[0] << ", " << point[1];
	return stream;
}

using dPoint2 = Point2<double>;
using iPoint2 = Point2<int>;

#endif //MOBILE_ROBOTIC_POINT2_H
