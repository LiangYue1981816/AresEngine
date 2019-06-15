#include "VKRenderer.h"


static VkPipelineCache CreatePipelineCache(const char* szFileName, CVKDevice* pDevice)
{
	eastl::vector<uint8_t> cacheData;
	VkPipelineCache vkPipelineCache = VK_NULL_HANDLE;

	if (FILE* pFile = fopen(szFileName, "rb")) {
		cacheData.resize(fsize(pFile));
		fread(cacheData.data(), 1, cacheData.size(), pFile);
		fclose(pFile);
	}

	VkPipelineCacheCreateInfo cacheCreateInfo = {};
	cacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	cacheCreateInfo.pNext = nullptr;
	cacheCreateInfo.flags = 0;
	cacheCreateInfo.initialDataSize = cacheData.size();
	cacheCreateInfo.pInitialData = cacheData.data();
	vkCreatePipelineCache(pDevice->GetDevice(), &cacheCreateInfo, pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &vkPipelineCache);

	return vkPipelineCache;
}

static void DestroyPipelineCache(const char* szFileName, CVKDevice* pDevice, VkPipelineCache vkPipelineCache)
{
	size_t size = 0;
	vkGetPipelineCacheData(pDevice->GetDevice(), vkPipelineCache, &size, nullptr);

	eastl::vector<uint8_t> cacheData(size);
	vkGetPipelineCacheData(pDevice->GetDevice(), vkPipelineCache, &size, cacheData.data());
	vkDestroyPipelineCache(pDevice->GetDevice(), vkPipelineCache, pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());

	if (FILE* pFile = fopen(szFileName, "wb")) {
		fwrite(cacheData.data(), 1, cacheData.size(), pFile);
		fclose(pFile);
	}
}


const char* szPipelineComputeCachePath = "./PipelineComputeCache.data";
const char* szPipelineGraphicsCachePath = "./PipelineGraphicsCache.data";


CVKPipelineComputeManager::CVKPipelineComputeManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
	, m_vkPipelineCache(VK_NULL_HANDLE)
{
	m_vkPipelineCache = CreatePipelineCache(szPipelineComputeCachePath, m_pDevice);
}

CVKPipelineComputeManager::~CVKPipelineComputeManager(void)
{
	for (const auto& itPipeline : m_pPipelines) {
		delete itPipeline.second;
	}

	DestroyPipelineCache(szPipelineComputeCachePath, m_pDevice, m_vkPipelineCache);
}

CVKPipelineCompute* CVKPipelineComputeManager::Create(const CGfxShader* pComputeShader)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValueFormat("%x", pComputeShader->GetName());

		if (m_pPipelines[name] == nullptr) {
			m_pPipelines[name] = new CVKPipelineCompute(m_pDevice, m_vkPipelineCache, name);
			m_pPipelines[name]->Create(pComputeShader);
		}

		return m_pPipelines[name];
	}
}


CVKPipelineGraphicsManager::CVKPipelineGraphicsManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
	, m_vkPipelineCache(VK_NULL_HANDLE)
{
	m_vkPipelineCache = CreatePipelineCache(szPipelineGraphicsCachePath, m_pDevice);
}

CVKPipelineGraphicsManager::~CVKPipelineGraphicsManager(void)
{
	for (const auto& itPipeline : m_pPipelines) {
		delete itPipeline.second;
	}

	DestroyPipelineCache(szPipelineGraphicsCachePath, m_pDevice, m_vkPipelineCache);
}

CVKPipelineGraphics* CVKPipelineGraphicsManager::Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, uint32_t indexSubpass, uint32_t vertexBinding, uint32_t instanceBinding)
{
	mutex_autolock autolock(&lock);
	{
		uint32_t name = HashValueFormat("%p_%x_%x_%x_%x_%x_%x", pRenderPass, pVertexShader->GetName(), pFragmentShader->GetName(), HashValue((uint8_t*)& state, sizeof(state)), indexSubpass, vertexBinding, instanceBinding);

		if (m_pPipelines[name] == nullptr) {
			m_pPipelines[name] = new CVKPipelineGraphics(m_pDevice, m_vkPipelineCache, name);
			m_pPipelines[name]->Create(pRenderPass, pVertexShader, pFragmentShader, state, indexSubpass, vertexBinding, instanceBinding);
		}

		return m_pPipelines[name];
	}
}
