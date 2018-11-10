#include "GLES3Renderer.h"
#include "GLES3UniformManager.h"


CGLES3UniformManager::CGLES3UniformManager(void)
{

}

CGLES3UniformManager::~CGLES3UniformManager(void)
{
	for (const auto &itUniform : m_pUniforms) {
		delete itUniform.second;
	}

	m_pUniforms.clear();
}

CGLES3UniformEngine* CGLES3UniformManager::CreateUniformEngine(bool bDynamic)
{
	CGLES3UniformEngine *pUniformEngine = new CGLES3UniformEngine(this, bDynamic);
	m_pUniforms[pUniformEngine] = pUniformEngine;
	return pUniformEngine;
}

CGLES3UniformCamera* CGLES3UniformManager::CreateUniformCamera(bool bDynamic)
{
	CGLES3UniformCamera *pUniformCamera = new CGLES3UniformCamera(this, bDynamic);
	m_pUniforms[pUniformCamera] = pUniformCamera;
	return pUniformCamera;
}

void CGLES3UniformManager::DestroyUniform(CGLES3UniformBase *pUniform)
{
	if (pUniform) {
		m_pUniforms.erase(pUniform);
		delete pUniform;
	}
}
