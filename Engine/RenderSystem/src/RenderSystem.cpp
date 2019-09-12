#include "EngineHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


static const ATTRIBUTE vertexAttributes[VERTEX_ATTRIBUTE_COUNT] = {
	{ VERTEX_ATTRIBUTE_POSITION,  4, 3, 0, GFX_DATAFORMAT_VEC3_SFLOAT, "inPosition"  },
	{ VERTEX_ATTRIBUTE_NORMAL,    4, 3, 1, GFX_DATAFORMAT_VEC3_SFLOAT, "inNormal"    },
	{ VERTEX_ATTRIBUTE_BINORMAL,  4, 3, 2, GFX_DATAFORMAT_VEC3_SFLOAT, "inBinormal"  },
	{ VERTEX_ATTRIBUTE_COLOR,     4, 3, 3, GFX_DATAFORMAT_VEC3_SFLOAT, "inColor"     },
	{ VERTEX_ATTRIBUTE_TEXCOORD0, 4, 2, 4, GFX_DATAFORMAT_VEC2_SFLOAT, "inTexcoord0" },
	{ VERTEX_ATTRIBUTE_TEXCOORD1, 4, 2, 5, GFX_DATAFORMAT_VEC2_SFLOAT, "inTexcoord1" },
};

static const ATTRIBUTE instanceAttributes[INSTANCE_ATTRIBUTE_COUNT] = {
	// Transform
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0, 4, 4, 8,  GFX_DATAFORMAT_VEC4_SFLOAT, "inInstanceTransformMatrixCol0" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1, 4, 4, 9,  GFX_DATAFORMAT_VEC4_SFLOAT, "inInstanceTransformMatrixCol1" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2, 4, 4, 10, GFX_DATAFORMAT_VEC4_SFLOAT, "inInstanceTransformMatrixCol2" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3, 4, 4, 11, GFX_DATAFORMAT_VEC4_SFLOAT, "inInstanceTransformMatrixCol3" },

	// PointLight
	{ INSTANCE_ATTRIBUTE_POINTLIGHT_COLOR,      4, 4, 12, GFX_DATAFORMAT_VEC4_SFLOAT, "inInstancePointLightColor"       },
	{ INSTANCE_ATTRIBUTE_POINTLIGHT_ATTENUATION,4, 4, 13, GFX_DATAFORMAT_VEC4_SFLOAT, "inInstancePointLightAttenuation" },
};


CRenderSystem::CRenderSystem(GfxApi api, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
	: m_pRenderer(nullptr)
	, m_pEngineUniform(nullptr)

	, m_pPassPreZ(nullptr)
	, m_pPassSSAO(nullptr)
	, m_pPassShadow(nullptr)
	, m_pPassDefault(nullptr)
	, m_pPassForwardLighting(nullptr)
	, m_pPassColorGrading(nullptr)
	, m_pPassFinal(nullptr)
{
	switch ((int)api) {
	case GFX_API_GLES3:
		m_pRenderer = new CGLES3Renderer(hInstance, hWnd, hDC, width, height, format);
		m_pEngineUniform = new CGfxUniformEngine;
		break;

	case GFX_API_VULKAN:
		m_pRenderer = new CVKRenderer(hInstance, hWnd, hDC, width, height, format);
		m_pEngineUniform = new CGfxUniformEngine;
		break;
	}

	m_ptrCommandBuffer[0] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrCommandBuffer[1] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrCommandBuffer[2] = GfxRenderer()->NewCommandBuffer(0, true);

	SetVertexAttributes(vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
	SetInstanceAttributes(instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);

	CreateRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR0, GfxRenderer()->GetSwapChain()->GetFrameTexture(0));
	CreateRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR1, GfxRenderer()->GetSwapChain()->GetFrameTexture(1));
	CreateRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR2, GfxRenderer()->GetSwapChain()->GetFrameTexture(2));
	CreateRenderTexture(RENDER_TEXTURE_SHADOW, GFX_PIXELFORMAT_D32_SFLOAT_PACK32, 2048, 2048);
	CreateRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_DEPTH, GFX_PIXELFORMAT_D32_SFLOAT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	CreateRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_COLOR, GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	CreateRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_SSAO, GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	CreateRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_FINAL, GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());

	CreateRenderPass();
}

