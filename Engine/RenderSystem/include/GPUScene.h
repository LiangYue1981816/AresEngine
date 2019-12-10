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
		glm::vec3 position;

		// Light
		glm::vec4 lightColor;
		glm::vec4 lightAttenuation;
	} InstanceData;

private:
	typedef struct TransferData {
		TransferData(void)
		{

		}
		TransferData(int _index, InstanceData _data)
		{
			index = _index;
			data = _data;
		}

		int index;
		InstanceData data;
	} TransferData;


private:
	CGPUScene(void);
	virtual ~CGPUScene(void);


public:
	int AddInstance(void);
	void RemoveInstance(int index);
	void ModifyInstanceData(int indexThread, int index, const InstanceData &data);
	void Clear(void);

public:
	const InstanceData& GetInstanceData(int index) const;

private:
	void Update(void);


private:
	eastl::unordered_set<int> m_freeIndex;

private:
	eastl::vector<InstanceData> m_instanceBuffer;
	eastl::unordered_map<int, TransferData> m_transferBuffer[MAX_THREAD_COUNT];

private:
	CGfxStorageBufferPtr m_ptrTransferBuffer;
	CGfxStorageBufferPtr m_ptrInstanceBuffer;
};
