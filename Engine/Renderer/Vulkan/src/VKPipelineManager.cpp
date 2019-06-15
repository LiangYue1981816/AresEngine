#include "VKRenderer.h"


static void LoadPipelineCache(const char* szFileName, eastl::vector<uint8_t>& cacheData)
{
	if (FILE* pFile = fopen(szFileName, "rb")) {
		cacheData.resize(fsize(pFile));
		fread(cacheData.data(), 1, cacheData.size(), pFile);
		fclose(pFile);
	}
}

static void SavePipelineCache(const char* szFileName, eastl::vector<uint8_t>& cacheData)
{
	if (FILE* pFile = fopen(szFileName, "wb")) {
		fwrite(cacheData.data(), 1, cacheData.size(), pFile);
		fclose(pFile);
	}
}


CVKPipelineComputeManager::CVKPipelineComputeManager(CVKDevice* pDevice)
	: m_pDevice(pDevice)
	, m_vkPipelineCache(VK_NULL_HANDLE)
{
	eastl::vector<uint8_t> cacheData;
	LoadPipelineCache("./ComputeCache.data", cacheData);

	VkPipelineCacheCreateInfo cacheCreateInfo = {};
	cacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	cacheCreateInfo.pNext = nullptr;
	cacheCreateInfo.flags = 0;
	cacheCreateInfo.initialDataSize = cacheData.size();
	cacheCreateInfo.pInitialData = cacheData.data();
	vkCreatePipelineCache(m_pDevice->GetDevice(), &cacheCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkPipelineCache);
}

CVKPipelineComputeManager::~CVKPipelineComputeManager(void)
{
	size_t size = 0;
	vkGetPipelineCacheData(m_pDevice->GetDevice(), m_vkPipelineCache, &size, nullptr);

	eastl::vector<uint8_t> cacheData(size);
	vkGetPipelineCacheData(m_pDevice->GetDevice(), m_vkPipelineCache, &size, cacheData.data());
	vkDestroyPipelineCache(m_pDevice->GetDevice(), m_vkPipelineCache, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());

	SavePipelineCache("./ComputeCache.data", cacheData);

	for (const auto& itPipeline : m_pPipelines) {
		delete itPipeline.second;
	}
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
	eastl::vector<uint8_t> cacheData;
	LoadPipelineCache("./GraphicsCache.data", cacheData);

	VkPipelineCacheCreateInfo cacheCreateInfo = {};
	cacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	cacheCreateInfo.pNext = nullptr;
	cacheCreateInfo.flags = 0;
	cacheCreateInfo.initialDataSize = cacheData.size();
	cacheCreateInfo.pInitialData = cacheData.data();
	vkCreatePipelineCache(m_pDevice->GetDevice(), &cacheCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkPipelineCache);
}

CVKPipelineGraphicsManager::~CVKPipelineGraphicsManager(void)
{
	size_t size = 0;
	vkGetPipelineCacheData(m_pDevice->GetDevice(), m_vkPipelineCache, &size, nullptr);

	eastl::vector<uint8_t> cacheData(size);
	vkGetPipelineCacheData(m_pDevice->GetDevice(), m_vkPipelineCache, &size, cacheData.data());
	vkDestroyPipelineCache(m_pDevice->GetDevice(), m_vkPipelineCache, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());

	SavePipelineCache("./GraphicsCache.data", cacheData);

	for (const auto& itPipeline : m_pPipelines) {
		delete itPipeline.second;
	}
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
