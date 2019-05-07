//
// Created by Maximilian Denninger on 2019-05-05.
//

#include "KrokiContainerInfo.h"

void KrokiContainerInfo::addKroki(const Kroki& kroki, double minSpeed, double maxSpeed, unsigned long globalFrameCounter){
	KrokiInfo info;
	const auto& pos = kroki.getPose();
	glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(pos[0], 0, pos[1]));
	float s = kroki.getSize() * 2;
	glm::mat4 scaleMatrix = glm::scale(glm::vec3(s, s, s));
	glm::mat4 rotationMatrix = glm::mat4(1.0);
	rotationMatrix[0][0] = cos(kroki.getAngle());
	rotationMatrix[2][2] = cos(kroki.getAngle());
	rotationMatrix[0][2] = sin(kroki.getAngle());
	rotationMatrix[2][0] = -sin(kroki.getAngle());

	glm::mat4 rotationMatrix2 = glm::mat4(1.0);
	if(kroki.stillAlive()){
		int currentListSize = m_list.size();
		const double dist = 0.05;
		double moveSpeed = fmod(currentListSize * 17.5123, 1.0) * 0.0001 + 0.00005;
		double randVal = fmod(currentListSize * 13.786156 + globalFrameCounter * moveSpeed, dist * 4) - dist;
		if(randVal > dist){
			randVal = dist - (randVal - dist);
		}
		randVal *= M_PI;
		//rotationMatrix2[0][0] = cos(randVal);
		//rotationMatrix2[1][1] = cos(randVal);
		//rotationMatrix2[1][0] = sin(randVal);
		//rotationMatrix2[0][1] = -sin(randVal);
		//glm::mat4 rotationMatrix2 = glm::mat4(1.0);
		rotationMatrix2[1][1] = cos(randVal);
		rotationMatrix2[2][2] = cos(randVal);
		rotationMatrix2[2][1] = sin(randVal);
		rotationMatrix2[1][2] = -sin(randVal);

		double fac = (kroki.getSpeed() - minSpeed) / (maxSpeed - minSpeed);
		cv::Mat3f rgb_first(cv::Vec3f(0., .0, 1.));
		cv::Mat3f rgb_second(cv::Vec3f(1., 0., 0.));
		cv::Mat3f conv_first, conv_second;
		const auto code = cv::COLOR_RGB2Lab;
		const auto backCode = cv::COLOR_Lab2RGB;
		cv::cvtColor(rgb_first, conv_first, code);
		cv::cvtColor(rgb_second, conv_second, code);

		auto res = cv::Mat3f((conv_second.at<cv::Vec3f>(0,0) - conv_first.at<cv::Vec3f>(0,0)) * fac + conv_first.at<cv::Vec3f>(0,0));
		cv::Mat3f conv_res;
		cv::cvtColor(res, conv_res, backCode);
		auto color = conv_res.at<cv::Vec3f>(0,0);
		//auto color = cv::Vec3f(conv_res.at<cv::Vec3f>(0, 0)) / 255.;
		info.color = glm::vec3(color[0], color[1],
							   color[2]);//glm::vec3(0, 0.5*fac, 1.0*fac)  + glm::vec3( (1-fac) * 1.0, 0., 0.); //kroki.getSpeed()
	}else{
		info.color = glm::vec3(0.3, 0.0, 0.3); //kroki.getSpeed()
	}
	info.modelMat = translationMatrix * rotationMatrix *rotationMatrix2 * scaleMatrix;
	m_list.emplace_back(info);
}