CRenderSystem::~CRenderSystem(void)
{
	DestroyRenderPass();
	DestroyRenderTexture();

	m_ptrCommandBuffer[0].Release();
	m_ptrCommandBuffer[1].Release();
	m_ptrCommandBuffer[2].Release();

	delete m_pEngineUniform;
	delete m_pRenderer;
}

CGfxUniformEngine* CRenderSystem::GetEngineUniform(void) const
{
	return m_pEngineUniform;
}

void CRenderSystem::CreateRenderTexture(uint32_t name, CGfxRenderTexturePtr ptrRenderTexture)
{
	m_ptrRenderTextures[name] = ptrRenderTexture;
}

void CRenderSystem::CreateRenderTexture(uint32_t name, GfxPixelFormat format, int width, int height, int samples, bool bTransient)
{
	m_ptrRenderTextures[name] = GfxRenderer()->NewRenderTexture(name);
	m_ptrRenderTextures[name]->Create(format, width, height, samples, bTransient);
}

void CRenderSystem::DestroyRenderTexture(void)
{
	m_ptrRenderTextures.clear();
}

CGfxRenderTexturePtr CRenderSystem::GetRenderTexture(uint32_t name) const
{
	const auto& itRenderTexture = m_ptrRenderTextures.find(name);

	if (itRenderTexture != m_ptrRenderTextures.end()) {
		return itRenderTexture->second;
	}
	else {
		return nullptr;
	}
}

CPassPreZ* CRenderSystem::GetPassPreZ(void) const
{
	return m_pPassPreZ;
}

CPassSSAO* CRenderSystem::GetPassSSAO(void) const
{
	return m_pPassSSAO;
}

CPassShadow* CRenderSystem::GetPassShadow(void) const
{
	return m_pPassShadow;
}

CPassDefault* CRenderSystem::GetPassDefault(void) const
{
	return m_pPassDefault;
}

CPassForwardLighting* CRenderSystem::GetPassForwardLighting(void) const
{
	return m_pPassForwardLighting;
}

CPassColorGrading* CRenderSystem::GetPassColorGrading(void) const
{
	return m_pPassColorGrading;
}

CPassFinal* CRenderSystem::GetPassFinal(void) const
{
	return m_pPassFinal;
}

void CRenderSystem::CreateRenderPass(void)
{
	CPassPreZ::Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassSSAO::Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8);
	CPassShadow::Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassDefault::Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassForwardLighting::Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8, GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassColorGrading::Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8);
	CPassFinal::Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8);

	m_pPassPreZ = new CPassPreZ(this);
	m_pPassSSAO = new CPassSSAO(this);
	m_pPassShadow = new CPassShadow(this);
	m_pPassDefault = new CPassDefault(this);
	m_pPassForwardLighting = new CPassForwardLighting(this);
	m_pPassColorGrading = new CPassColorGrading(this);
	m_pPassFinal = new CPassFinal(this);
}

void CRenderSystem::DestroyRenderPass(void)
{
	CPassPreZ::Destroy();
	CPassSSAO::Destroy();
	CPassShadow::Destroy();
	CPassDefault::Destroy();
	CPassForwardLighting::Destroy();
	CPassColorGrading::Destroy();
	CPassFinal::Destroy();

	delete m_pPassPreZ;
	delete m_pPassSSAO;
	delete m_pPassShadow;
	delete m_pPassDefault;
	delete m_pPassForwardLighting;
	delete m_pPassColorGrading;
	delete m_pPassFinal;
}

void CRenderSystem::SetTime(float t, float dt)
{
	m_pEngineUniform->SetTime(t, dt);
}

void CRenderSystem::SetEnvLightFactor(float factor)
{
	m_pEngineUniform->SetEnvLightFactor(factor);
}

