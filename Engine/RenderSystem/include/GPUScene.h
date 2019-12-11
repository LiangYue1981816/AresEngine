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
		glm::vec4 center;

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
	void ModifyInstanceData(int index, const InstanceData &data, int indexThread = 0);
	void Clear(void);

public:
	const InstanceData& GetInstanceData(int index) const;
	int GetDefaultInstanceIndex(void) const;

private:
	void Update(CTaskGraph& taskGraph);


private:
	int m_indexDefaultInstance;

private:
	eastl::vector<InstanceData> m_instanceBuffer;
	eastl::unordered_map<int, TransferData> m_transferBuffer[MAX_THREAD_COUNT];
	eastl::unordered_set<int> m_freeIndex;

private:
	CGfxStorageBufferPtr m_ptrInstanceBuffer;
	CGfxStorageBufferPtr m_ptrTransferBuffer;
};
