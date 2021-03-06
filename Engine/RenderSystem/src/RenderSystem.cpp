#include "EngineHeader.h"
#include "RenderHeader.h"
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
	{ INSTANCE_ATTRIBUTE_INDEX, 4, 1, 8,  GFX_DATAFORMAT_SINT, "inInstanceIndex" },
};


CRenderSystem::CRenderSystem(GfxApi api, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
	: m_pRenderer(nullptr)
	, m_pGPUScene(nullptr)
	, m_pEngineUniform(nullptr)
	, m_pInstanceBufferPool(nullptr)

	, m_pPassPreZ(nullptr)
	, m_pPassShadow(nullptr)
	, m_pPassDefault(nullptr)
	, m_pPassForwardLighting(nullptr)
	, m_pPassSSAO(nullptr)
	, m_pPassSSAOBlurHorizontal(nullptr)
	, m_pPassSSAOBlurVertical(nullptr)
	, m_pPassBloomLuminanceThreshold(nullptr)
	, m_pPassBloomBlurHorizontal(nullptr)
	, m_pPassBloomBlurVertical(nullptr)
	, m_pPassBloomBlendAdd(nullptr)
	, m_pPassColorGrading(nullptr)
	, m_pPassFinal(nullptr)
{
	SetVertexAttributes(vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
	SetInstanceAttributes(instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);

	switch ((int)api) {
	case GFX_API_GLES3:
		m_pRenderer = new CGLES3Renderer(hInstance, hWnd, hDC, width, height, format);
		break;

	case GFX_API_VULKAN:
		m_pRenderer = new CVKRenderer(hInstance, hWnd, hDC, width, height, format);
		break;
	}

	m_pGPUScene = new CGPUScene;
	m_pEngineUniform = new CUniformEngine;
	m_pInstanceBufferPool = new CInstanceBufferPool;

	m_ptrComputeCommandBuffer[0] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrComputeCommandBuffer[1] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrComputeCommandBuffer[2] = GfxRenderer()->NewCommandBuffer(0, true);

	m_ptrGraphicCommandBuffer[0] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrGraphicCommandBuffer[1] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrGraphicCommandBuffer[2] = GfxRenderer()->NewCommandBuffer(0, true);

	Settings()->SetValue("RenderSystem.Shadow.Factor", 1.0f);
	Settings()->SetValue("RenderSystem.SSAO.SampleCount", 8.0f);
	Settings()->SetValue("RenderSystem.SSAO.SampleMinRadius", 0.05f);
	Settings()->SetValue("RenderSystem.SSAO.SampleMaxRadius", 1.00f);
	Settings()->SetValue("RenderSystem.SSAO.BlurRange", 1.00f);
	Settings()->SetValue("RenderSystem.Bloom.BlurRange.FirstTime", 2.00f);
	Settings()->SetValue("RenderSystem.Bloom.BlurRange.SecondTime", 1.00f);
}

CRenderSystem::~CRenderSystem(void)
{
	m_ptrComputeCommandBuffer[0].Release();
	m_ptrComputeCommandBuffer[1].Release();
	m_ptrComputeCommandBuffer[2].Release();

	m_ptrGraphicCommandBuffer[0].Release();
	m_ptrGraphicCommandBuffer[1].Release();
	m_ptrGraphicCommandBuffer[2].Release();

	delete m_pInstanceBufferPool;
	delete m_pEngineUniform;
	delete m_pGPUScene;
	delete m_pRenderer;
}

CGPUScene* CRenderSystem::GetGPUScene(void) const
{
	return m_pGPUScene;
}

CUniformEngine* CRenderSystem::GetEngineUniform(void) const
{
	return m_pEngineUniform;
}

void CRenderSystem::CreatePasses(void)
{
	CPassPreZ::Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassShadow::Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassDefault::Create(GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32, GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassForwardLighting::Create(GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32, GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassCopy::Create(GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32);
	CPassSSAO::Create(GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32);
	CPassBlurBox::Create(GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32);
	CPassBlurHorizontal::Create(GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32);
	CPassBlurVertical::Create(GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32);
	CPassBlendAdd::Create(GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32);
	CPassLuminanceThreshold::Create(GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32);
	CPassColorGrading::Create(GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32);
	CPassFinal::Create(GFX_PIXELFORMAT_BGRA8_UNORM_PACK8);

	m_pPassPreZ = new CPassPreZ(this);
	m_pPassShadow = new CPassShadow(this);
	m_pPassDefault = new CPassDefault(this);
	m_pPassForwardLighting = new CPassForwardLighting(this);
	m_pPassSSAO = new CPassSSAO(this);
	m_pPassSSAOBlurHorizontal = new CPassBlurHorizontal(this);
	m_pPassSSAOBlurVertical = new CPassBlurVertical(this);
	m_pPassBloomLuminanceThreshold = new CPassLuminanceThreshold(this);
	m_pPassBloomBlurHorizontal = new CPassBlurHorizontal(this);
	m_pPassBloomBlurVertical = new CPassBlurVertical(this);
	m_pPassBloomBlendAdd = new CPassBlendAdd(this);
	m_pPassColorGrading = new CPassColorGrading(this);
	m_pPassFinal = new CPassFinal(this);
}

void CRenderSystem::DestroyPasses(void)
{
	CPassPreZ::Destroy();
	CPassShadow::Destroy();
	CPassDefault::Destroy();
	CPassForwardLighting::Destroy();
	CPassCopy::Destroy();
	CPassSSAO::Destroy();
	CPassBlurBox::Destroy();
	CPassBlurHorizontal::Destroy();
	CPassBlurVertical::Destroy();
	CPassBlendAdd::Destroy();
	CPassLuminanceThreshold::Destroy();
	CPassColorGrading::Destroy();
	CPassFinal::Destroy();

	delete m_pPassPreZ;
	delete m_pPassShadow;
	delete m_pPassDefault;
	delete m_pPassForwardLighting;
	delete m_pPassSSAO;
	delete m_pPassSSAOBlurHorizontal;
	delete m_pPassSSAOBlurVertical;
	delete m_pPassBloomLuminanceThreshold;
	delete m_pPassBloomBlurHorizontal;
	delete m_pPassBloomBlurVertical;
	delete m_pPassBloomBlendAdd;
	delete m_pPassColorGrading;
	delete m_pPassFinal;
}

void CRenderSystem::CreateRenderTextures(void)
{
	CreateRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR0, GfxRenderer()->GetSwapChain()->GetFrameTexture(0));
	CreateRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR1, GfxRenderer()->GetSwapChain()->GetFrameTexture(1));
	CreateRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR2, GfxRenderer()->GetSwapChain()->GetFrameTexture(2));
	CreateRenderTexture(RENDER_TEXTURE_SHADOW, GFX_PIXELFORMAT_D32_SFLOAT_PACK32, 2048, 2048);
	CreateRenderTexture(RENDER_TEXTURE_FULL_DEPTH, GFX_PIXELFORMAT_D32_SFLOAT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	CreateRenderTexture(RENDER_TEXTURE_FULL_HDR_COLOR0, GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	CreateRenderTexture(RENDER_TEXTURE_FULL_HDR_COLOR1, GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	CreateRenderTexture(RENDER_TEXTURE_QUATER_HDR_COLOR0, GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32, GfxRenderer()->GetSwapChain()->GetWidth() / 4, GfxRenderer()->GetSwapChain()->GetHeight() / 4);
	CreateRenderTexture(RENDER_TEXTURE_QUATER_HDR_COLOR1, GFX_PIXELFORMAT_BGR10A2_UNORM_PACK32, GfxRenderer()->GetSwapChain()->GetWidth() / 4, GfxRenderer()->GetSwapChain()->GetHeight() / 4);
}

void CRenderSystem::DestroyRenderTextures(void)
{
	m_ptrRenderTextures.clear();
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

int CRenderSystem::AddInstance(void)
{
	return m_pGPUScene->AddInstance();
}

void CRenderSystem::RemoveInstance(int index)
{
	m_pGPUScene->RemoveInstance(index);
}

void CRenderSystem::ModifyInstanceData(int index, const InstanceData &data, int indexThread)
{
	m_pGPUScene->ModifyInstanceData(index, data, indexThread);
}

int CRenderSystem::GetDefaultInstanceIndex(void) const
{
	return m_pGPUScene->GetDefaultInstanceIndex();
}

int CRenderSystem::GetPostProcessInstnaceIndex(void) const
{
	return m_pGPUScene->GetPostProcessInstnaceIndex();
}

const InstanceData& CRenderSystem::GetInstanceData(int index)
{
	return m_pGPUScene->GetInstanceData(index);
}

const CGfxMultiInstanceBufferPtr CRenderSystem::GetInstanceBuffer(uint32_t instanceFormat, int instanceBinding)
{
	return m_pInstanceBufferPool->GetInstanceBuffer(instanceFormat, instanceBinding);
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

void CRenderSystem::UpdateCamera(CTaskGraph& taskGraph, CCamera* pCamera, uint32_t mask, bool bComputeLOD)
{
	pCamera->Update(taskGraph, mask, bComputeLOD);
}

void CRenderSystem::UpdateGPUScene(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	m_pGPUScene->Update(taskGraph, ptrCommandBuffer);
}