void CRenderSystem::SetAmbientLightFactor(float factor)
{
	m_pEngineUniform->SetAmbientLightFactor(factor);
}

void CRenderSystem::SetMainPointLightFactor(float factor)
{
	m_pEngineUniform->SetMainPointLightFactor(factor);
}

void CRenderSystem::SetMainDirectLightFactor(float factor)
{
	m_pEngineUniform->SetMainDirectLightFactor(factor);
}

void CRenderSystem::SetAmbientLightSH(float shRed[9], float shGreen[9], float shBlue[9])
{
	m_pEngineUniform->SetAmbientLightSH(shRed, shGreen, shBlue);
}

void CRenderSystem::SetAmbientLightRotation(float angle, float axisx, float axisy, float axisz)
{
	m_pEngineUniform->SetAmbientLightRotation(angle, axisx, axisy, axisz);
}

void CRenderSystem::SetAmbientLightDirection(float dirx, float diry, float dirz)
{
	m_pEngineUniform->SetAmbientLightDirection(dirx, diry, dirz);
}

void CRenderSystem::SetMainPointLightColor(float red, float green, float blue)
{
	m_pEngineUniform->SetMainPointLightColor(red, green, blue);
}

void CRenderSystem::SetMainPointLightPosition(float posx, float posy, float posz, float radius)
{
	m_pEngineUniform->SetMainPointLightPosition(posx, posy, posz, radius);
}

void CRenderSystem::SetMainPointLightAttenuation(float linear, float square, float constant)
{
	m_pEngineUniform->SetMainPointLightAttenuation(linear, square, constant);
}

void CRenderSystem::SetMainDirectLightColor(float red, float green, float blue)
{
	m_pEngineUniform->SetMainDirectLightColor(red, green, blue);
}

void CRenderSystem::SetMainDirectLightDirection(float dirx, float diry, float dirz)
{
	m_pEngineUniform->SetMainDirectLightDirection(dirx, diry, dirz);
}

void CRenderSystem::SetMainFogColor(float red, float green, float blue)
{
	m_pEngineUniform->SetMainFogColor(red, green, blue);
}

void CRenderSystem::SetMainFogHeightDensity(float startHeight, float endHeight, float density)
{
	m_pEngineUniform->SetMainFogHeightDensity(startHeight, endHeight, density);
}

void CRenderSystem::SetMainFogDistanceDensity(float startDistance, float endDistance, float density)
{
	m_pEngineUniform->SetMainFogDistanceDensity(startDistance, endDistance, density);
}

void CRenderSystem::SetMainShadowOrtho(int indexLevel, float left, float right, float bottom, float top, float zNear, float zFar)
{
	m_pEngineUniform->SetMainShadowOrtho(indexLevel, left, right, bottom, top, zNear, zFar);
}

void CRenderSystem::SetMainShadowLookat(int indexLevel, float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz)
{
	m_pEngineUniform->SetMainShadowLookat(indexLevel, eyex, eyey, eyez, centerx, centery, centerz, upx, upy, upz);
}

void CRenderSystem::Update(CTaskGraph& taskGraph, CCamera* pCamera) const
{
	pCamera->Update(taskGraph);
}

