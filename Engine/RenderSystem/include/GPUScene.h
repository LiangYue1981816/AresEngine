#pragma once
#include "PreHeader.h"


class CALL_API CGPUScene
{
	friend class CRenderSystem;


public:
	typedef struct InstanceData {
		// Base
		glm::mat4 transformMatrix;
		glm::vec4 position;

		// Light
		glm::vec4 lightColor;
		glm::vec4 lightAttenuation;
	} InstanceData;

private:
	typedef struct TransferData {
		uint32_t index;
		InstanceData data;
	} TransferData;


private:
	CGPUScene(void);
	virtual ~CGPUScene(void);


public:
	void AddInstance(uint32_t name);
	void RemoveInstance(uint32_t name);
	void ModifyInstanceData(uint32_t name, const InstanceData &data);

public:
	uint32_t GetIndex(uint32_t name) const;
	const InstanceData& GetInstanceData(uint32_t index) const;

private:
	void Update(void);


private:
	eastl::list<uint32_t> m_freeIndex;
	eastl::unordered_map<uint32_t, uint32_t> m_nameIndex;

private:
	eastl::vector<InstanceData> m_instanceBuffer;
	eastl::vector<TransferData> m_transferBuffer[2];

private:
	CGfxStorageBufferPtr m_ptrTransferBuffer;
	CGfxStorageBufferPtr m_ptrInstanceBuffer[2];
};
