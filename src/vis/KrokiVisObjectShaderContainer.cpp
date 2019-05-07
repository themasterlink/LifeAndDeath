//
// Created by Maximilian Denninger on 2019-05-05.
//

#include "KrokiVisObjectShaderContainer.h"

void KrokiVisObjectShaderContainer::innerDraw(){
	for(const auto& krokiInfo : m_krokiContainerInfo.getList()){
		glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, &krokiInfo.modelMat[0][0]);
		glUniform3f(m_colorID, krokiInfo.color.r, krokiInfo.color.g, krokiInfo.color.b);
		m_visObject.draw();
	}
}

void KrokiVisObjectShaderContainer::setWithPopulation(Population& pop, unsigned long globalFrameCounter){
	m_krokiContainerInfo.clear();
	double minSpeed = DBL_MAX;
	double maxSpeed = DBL_MIN;
	for(const auto& kroki : pop.getKrokis()){
		if(kroki.getSpeed() < minSpeed){
			minSpeed = kroki.getSpeed();
		}
		if(kroki.getSpeed() > maxSpeed){
			maxSpeed = kroki.getSpeed();
		}
	}
	for(const auto& kroki : pop.getKrokis()){
		m_krokiContainerInfo.addKroki(kroki, minSpeed, maxSpeed, globalFrameCounter);
	}
}