void CRenderSystem::RenderDefault(CTaskGraph& taskGraph, CCamera* pCamera, bool bPresent) const
{
	const CGfxSemaphore* pWaitSemaphore = GfxRenderer()->GetSwapChain()->GetAcquireSemaphore();
	const CGfxCommandBufferPtr ptrCommandBuffer = m_ptrCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];

	GfxRenderer()->AcquireNextFrame();
	{
		ptrCommandBuffer->Clearup();
		GfxRenderer()->BeginRecord(ptrCommandBuffer);
		{
			m_pPassPreZ->SetCamera(pCamera);
			m_pPassPreZ->SetOutputTexture(GetRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_DEPTH));
			m_pPassPreZ->Render(taskGraph, ptrCommandBuffer);

			m_pPassDefault->SetCamera(pCamera);
			m_pPassDefault->SetOutputTexture(GetRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_COLOR), GetRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_DEPTH));
			m_pPassDefault->Render(taskGraph, ptrCommandBuffer);

			m_pPassFinal->SetCamera(pCamera);
			m_pPassFinal->SetInputTexture(GetRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_FINAL));
			m_pPassFinal->SetOutputTexture(0, GetRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR0));
			m_pPassFinal->SetOutputTexture(1, GetRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR1));
			m_pPassFinal->SetOutputTexture(2, GetRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR2));
			m_pPassFinal->Render(taskGraph, ptrCommandBuffer, GfxRenderer()->GetSwapChain()->GetFrameIndex(), bPresent);
		}
		GfxRenderer()->EndRecord(ptrCommandBuffer);
	}
	GfxRenderer()->Submit(ptrCommandBuffer, pWaitSemaphore);
	GfxRenderer()->Present(ptrCommandBuffer->GetSemaphore());
}

void CRenderSystem::RenderForwardLighting(CTaskGraph& taskGraph, CCamera* pCamera, bool bShadow, bool bPresent) const
{
	const CGfxSemaphore* pWaitSemaphore = GfxRenderer()->GetSwapChain()->GetAcquireSemaphore();
	const CGfxCommandBufferPtr ptrCommandBuffer = m_ptrCommandBuffer[GfxRenderer()->GetSwapChain()->GetFrameIndex()];

	GfxRenderer()->AcquireNextFrame();
	{
		ptrCommandBuffer->Clearup();
		GfxRenderer()->BeginRecord(ptrCommandBuffer);
		{
			m_pPassPreZ->SetCamera(pCamera);
			m_pPassPreZ->SetOutputTexture(GetRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_DEPTH));
			m_pPassPreZ->Render(taskGraph, ptrCommandBuffer);

			m_pPassSSAO->SetCamera(pCamera);
			m_pPassSSAO->SetInputTexture(GetRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_DEPTH));
			m_pPassSSAO->SetOutputTexture(GetRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_SSAO));
			m_pPassSSAO->Render(taskGraph, ptrCommandBuffer);

			m_pPassShadow->SetCamera(pCamera);
			m_pPassShadow->SetOutputTexture(GetRenderTexture(RENDER_TEXTURE_SHADOW));
			m_pPassShadow->Render(taskGraph, ptrCommandBuffer);

			m_pPassForwardLighting->SetCamera(pCamera);
			m_pPassForwardLighting->SetInputTexture(GetRenderTexture(RENDER_TEXTURE_SHADOW), GetRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_SSAO));
			m_pPassForwardLighting->SetOutputTexture(GetRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_COLOR), GetRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_DEPTH));
			m_pPassForwardLighting->Render(taskGraph, ptrCommandBuffer);

			m_pPassColorGrading->SetCamera(pCamera);
			m_pPassColorGrading->SetInputTexture(GetRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_COLOR));
			m_pPassColorGrading->SetOutputTexture(GetRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_FINAL));
			m_pPassColorGrading->Render(taskGraph, ptrCommandBuffer);

			m_pPassFinal->SetCamera(pCamera);
			m_pPassFinal->SetInputTexture(GetRenderTexture(RENDER_TEXTURE_FRAMEBUFFER_FINAL));
			m_pPassFinal->SetOutputTexture(0, GetRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR0));
			m_pPassFinal->SetOutputTexture(1, GetRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR1));
			m_pPassFinal->SetOutputTexture(2, GetRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR2));
			m_pPassFinal->Render(taskGraph, ptrCommandBuffer, GfxRenderer()->GetSwapChain()->GetFrameIndex(), bPresent);
		}
		GfxRenderer()->EndRecord(ptrCommandBuffer);
	}
	GfxRenderer()->Submit(ptrCommandBuffer, pWaitSemaphore);
	GfxRenderer()->Present(ptrCommandBuffer->GetSemaphore());
}
