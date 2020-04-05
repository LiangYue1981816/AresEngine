#include "EngineHeader.h"
#include "RenderHeader.h"


CGPUScene::CGPUScene(void)
	: m_indexDefaultInstance(INVALID_VALUE)
	, m_indexPostProcessInstnace(INVALID_VALUE)

	, m_pShaderCompute(nullptr)
	, m_pPipelineCompute(nullptr)
{
	char szFileName[] = "TransferSceneData.glsl";
	char szBinFileName[_MAX_STRING] = { 0 };

	sprintf(szBinFileName, "%x.comp", HashValue(szFileName));
	ShaderCompiler()->Compile(FileManager()->GetFullName(szFileName), szBinFileName, shaderc_compute_shader);

	m_pShaderCompute = GfxRenderer()->CreateShader(szBinFileName, compute_shader);
	m_pPipelineCompute = GfxRenderer()->CreatePipelineCompute(m_pShaderCompute);

	const uint32_t size = sizeof(InstanceData) * 16 * 1024;
	m_ptrInstanceBuffer = GfxRenderer()->NewStorageBuffer(size);
	m_ptrTransferBuffer = GfxRenderer()->NewStorageBuffer(size);

	m_ptrDescriptorSet = GfxRenderer()->NewDescriptorSet(HashValue(szFileName), m_pPipelineCompute->GetDescriptorLayout(DESCRIPTOR_SET_PASS));
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_SCENE_DATA_NAME, m_ptrInstanceBuffer, 0, m_ptrInstanceBuffer->GetSize());
	m_ptrDescriptorSet->SetStorageBuffer(STORAGE_TRANSFER_SCENE_DATA_NAME, m_ptrTransferBuffer, 0, m_ptrTransferBuffer->GetSize());

	m_indexDefaultInstance = AddInstance();
	m_indexPostProcessInstnace = AddInstance();
}

CGPUScene::~CGPUScene(void)
{

}

const CGfxStorageBufferPtr CGPUScene::GetInstanceBuffer(void) const
{
	return m_ptrInstanceBuffer;
}

const CGfxStorageBufferPtr CGPUScene::GetTransferBuffer(void) const
{
	return m_ptrTransferBuffer;
}

int CGPUScene::AddInstance(void)
{
	int index;

	if (m_freeIndex.empty()) {
		index = m_instanceBuffer.size();
		m_instanceBuffer.emplace_back();
	}
	else {
		index = *m_freeIndex.begin();
		m_freeIndex.erase(index);
	}

	return index;
}

void CGPUScene::RemoveInstance(int index)
{
	if (index >= 0 && index < m_instanceBuffer.size() && index != m_indexDefaultInstance) {
		if (m_freeIndex.find(index) == m_freeIndex.end()) {
			m_freeIndex.emplace(index);

			for (int indexThread = 0; indexThread < MAX_THREAD_COUNT; indexThread++) {
				m_transferBuffer[indexThread].erase(index);
			}
		}
	}
}

void CGPUScene::ModifyInstanceData(int index, const InstanceData &data, int indexThread)
{
	if (indexThread >= 0 && indexThread < MAX_THREAD_COUNT) {
		if (index >= 0 && index < m_instanceBuffer.size() && index != m_indexDefaultInstance) {
			if (m_freeIndex.find(index) == m_freeIndex.end()) {
				m_transferBuffer[indexThread][index].Set(index, data);
			}
		}
	}
}

int CGPUScene::GetDefaultInstanceIndex(void) const
{
	return m_indexDefaultInstance;
}

int CGPUScene::GetPostProcessInstnaceIndex(void) const
{
	return m_indexPostProcessInstnace;
}

const InstanceData& CGPUScene::GetInstanceData(int index) const
{
	static InstanceData invalid;

	if (index >= 0 && index < m_instanceBuffer.size()) {
		return m_instanceBuffer[index];
	}
	else {
		return invalid;
	}
}

void CGPUScene::Update(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer)
{
	eastl::vector<TransferData> datas;
	{
		datas.reserve(1024);

		for (int indexThread = 0; indexThread < MAX_THREAD_COUNT; indexThread++) {
			for (const auto& itTransfer : m_transferBuffer[indexThread]) {
				datas.emplace_back(itTransfer.second);
			}

			m_transferBuffer[indexThread].clear();
		}

		if (datas.empty()) {
			return;
		}
	}
	m_ptrTransferBuffer->BufferData(0, sizeof(TransferData) * datas.size(), (const void*)datas.data());

	GfxRenderer()->CmdPushDebugGroup(ptrCommandBuffer, "TransferSceneData");
	{
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_ptrInstanceBuffer, GFX_ACCESS_TRANSFER_READ_BIT, GFX_ACCESS_TRANSFER_WRITE_BIT);
		{
			GfxRenderer()->CmdBindPipelineCompute(ptrCommandBuffer, m_pPipelineCompute);
			GfxRenderer()->CmdBindDescriptorSet(ptrCommandBuffer, m_ptrDescriptorSet);
			GfxRenderer()->CmdUniform1i(ptrCommandBuffer, HashValue("Param.numTransfer"), datas.size());
			GfxRenderer()->CmdDispatch(ptrCommandBuffer, 8, 1, 1);
		}
		GfxRenderer()->CmdSetBufferBarrier(ptrCommandBuffer, m_ptrInstanceBuffer, GFX_ACCESS_TRANSFER_WRITE_BIT, GFX_ACCESS_TRANSFER_READ_BIT);
	}
	GfxRenderer()->CmdPopDebugGroup(ptrCommandBuffer);

	// Debug
	/*
	bool bNeedUpdate = false;

	for (int indexThread = 0; indexThread < MAX_THREAD_COUNT; indexThread++) {
		for (const auto& itTransfer : m_transferBuffer[indexThread]) {
			m_instanceBuffer[itTransfer.second.index] = itTransfer.second.data;
			bNeedUpdate = true;
		}

		m_transferBuffer[indexThread].clear();
	}

	if (bNeedUpdate) {
		m_ptrInstanceBuffer->BufferData(0, sizeof(InstanceData) * m_instanceBuffer.size(), m_instanceBuffer.data());
	}
	*/
}
