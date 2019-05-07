//
// Created by Maximilian Denninger on 2019-05-06.
//

#include "FoodVisObjectShaderContainer.h"

void FoodVisObjectShaderContainer::setWithFoodSources(const std::vector<FoodSource>& food){
	m_modelFoodMatList.clear();
	for(const auto& food_ele: food){
		const auto pos = food_ele.getPoint();
		glm::mat4 translationMatrix = glm::translate(glm::mat4(), glm::vec3(pos[0], 0, pos[1]));
		const double s1 = fmod(pos[0]* 0.1 + pos[1] * 0.1, 2.0) + 1;
		const double s2 = fmod(pos[0]* 0.3 + pos[1] * 0.2, 2.0) + 1;
		const double s3 = fmod(pos[0]* 0.2 + pos[1] * 0.3, 2.0) + 1;
		glm::mat4 scaleMatrix = glm::scale(glm::vec3(s1,s2,s3));
		const double angle = pos[0]* 0.1 + pos[1] * 0.1;
		glm::mat4 rotationMatrix = glm::mat4(1.0); // = glm::rotate(float(kroki.getAngle()), glm::vec3(0., 1, 0));
		if(angle != 0.){
			rotationMatrix[0][0] = cos(angle);
			rotationMatrix[2][2] = cos(angle);
			rotationMatrix[0][2] = sin(angle);
			rotationMatrix[2][0] = -sin(angle);
		}
		const auto mat = translationMatrix * rotationMatrix * scaleMatrix;
		m_modelFoodMatList.emplace_back(mat);
	}
}

void FoodVisObjectShaderContainer::innerDraw(){
	glUniform3f(m_colorID, m_currentColor.r, m_currentColor.g, m_currentColor.b);
	for(unsigned int i = 0; i < m_modelFoodMatList.size(); ++i){
		glUniformMatrix4fv(m_modelMatrixID, 1, GL_FALSE, &m_modelFoodMatList[i][0][0]);
		m_visObject.draw();
	}
}
