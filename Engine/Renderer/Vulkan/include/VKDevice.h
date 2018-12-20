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
	void DestroyDevice(void);

public:
	VkDevice GetDevice(void) const;
	VkPhysicalDevice GetPhysicalDevice(void) const;

	CVKInstance* GetInstance(void) const;
	CVKQueue* GetComputeQueue(void) const;
	CVKQueue* GetGraphicsQueue(void) const;
	CVKQueue* GetTransferQueue(void) const;

public:
	const VkPhysicalDeviceLimits& GetPhysicalDeviceLimits(void) const;
	const VkPhysicalDeviceFeatures& GetPhysicalDeviceFeatures(void) const;
	const VkPhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProperties(void) const;


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

private:
	CVKInstance *m_pInstance;
};
