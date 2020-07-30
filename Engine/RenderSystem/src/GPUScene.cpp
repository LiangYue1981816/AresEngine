#include "RenderHeader.h"


CGPUScene::CGPUScene(CRenderSystem* pRenderSystem)
	: CPassBase(pRenderSystem)
	, m_pShaderCompute(nullptr)
	, m_pPipelineCompute(nullptr)

	, m_indexDefaultInstance(INVALID_VALUE)
	, m_indexPostProcessInstnace(INVALID_VALUE)
{
	char szFileName[] = "GPU_TransferSceneData.glsl";
	char szBinFileName[_MAX_STRING] = { 0 };

	sprintf(szBinFileName, "%x.comp", HashValue(szFileName));
	ShaderCompiler()->Compile(FileManager()->GetFullName(szFileName), szBinFileName, shaderc_compute_shader);

	m_pShaderCompute = GfxRenderer()->CreateShader(szBinFileName, compute_shader);
	m_pPipelineCompute = GfxRenderer()->CreatePipelineCompute(m_pShaderCompute);

	m_ptrDescriptorSet = GfxRenderer()->NewDescriptorSet(HashValue(szFileName), m_pPipelineCompute->GetDescriptorLayout(DESCRIPTOR_SET_PASS));
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_SCENE_DATA_NAME, m_pRenderSystem->GetInstanceBuffer(), 0, m_pRenderSystem->GetInstanceBuffer()->GetSize());
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_TRANSFER_SCENE_DATA_NAME, m_pRenderSystem->GetTransferBuffer(), 0, m_pRenderSystem->GetTransferBuffer()->GetSize());
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_TRANSFER_SCENE_DATA_INDEX_NAME, m_pRenderSystem->GetTransferIndexBuffer(), 0, m_pRenderSystem->GetTransferIndexBuffer()->GetSize());

	m_indexDefaultInstance = AddInstance();
	m_indexPostProcessInstnace = AddInstance();
}

CGPUScene::~CGPUScene(void)
{

}

int CGPUScene::GetDefaultInstanceIndex(void) const
{
	return m_indexDefaultInstance;
}

int CGPUScene::GetPostProcessInstnaceIndex(void) const
{
	return m_indexPostProcessInstnace;
}

int CGPUScene::AddInstance(void)
{
	int index;

	if (m_freeIndex.empty()) {
		index = m_instanceDataBuffer.size();
		m_instanceDataBuffer.emplace_back();
	}
	else {
		index = *m_freeIndex.begin();
		m_freeIndex.erase(index);
	}

	return index;
}

void CGPUScene::RemoveInstance(int index)
{
	if (index >= 0 && index < m_instanceDataBuffer.size() && index != m_indexDefaultInstance) {
		if (m_freeIndex.find(index) == m_freeIndex.end()) {
			m_freeIndex.emplace(index);

			for (int indexThread = 0; indexThread < MAX_THREAD_COUNT; indexThread++) {
				m_transferIndexBuffer[indexThread].erase(index);
			}
		}
	}
}

void CGPUScene::ModifyInstance(int index, const InstanceData &data, int indexThread)
{
	if (indexThread >= 0 && indexThread < MAX_THREAD_COUNT) {
		if (index >= 0 && index < m_instanceDataBuffer.size() && index != m_indexDefaultInstance) {
			if (m_freeIndex.find(index) == m_freeIndex.end()) {
				m_instanceDataBuffer[index] = data;
				m_transferIndexBuffer[indexThread].emplace(index);
			}
		}
	}
}

const InstanceData& CGPUScene::GetInstance(int index) const
{
	static InstanceData invalid;

	if (index >= 0 && index < m_instanceDataBuffer.size()) {
		return m_instanceDataBuffer[index];
	}
	else {
		return invalid;
	}
}

void CGPUScene::Compute(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	// Update Buffer
	eastl::vector<InstanceData> datas;
	eastl::vector<int> indices;
	{
		datas.reserve(MAX_GPUSCENE_TRANSFER_COUNT);
		indices.reserve(MAX_GPUSCENE_TRANSFER_COUNT);

		for (int indexThread = 0; indexThread < MAX_THREAD_COUNT; indexThread++) {
			do {
				if (datas.size() == MAX_GPUSCENE_TRANSFER_COUNT) {
					goto TRANSFER_FULL;
				}

				if (m_transferIndexBuffer[indexThread].empty()) {
					break;
				}

				const auto& itTransferIndex = m_transferIndexBuffer[indexThread].begin();
				{
					if (*itTransferIndex < MAX_GPUSCENE_INSTANCE_COUNT) {
						datas.emplace_back(m_instanceDataBuffer[*itTransferIndex]);
						indices.emplace_back(*itTransferIndex);
					}
				}
				m_transferIndexBuffer[indexThread].erase(itTransferIndex);
			} while (true);
		}

	TRANSFER_FULL:
		if (datas.empty()) {
			return;
		}
	}
	m_pRenderSystem->GetTransferBuffer()->BufferData(0, sizeof(InstanceData) * datas.size(), (const void*)datas.data());
	m_pRenderSystem->GetTransferIndexBuffer()->BufferData(0, sizeof(int) * indices.size(), (const void*)indices.data());

	// Compute
	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "TransferSceneData");
	{
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetInstanceBuffer(), GFX_ACCESS_TRANSFER_READ_BIT, GFX_ACCESS_TRANSFER_WRITE_BIT);
		{
			const int local_size_x = 128;
			const int local_size_y = 1;
			const int local_size_z = 1;

			GfxRenderer()->CmdBindPipelineCompute(ptrCommandBuffer, m_pPipelineCompute);
			GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, m_ptrDescriptorSet);
			GfxRenderer()->CmdUniform1i(ptrCommandBuffer, HashValue("Param.numTransfers"), (int)datas.size());
			GfxRenderer()->CmdDispatch(ptrCommandBuffer, (int)datas.size() / local_size_x + 1, 0 / local_size_y + 1, 0 / local_size_z + 1);
		}
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_pRenderSystem->GetInstanceBuffer(), GFX_ACCESS_TRANSFER_WRITE_BIT, GFX_ACCESS_TRANSFER_READ_BIT);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);
}
