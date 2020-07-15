#include "EngineHeader.h"
#include "RenderHeader.h"


CUniformCamera::CUniformCamera(void)
	: m_bDirty(false)
	, m_offset(0)
{
	m_ptrUniformBuffer = GfxRenderer()->NewUniformBuffer(CGfxSwapChain::SWAPCHAIN_FRAME_COUNT * sizeof(m_params));
}

CUniformCamera::~CUniformCamera(void)
{

}

const CUniformCamera::Params& CUniformCamera::GetParams(void) const
{
	return m_params;
}

const CGfxUniformBufferPtr CUniformCamera::GetUniformBuffer(void) const
{
	return m_ptrUniformBuffer;
}

void CUniformCamera::SetScreen(float width, float height)
{
	m_bDirty = true;
	m_params.SetScreen(width, height);
}

void CUniformCamera::SetPerspective(float fovy, float aspect, float zNear, float zFar)
{
	// [-1 1] OpenGL
	// [ 0 1] Vulkan DirectX

	m_bDirty = true;
	m_params.SetPerspective(GfxRenderer()->GetBaseMatrix(), fovy, aspect, zNear, zFar);
}

void CUniformCamera::SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar)
{
	// [-1 1] OpenGL
	// [ 0 1] Vulkan DirectX

	m_bDirty = true;
	m_params.SetOrtho(GfxRenderer()->GetBaseMatrix(), left, right, bottom, top, zNear, zFar);
}

void CUniformCamera::SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_bDirty = true;
	m_params.SetLookat(eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CUniformCamera::Apply(void)
{
	if (m_bDirty) {
		m_bDirty = false;
		m_offset = (GfxRenderer()->GetSwapChain()->GetFrameIndex() % CGfxSwapChain::SWAPCHAIN_FRAME_COUNT) * sizeof(m_params);
		m_ptrUniformBuffer->BufferData(m_offset, sizeof(m_params), &m_params);
	}
}
