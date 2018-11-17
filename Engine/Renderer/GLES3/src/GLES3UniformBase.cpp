#include "GLES3Renderer.h"


CGLES3UniformBase::CGLES3UniformBase(CGLES3UniformManager *pManager, size_t size, bool bDynamic)
	: CGfxUniformBase(size, bDynamic)
	, m_pManager(pManager)

	, m_pUniformBuffer(nullptr)
{
	m_pUniformBuffer = new CGLES3UniformBuffer(size, bDynamic);
}

CGLES3UniformBase::~CGLES3UniformBase(void)
{
	delete m_pUniformBuffer;
}

CGLES3UniformBuffer* CGLES3UniformBase::GetUniformBuffer(void) const
{
	return m_pUniformBuffer;
}
