#pragma once
#include "PreHeader.h"


typedef struct InstanceData {
	InstanceData(void)
	{

	}

	void SetTransform(const glm::mat4& _transformMatrix)
	{
		transformMatrix = _transformMatrix;
	}

	void SetCenter(const glm::vec4& _center)
	{
		center = _center;
	}

	void SetLightColor(const glm::vec4& _lightColor)
	{
		lightColor = _lightColor;
	}

	void SetLightAttenuation(const glm::vec4& _lightAttenuation)
	{
		lightAttenuation = _lightAttenuation;
	}

	// Base
	glm::mat4 transformMatrix;
	glm::vec4 center;

	// Light
	glm::vec4 lightColor;
	glm::vec4 lightAttenuation;
} InstanceData;

class CALL_API CGPUScene
{
	friend class CRenderSystem;


private:
	CGPUScene(void);
	virtual ~CGPUScene(void);


public:
	const CGfxStorageBufferPtr GetInstanceBuffer(void) const;

public:
	int GetDefaultInstanceIndex(void) const;
	int GetPostProcessInstnaceIndex(void) const;

public:
	int AddInstance(void);
	void RemoveInstance(int index);
	void ModifyInstanceData(int index, const InstanceData &data, int indexThread);
	const InstanceData& GetInstanceData(int index) const;

private:
	void Compute(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	int m_indexDefaultInstance;
	int m_indexPostProcessInstnace;

private:
	eastl::unordered_set<int> m_freeIndex;
	eastl::vector<InstanceData> m_instanceDataBuffer;

private:
	eastl::unordered_set<int> m_transferIndexBuffer[MAX_THREAD_COUNT];

private:
	CGfxShader* m_pShaderCompute;
	CGfxPipelineCompute* m_pPipelineCompute;
	CGfxDescriptorSetPtr m_ptrDescriptorSet;

private:
	CGfxStorageBufferPtr m_ptrInstanceDataBuffer;
	CGfxStorageBufferPtr m_ptrTransferDataBuffer;
	CGfxStorageBufferPtr m_ptrTransferIndexBuffer;
};
