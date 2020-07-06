#include "EngineHeader.h"
#include "RenderHeader.h"
#include "VKRenderer.h"
#include "GLES3Renderer.h"


static const ATTRIBUTE vertexAttributes[VERTEX_ATTRIBUTE_COUNT] = {
	{ VERTEX_ATTRIBUTE_POSITION,  0, 3, GFX_DATA_FLOAT32, GFX_PIXELFORMAT_RGB32_SFLOAT_PACK32, FALSE, "inPosition"  },
	{ VERTEX_ATTRIBUTE_NORMAL,    1, 4, GFX_DATA_SINT8,   GFX_PIXELFORMAT_RGBA8_SNORM_PACK8,   TRUE,  "inNormal"    },
	{ VERTEX_ATTRIBUTE_BINORMAL,  2, 4, GFX_DATA_SINT8,   GFX_PIXELFORMAT_RGBA8_SNORM_PACK8,   TRUE,  "inBinormal"  },
	{ VERTEX_ATTRIBUTE_COLOR,     3, 4, GFX_DATA_UINT8,   GFX_PIXELFORMAT_RGBA8_UNORM_PACK8,   TRUE,  "inColor"     },
	{ VERTEX_ATTRIBUTE_TEXCOORD0, 4, 2, GFX_DATA_FLOAT32, GFX_PIXELFORMAT_RG32_SFLOAT_PACK32,  FALSE, "inTexcoord0" },
	{ VERTEX_ATTRIBUTE_TEXCOORD1, 5, 2, GFX_DATA_FLOAT32, GFX_PIXELFORMAT_RG32_SFLOAT_PACK32,  FALSE, "inTexcoord1" },
};

static const ATTRIBUTE instanceAttributes[INSTANCE_ATTRIBUTE_COUNT] = {
	{ INSTANCE_ATTRIBUTE_INDEX,   8, 1, GFX_DATA_SINT32,  GFX_PIXELFORMAT_R32_SINT_PACK32,     FALSE, "inInstanceIndex" },
};


