#pragma once
#include "PreHeader.h"


class CALL_API CGPUScene
{
	friend class CRenderSystem;


public:
	typedef struct InstanceData {
		InstanceData(void)
		{

		}

		// Base
		glm::mat4 transformMatrix;
		glm::vec4 position;

		// Light
		glm::vec4 lightColor;
		glm::vec4 lightAttenuation;
	} InstanceData;

private:
	typedef struct TransferData {
		TransferData(void)
		{

		}
		TransferData(uint32_t _index, InstanceData _data)
		{
			index = _index;
			data = _data;
		}

		uint32_t index;
		InstanceData data;
	} TransferData;


private:
	CGPUScene(void);
	virtual ~CGPUScene(void);


public:
	uint32_t AddInstance(void);
	void RemoveInstance(uint32_t index);
	void ModifyInstanceData(uint32_t index, const InstanceData &data);

public:
	const InstanceData& GetInstanceData(uint32_t index) const;

private:
	void Update(void);


private:
	eastl::unordered_set<uint32_t> m_freeIndex;

private:
	eastl::vector<InstanceData> m_instanceBuffer;
	eastl::unordered_map<uint32_t, TransferData> m_transferBuffer[2];

private:
	CGfxStorageBufferPtr m_ptrTransferBuffer;
	CGfxStorageBufferPtr m_ptrInstanceBuffer[2];
};
