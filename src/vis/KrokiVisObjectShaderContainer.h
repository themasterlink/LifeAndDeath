//
// Created by Maximilian Denninger on 2019-05-05.
//

#ifndef LIFEANDDEATH_KROKIVISOBJECTSHADERCONTAINER_H
#define LIFEANDDEATH_KROKIVISOBJECTSHADERCONTAINER_H

#include <src/Population.h>
#include "VisObjectShaderContainer.h"
#include "KrokiContainerInfo.h"

class KrokiVisObjectShaderContainer : public VisObjectShaderContainer {
public:

	KrokiVisObjectShaderContainer(const std::string& vertexFilePath, const std::string& fragmentFilePath,
							 const std::string& filePathObj) :
							 VisObjectShaderContainer(vertexFilePath, fragmentFilePath, filePathObj) {}

	void setWithPopulation(Population& pop, unsigned long globalFrameCounter);

protected:

	void innerDraw() override;

private:

	KrokiContainerInfo m_krokiContainerInfo;

};


#endif //LIFEANDDEATH_KROKIVISOBJECTSHADERCONTAINER_H
