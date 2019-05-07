//
// Created by Maximilian Denninger on 2019-05-05.
//

#ifndef LIFEANDDEATH_KROKICONTAINERINFO_H
#define LIFEANDDEATH_KROKICONTAINERINFO_H


#include <src/Kroki.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <opencv2/imgproc.hpp>


class KrokiContainerInfo {
public:

	struct KrokiInfo {
		glm::mat4 modelMat;
		glm::vec3 color;
	};

	using KrokiInfoList = std::list<KrokiInfo>;

	void addKroki(const Kroki& kroki, double minSpeed, double maxSpeed, unsigned long globalFrameCounter);

	KrokiInfoList& getList(){
		return m_list;
	}

	void clear(){
		m_list.clear();
	}


private:

	KrokiInfoList m_list;

};


#endif //LIFEANDDEATH_KROKICONTAINERINFO_H
