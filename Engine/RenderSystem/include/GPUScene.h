#pragma once
#include "PreHeader.h"


class CALL_API CGfxGPUScene
{
private:
	typedef struct InstanceData {
		glm::mat4 transformMatrix;
	} InstanceData;

	typedef struct TransferData {
		uint32_t index;
		InstanceData data;
	} TransferData;


public:
	CGfxGPUScene(void);
	virtual ~CGfxGPUScene(void);


public:
	void AddInstance(uint32_t name);
	void RemoveInstance(uint32_t name);
	void ModifyInstance(uint32_t name, const InstanceData &data);

public:
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
