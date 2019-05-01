#pragma once
#include "VKRenderer.h"


class CVKDevice
{
	friend class CVKRenderer;


private:
	CVKDevice(CVKInstance* pInstance);
	virtual ~CVKDevice(void);


private:
	bool EnumeratePhysicalDevices(eastl::vector<VkPhysicalDevice>& devices) const;

	bool SelectPhysicalDevices(eastl::vector<VkPhysicalDevice>& devices, uint32_t& deviceIndex, uint32_t& queueFamilyIndex) const;
	bool CheckPhysicalDeviceCapabilities(VkPhysicalDevice vkPhysicalDevice) const;
	bool CheckPhysicalDeviceExtensionProperties(VkPhysicalDevice vkPhysicalDevice) const;
	bool CheckPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice vkPhysicalDevice, uint32_t& queueFamilyIndex) const;

	bool CreateDevice(VkPhysicalDevice vkPhysicalDevice, uint32_t queueFamilyIndex);
	void DestroyDevice(void);

public:
	VkDevice GetDevice(void) const;
	VkPhysicalDevice GetPhysicalDevice(void) const;

	const VkPhysicalDeviceLimits& GetPhysicalDeviceLimits(void) const;
	const VkPhysicalDeviceFeatures& GetPhysicalDeviceFeatures(void) const;
	const VkPhysicalDeviceMemoryProperties& GetPhysicalDeviceMemoryProperties(void) const;

public:
	CVKInstance* GetInstance(void) const;
	CVKQueue* GetQueue(void) const;
	CVKMemoryManager* GetMemoryManager(void) const;
	CVKTransferManager* GetTransferManager(void) const;


private:
	VkDevice m_vkDevice;
	VkPhysicalDevice m_vkPhysicalDevice;

	VkPhysicalDeviceFeatures m_vkPhysicalDeviceFeatures;
	VkPhysicalDeviceProperties m_vkPhysicalDeviceProperties;
	VkPhysicalDeviceMemoryProperties m_vkPhysicalDeviceMemoryProperties;

private:
	CVKQueue* m_pQueue;
	CVKMemoryManager* m_pMemoryManager;
	CVKTransferManager* m_pTransferManager;

private:
	CVKInstance* m_pInstance;
};
