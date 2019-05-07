//
// Created by Maximilian Denninger on 2019-05-06.
//

#ifndef LIFEANDDEATH_FOODVISOBJECTSHADERCONTAINER_H
#define LIFEANDDEATH_FOODVISOBJECTSHADERCONTAINER_H

#include <src/FoodSource.h>
#include "VisObjectShaderContainer.h"

class FoodVisObjectShaderContainer : public VisObjectShaderContainer {

public:
	FoodVisObjectShaderContainer(const std::string& vertexFilePath, const std::string& fragmentFilePath,
								  const std::string& filePathObj) :
			VisObjectShaderContainer(vertexFilePath, fragmentFilePath, filePathObj) {}

	void setWithFoodSources(const std::vector<FoodSource>& food);

protected:
	void innerDraw() override;


private:
	std::vector<glm::mat4> m_modelFoodMatList;




};


#endif //LIFEANDDEATH_FOODVISOBJECTSHADERCONTAINER_H