CRenderSystem::CRenderSystem(void)
	: m_pRenderer(nullptr)
	, m_pEngineUniform(nullptr)
	, m_pInstanceBufferPool(nullptr)

	, m_pGPUScene(nullptr)
	, m_pGPUCluster(nullptr)
	, m_pGPUClusterCull(nullptr)
	, m_pGPUEyeHistogram(nullptr)

	, m_pPassPreZ(nullptr)
	, m_pPassShadow(nullptr)
	, m_pPassUnlit(nullptr)
	, m_pPassForwardShading(nullptr)
	, m_pPassDeferredShading(nullptr)
	, m_pPassTileDeferredShading(nullptr)
	, m_pPassCopyColor(nullptr)
	, m_pPassCopyDepthStencil(nullptr)
	, m_pPassSSAO(nullptr)
	, m_pPassSSAOBlurHorizontal(nullptr)
	, m_pPassSSAOBlurVertical(nullptr)
	, m_pPassBloomLuminanceThreshold(nullptr)
	, m_pPassBloomBlurHorizontal(nullptr)
	, m_pPassBloomBlurVertical(nullptr)
	, m_pPassBloomBlendAdd(nullptr)
	, m_pPassAutoExposure(nullptr)
	, m_pPassColorGrading(nullptr)
	, m_pPassFinal(nullptr)
{
	SetVertexAttributes(vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
	SetInstanceAttributes(instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);

	Settings()->SetValue("RenderSystem.Shadow.Factor", 1.0f);
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor0", exp(-4.0f));
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor1", exp(-3.0f));
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor2", exp(-2.0f));
	Settings()->SetValue("RenderSystem.Shadow.SplitFactor3", exp(-1.0f));
	Settings()->SetValue("RenderSystem.SSAO.SampleCount", 8.0f);
	Settings()->SetValue("RenderSystem.SSAO.MinSampleRadius", 0.02f);
	Settings()->SetValue("RenderSystem.SSAO.MaxSampleRadius", 1.25f);
	Settings()->SetValue("RenderSystem.SSAO.MinDepthRange", 0.00f);
	Settings()->SetValue("RenderSystem.SSAO.MaxDepthRange", 0.20f);
	Settings()->SetValue("RenderSystem.SSAO.BlurRange", 1.00f);
	Settings()->SetValue("RenderSystem.Bloom.LuminanceThreshold", 1.00f);
	Settings()->SetValue("RenderSystem.Bloom.BlurRange.FirstTime", 1.00f);
	Settings()->SetValue("RenderSystem.Bloom.BlurRange.SecondTime", 0.75f);
}

CRenderSystem::~CRenderSystem(void)
{
	Destroy();
}

void CRenderSystem::Create(GfxApi api, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
{
	CreateRenderer(api, hInstance, hWnd, hDC, width, height, format);
	CreateCommandBuffers();
	CreateStorageBuffers();
	CreateRenderTextures();
	CreateComputes();
	CreatePasses();
}

void CRenderSystem::CreateRenderer(GfxApi api, void* hInstance, void* hWnd, void* hDC, int width, int height, GfxPixelFormat format)
{
	switch ((int)api) {
	case GFX_API_GLES3:
		m_pRenderer = new CGLES3Renderer(hInstance, hWnd, hDC, width, height, format);
		break;

	case GFX_API_VULKAN:
		m_pRenderer = new CVKRenderer(hInstance, hWnd, hDC, width, height, format);
		break;
	}

	m_pEngineUniform = new CUniformEngine;
	m_pInstanceBufferPool = new CInstanceBufferPool;
}

void CRenderSystem::CreatePasses(void)
{
	CPassPreZ::Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassShadow::Create(GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassUnlit::Create(GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32, GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassForwardShading::Create(GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32, GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassDeferredShading::Create(GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32, GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassTileDeferredShading::Create(GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32, GFX_PIXELFORMAT_D32_SFLOAT_PACK32);
	CPassCopyColor::Create(GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32);
	CPassCopyDepthStencil::Create(GFX_PIXELFORMAT_R32_SFLOAT_PACK32);
	CPassSSAO::Create(GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32);
	CPassBlurBox::Create(GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32);
	CPassBlurHorizontal::Create(GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32);
	CPassBlurVertical::Create(GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32);
	CPassBlendAdd::Create(GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32);
	CPassLuminanceThreshold::Create(GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32);
	CPassAutoExposure::Create(GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32);
	CPassColorGrading::Create(GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32);
	CPassFinal::Create(GFX_PIXELFORMAT_RGBA8_UNORM_PACK8);

	m_pPassPreZ = new CPassPreZ(this);
	m_pPassShadow = new CPassShadow(this);
	m_pPassUnlit = new CPassUnlit(this);
	m_pPassForwardShading = new CPassForwardShading(this);
	m_pPassDeferredShading = new CPassDeferredShading(this);
	m_pPassTileDeferredShading = new CPassTileDeferredShading(this);
	m_pPassCopyColor = new CPassCopyColor(this);
	m_pPassCopyDepthStencil = new CPassCopyDepthStencil(this);
	m_pPassSSAO = new CPassSSAO(this);
	m_pPassSSAOBlurHorizontal = new CPassBlurHorizontal(this);
	m_pPassSSAOBlurVertical = new CPassBlurVertical(this);
	m_pPassBloomLuminanceThreshold = new CPassLuminanceThreshold(this);
	m_pPassBloomBlurHorizontal = new CPassBlurHorizontal(this);
	m_pPassBloomBlurVertical = new CPassBlurVertical(this);
	m_pPassBloomBlendAdd = new CPassBlendAdd(this);
	m_pPassAutoExposure = new CPassAutoExposure(this);
	m_pPassColorGrading = new CPassColorGrading(this);
	m_pPassFinal = new CPassFinal(this);
}

void CRenderSystem::CreateComputes(void)
{
	m_pGPUScene = new CGPUScene(this);
	m_pGPUCluster = new CGPUCluster(this);
	m_pGPUClusterCull = new CGPUClusterCull(this);
	m_pGPUEyeHistogram = new CGPUEyeHistogram(this);
}

void CRenderSystem::CreateCommandBuffers(void)
{
	m_ptrComputeCommandBuffer[0] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrComputeCommandBuffer[1] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrComputeCommandBuffer[2] = GfxRenderer()->NewCommandBuffer(0, true);

	m_ptrGraphicCommandBuffer[0] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrGraphicCommandBuffer[1] = GfxRenderer()->NewCommandBuffer(0, true);
	m_ptrGraphicCommandBuffer[2] = GfxRenderer()->NewCommandBuffer(0, true);
}

void CRenderSystem::CreateStorageBuffers(void)
{
	m_ptrInstanceDataBuffer = GfxRenderer()->NewStorageBuffer(sizeof(InstanceData) * MAX_GPUSCENE_INSTANCE_COUNT);
	m_ptrTransferDataBuffer = GfxRenderer()->NewStorageBuffer(sizeof(InstanceData) * MAX_GPUSCENE_TRANSFER_COUNT);
	m_ptrTransferIndexBuffer = GfxRenderer()->NewStorageBuffer(sizeof(int) * MAX_GPUSCENE_TRANSFER_COUNT);

	m_ptrClusterBuffer = GfxRenderer()->NewStorageBuffer(CLUSTER_WIDTH_TILE_COUNT * CLUSTER_HEIGHT_TILE_COUNT * CLUSTER_DEPTH_TILE_COUNT * 32);
	m_ptrFullLightListBuffer = GfxRenderer()->NewStorageBuffer(MAX_GPUSCENE_INSTANCE_COUNT * sizeof(int));
	m_ptrCullLightListBuffer = GfxRenderer()->NewStorageBuffer(MAX_GPUSCENE_INSTANCE_COUNT * sizeof(int));

	m_ptrHistogramBuffer = GfxRenderer()->NewStorageBuffer(HISTOGRAM_SIZE * sizeof(int));
}

void CRenderSystem::CreateRenderTextures(void)
{
	CreateRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR0, GfxRenderer()->GetSwapChain()->GetFrameTexture(0));
	CreateRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR1, GfxRenderer()->GetSwapChain()->GetFrameTexture(1));
	CreateRenderTexture(RENDER_TEXTURE_SWAPCHAIN_COLOR2, GfxRenderer()->GetSwapChain()->GetFrameTexture(2));
	CreateRenderTexture(RENDER_TEXTURE_SHADOW, GFX_PIXELFORMAT_D32_SFLOAT_PACK32, 4096, 4096);
	CreateRenderTexture(RENDER_TEXTURE_GBUFFER0, GFX_PIXELFORMAT_RGBA8_UNORM_PACK8, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	CreateRenderTexture(RENDER_TEXTURE_GBUFFER1, GFX_PIXELFORMAT_RGBA8_UNORM_PACK8, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
//	CreateRenderTexture(RENDER_TEXTURE_GBUFFER2, GFX_PIXELFORMAT_RGBA8_UNORM_PACK8, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
//	CreateRenderTexture(RENDER_TEXTURE_GBUFFER3, GFX_PIXELFORMAT_RGBA8_UNORM_PACK8, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	CreateRenderTexture(RENDER_TEXTURE_FULL_DEPTH, GFX_PIXELFORMAT_D32_SFLOAT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	CreateRenderTexture(RENDER_TEXTURE_FULL_DEPTH_COPY, GFX_PIXELFORMAT_R32_SFLOAT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	CreateRenderTexture(RENDER_TEXTURE_FULL_HDR_COLOR0, GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	CreateRenderTexture(RENDER_TEXTURE_FULL_HDR_COLOR1, GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth(), GfxRenderer()->GetSwapChain()->GetHeight());
	CreateRenderTexture(RENDER_TEXTURE_QUATER_HDR_COLOR0, GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth() / 4, GfxRenderer()->GetSwapChain()->GetHeight() / 4);
	CreateRenderTexture(RENDER_TEXTURE_QUATER_HDR_COLOR1, GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32, GfxRenderer()->GetSwapChain()->GetWidth() / 4, GfxRenderer()->GetSwapChain()->GetHeight() / 4);
	CreateRenderTexture(RENDER_TEXTURE_HISTOGRAM_HDR_COLOR, GFX_PIXELFORMAT_RG11B10_UFLOAT_PACK32, HISTOGRAM_TEXTURE_WIDTH, HISTOGRAM_TEXTURE_HEIGHT);
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

void CRenderSystem::Destroy(void)
{
	DestroyPasses();
	DestroyComputes();
	DestroyStorageBuffers();
	DestroyRenderTextures();
	DestroyCommandBuffers();
	DestroyRenderer();
}

void CRenderSystem::DestroyRenderer(void)
{
	delete m_pInstanceBufferPool;
	delete m_pEngineUniform;
	delete m_pRenderer;
}

void CRenderSystem::DestroyPasses(void)
{
	CPassPreZ::Destroy();
	CPassShadow::Destroy();
	CPassUnlit::Destroy();
	CPassForwardShading::Destroy();
	CPassDeferredShading::Destroy();
	CPassTileDeferredShading::Destroy();
	CPassCopyColor::Destroy();
	CPassCopyDepthStencil::Destroy();
	CPassSSAO::Destroy();
	CPassBlurBox::Destroy();
	CPassBlurHorizontal::Destroy();
	CPassBlurVertical::Destroy();
	CPassBlendAdd::Destroy();
	CPassLuminanceThreshold::Destroy();
	CPassAutoExposure::Destroy();
	CPassColorGrading::Destroy();
	CPassFinal::Destroy();

	delete m_pPassPreZ;
	delete m_pPassShadow;
	delete m_pPassUnlit;
	delete m_pPassForwardShading;
	delete m_pPassDeferredShading;
	delete m_pPassTileDeferredShading;
	delete m_pPassCopyColor;
	delete m_pPassCopyDepthStencil;
	delete m_pPassSSAO;
	delete m_pPassSSAOBlurHorizontal;
	delete m_pPassSSAOBlurVertical;
	delete m_pPassBloomLuminanceThreshold;
	delete m_pPassBloomBlurHorizontal;
	delete m_pPassBloomBlurVertical;
	delete m_pPassBloomBlendAdd;
	delete m_pPassAutoExposure;
	delete m_pPassColorGrading;
	delete m_pPassFinal;
}

void CRenderSystem::DestroyComputes(void)
{
	delete m_pGPUEyeHistogram;
	delete m_pGPUClusterCull;
	delete m_pGPUCluster;
	delete m_pGPUScene;
}

void CRenderSystem::DestroyCommandBuffers(void)
{
	m_ptrComputeCommandBuffer[0].Release();
	m_ptrComputeCommandBuffer[1].Release();
	m_ptrComputeCommandBuffer[2].Release();

	m_ptrGraphicCommandBuffer[0].Release();
	m_ptrGraphicCommandBuffer[1].Release();
	m_ptrGraphicCommandBuffer[2].Release();
}

void CRenderSystem::DestroyStorageBuffers(void)
{
	m_ptrInstanceDataBuffer.Release();
	m_ptrTransferDataBuffer.Release();
	m_ptrTransferIndexBuffer.Release();

	m_ptrClusterBuffer.Release();
	m_ptrFullLightListBuffer.Release();
	m_ptrCullLightListBuffer.Release();
}

void CRenderSystem::DestroyRenderTextures(void)
{
	m_ptrRenderTextures.clear();
}

int CRenderSystem::AddInstance(void)
{
	return m_pGPUScene->AddInstance();
}

void CRenderSystem::RemoveInstance(int index)
{
	m_pGPUScene->RemoveInstance(index);
}

void CRenderSystem::ModifyInstance(int index, const InstanceData &data, int indexThread)
{
	m_pGPUScene->ModifyInstance(index, data, indexThread);
}

const InstanceData& CRenderSystem::GetInstance(int index) const
{
	return m_pGPUScene->GetInstance(index);
}

int CRenderSystem::GetDefaultInstanceIndex(void) const
{
	return m_pGPUScene->GetDefaultInstanceIndex();
}

int CRenderSystem::GetPostProcessInstnaceIndex(void) const
{
	return m_pGPUScene->GetPostProcessInstnaceIndex();
}

CUniformEngine* CRenderSystem::GetEngineUniform(void) const
{
	return m_pEngineUniform;
}

const CGfxUniformBufferPtr CRenderSystem::GetEngineUniformBuffer(void) const
{
	return m_pEngineUniform->GetUniformBuffer();
}

const CGfxMultiInstanceBufferPtr CRenderSystem::GetInstanceBuffer(uint32_t instanceFormat, int instanceBinding) const
{
	return m_pInstanceBufferPool->GetInstanceBuffer(instanceFormat, instanceBinding);
}

const CGfxStorageBufferPtr CRenderSystem::GetClusterBuffer(void) const
{
	return m_ptrClusterBuffer;
}

const CGfxStorageBufferPtr CRenderSystem::GetFullLightListBuffer(void) const
{
	return m_ptrFullLightListBuffer;
}

const CGfxStorageBufferPtr CRenderSystem::GetCullLightListBuffer(void) const
{
	return m_ptrCullLightListBuffer;
}

const CGfxStorageBufferPtr CRenderSystem::GetInstanceDataBuffer(void) const
{
	return m_ptrInstanceDataBuffer;
}

const CGfxStorageBufferPtr CRenderSystem::GetTransferDataBuffer(void) const
{
	return m_ptrTransferDataBuffer;
}

const CGfxStorageBufferPtr CRenderSystem::GetTransferIndexBuffer(void) const
{
	return m_ptrTransferIndexBuffer;
}

const CGfxStorageBufferPtr CRenderSystem::GetHistogramBuffer(void) const
{
	return m_ptrHistogramBuffer;
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

void CRenderSystem::UpdateCamera(CTaskPool& taskPool, CTaskGraph& taskGraph, CCamera* pCamera, uint32_t mask, bool bComputeLOD)
{
	pCamera->Update(taskPool, taskGraph, mask, bComputeLOD);
}
