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
	friend class CPassPreZ;
	friend class CPassShadow;
	friend class CPassDefault;
	friend class CPassForwardLighting;


private:
	typedef struct TransferData {
		TransferData(void)
		{

		}

		void Set(int _index, InstanceData _data)
		{
			index = glm::ivec4(_index);
			data = _data;
		}

		glm::ivec4 index;
		InstanceData data;
	} TransferData;


public:
	CGPUScene(void);
	virtual ~CGPUScene(void);


private:
	const CGfxStorageBufferPtr GetInstanceBuffer(void) const;
	const CGfxStorageBufferPtr GetTransferBuffer(void) const;

public:
	int AddInstance(void);
	void RemoveInstance(int index);
	void ModifyInstanceData(int index, const InstanceData &data, int indexThread);

	int GetDefaultInstanceIndex(void) const;
	int GetPostProcessInstnaceIndex(void) const;

	const InstanceData& GetInstanceData(int index) const;

private:
	void Update(CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	int m_indexDefaultInstance;
	int m_indexPostProcessInstnace;

private:
	eastl::vector<InstanceData> m_instanceBuffer;
	eastl::unordered_map<int, TransferData> m_transferBuffer[MAX_THREAD_COUNT];
	eastl::unordered_set<int> m_freeIndex;

private:
	CGfxShader* m_pShaderCompute;
	CGfxPipelineCompute* m_pPipelineCompute;
	CGfxDescriptorSetPtr m_ptrDescriptorSet;

private:
	CGfxStorageBufferPtr m_ptrInstanceBuffer;
	CGfxStorageBufferPtr m_ptrTransferBuffer;
};
