#pragma once
#include "GLES3Renderer.h"


class CGLES3UniformManager
{
	friend class CGLES3Renderer;
	friend class CGLES3UniformEngine;
	friend class CGLES3UniformCamera;


private:
	CGLES3UniformManager(void);
	virtual ~CGLES3UniformManager(void);


private:
	CGLES3UniformEngine* CreateUniformEngine(bool bDynamic);
	CGLES3UniformCamera* CreateUniformCamera(bool bDynamic);
	void DestroyUniform(CGLES3UniformBase *pUniform);


private:
	eastl::unordered_map<CGLES3UniformBase*, CGLES3UniformBase*> m_pUniforms;
};
