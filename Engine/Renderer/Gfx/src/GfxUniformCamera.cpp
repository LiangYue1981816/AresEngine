#include "GfxHeader.h"


CGfxUniformCamera::CGfxUniformCamera(void)
	: m_bDirty(false)
{
	m_ptrUniformBuffer = GfxRenderer()->NewUniformBuffer(sizeof(m_params));
}

CGfxUniformCamera::~CGfxUniformCamera(void)
{

}

const CGfxUniformCamera::Params& CGfxUniformCamera::GetParams(void) const
{
	return m_params;
}

const CGfxUniformBufferPtr CGfxUniformCamera::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
}

void CGfxUniformCamera::SetScreen(float width, float height)
{
	m_bDirty = true;
	m_params.SetScreen(width, height);
}

void CGfxUniformCamera::SetPerspective(float fovy, float aspect, float zNear, float zFar)
{
	// [-1 1] OpenGL
	// [ 0 1] Vulkan DirectX

	m_bDirty = true;
	m_params.SetPerspective(GfxRenderer()->GetBaseMatrix(), fovy, aspect, zNear, zFar);
}

void CGfxUniformCamera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	// [-1 1] OpenGL
	// [ 0 1] Vulkan DirectX

	m_bDirty = true;
	m_params.SetOrtho(GfxRenderer()->GetBaseMatrix(), left, right, bottom, top, zNear, zFar);
}

void CGfxUniformCamera::SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_bDirty = true;
	m_params.SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CGfxUniformCamera::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_ptrUniformBuffer->BufferData(0, sizeof(m_params), &m_params);
	}
}
