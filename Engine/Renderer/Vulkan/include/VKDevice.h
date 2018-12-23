#pragma once
#include "VKRenderer.h"


class CVKDevice
{
	friend class CVKRenderer;


private:
	CVKDevice(CVKInstance *pInstance);
	virtual ~CVKDevice(void);


private:
	bool EnumeratePhysicalDevices(eastl::vector<VkPhysicalDevice> &devices) const;

	bool SelectPhysicalDevices(eastl::vector<VkPhysicalDevice> &devices, VkPhysicalDevice &vkPhysicalDevice, uint32_t &queueFamilyIndex) const;
	bool CheckPhysicalDeviceCapabilities(VkPhysicalDevice vkPhysicalDevice) const;
	bool CheckPhysicalDeviceExtensionProperties(VkPhysicalDevice vkPhysicalDevice) const;
	bool CheckPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice vkPhysicalDevice, uint32_t &queueFamilyIndex) const;

	bool CreateDevice(VkPhysicalDevice vkPhysicalDevice, uint32_t queueFamilyIndex);
	bool CreateDevice(void);
	void DestroyDevice(void);

public:
	VkDevice GetDevice(void) const;
	VkPhysicalDevice GetPhysicalDevice(void) const;

	const VkPhysicalDeviceLimits& GetPhysicalDeviceLimits(void) const;
	const VkPhysicalDeviceFeatures& GetPhysicalDeviceFeatures(void) const;
	const VkPhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProperties(void) const;

public:
	CVKInstance* GetInstance(void) const;
	CVKQueue* GetComputeQueue(void) const;
	CVKQueue* GetGraphicsQueue(void) const;
	CVKQueue* GetTransferQueue(void) const;
	CVKMemoryManager* GetMemoryManager(void) const;

public:
	CVKUniformBuffer* CreateUniformBuffer(size_t size, bool bDynamic) const;
	CVKIndexBuffer* CreateIndexBuffer(GfxIndexType type, size_t size, bool bDynamic) const;
	CVKVertexBuffer* CreateVertexBuffer(uint32_t vertexFormat, uint32_t vertexBinding, size_t size, bool bDynamic) const;
	CVKInstanceBuffer* CreateInstanceBuffer(uint32_t instanceFormat, uint32_t instanceBinding) const;
	CVKIndirectBuffer* CreateIndirectBuffer(uint32_t count) const;

	void DestroyUniformBuffer(CVKUniformBuffer *pUniformBuffer) const;
	void DestroyIndexBuffer(CVKIndexBuffer *pIndexBuffer) const;
	void DestroyVertexBuffer(CVKVertexBuffer *pVertexBuffer) const;
	void DestroyInstanceBuffer(CVKInstanceBuffer *pInstanceBuffer) const;
	void DestroyIndirectBuffer(CVKIndirectBuffer *pIndirectBuffer) const;


private:
	VkDevice m_vkDevice;
	VkPhysicalDevice m_vkPhysicalDevice;

	VkPhysicalDeviceFeatures m_vkPhysicalDeviceFeatures;
	VkPhysicalDeviceProperties m_vkPhysicalDeviceProperties;
	VkPhysicalDeviceMemoryProperties m_vkPhysicalDeviceMemoryProperties;

private:
	uint32_t m_queueFamilyIndex;
	CVKQueue *m_pComputeQueue;
	CVKQueue *m_pGraphicsQueue;
	CVKQueue *m_pTransferQueue;
	CVKMemoryManager *m_pMemoryManager;

private:
	CVKInstance *m_pInstance;
};
