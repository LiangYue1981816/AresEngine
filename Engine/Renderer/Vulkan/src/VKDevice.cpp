#include "VKRenderer.h"


CVKDevice::CVKDevice(CVKInstance* pInstance)
	: m_pInstance(pInstance)

	, m_vkDevice(VK_NULL_HANDLE)
	, m_vkPhysicalDevice(VK_NULL_HANDLE)

	, m_vkPhysicalDeviceFeatures{}
	, m_vkPhysicalDeviceProperties{}
	, m_vkPhysicalDeviceMemoryProperties{}

	, m_pQueue(nullptr)
	, m_pMemoryManager(nullptr)
	, m_pTransferManager(nullptr)
{
	uint32_t deviceIndex;
	uint32_t queueFamilyIndex;
	eastl::vector<VkPhysicalDevice> devices;
	CALL_BOOL_FUNCTION_RETURN(EnumeratePhysicalDevices(devices));
	CALL_BOOL_FUNCTION_RETURN(SelectPhysicalDevices(devices, deviceIndex, queueFamilyIndex));
	CALL_BOOL_FUNCTION_RETURN(CreateDevice(devices[deviceIndex], queueFamilyIndex));

	m_pQueue = new CVKQueue(this, queueFamilyIndex);
	m_pMemoryManager = new CVKMemoryManager(this);
	m_pTransferManager = new CVKTransferManager(this, queueFamilyIndex);
}

CVKDevice::~CVKDevice(void)
{
	delete m_pTransferManager;
	delete m_pMemoryManager;
	delete m_pQueue;

	DestroyDevice();
}

bool CVKDevice::EnumeratePhysicalDevices(eastl::vector<VkPhysicalDevice>& devices) const
{
	uint32_t numDevices;
	CALL_VK_FUNCTION_RETURN_BOOL(vkEnumeratePhysicalDevices(m_pInstance->GetInstance(), &numDevices, nullptr));
	if (numDevices == 0) return false;

	devices.resize(numDevices);
	CALL_VK_FUNCTION_RETURN_BOOL(vkEnumeratePhysicalDevices(m_pInstance->GetInstance(), &numDevices, devices.data()));

	return true;
}

bool CVKDevice::SelectPhysicalDevices(eastl::vector<VkPhysicalDevice> & devices, uint32_t & deviceIndex, uint32_t & queueFamilyIndex) const
{
	uint32_t familyIndex = UINT32_MAX;

	for (int indexDevice = 0; indexDevice < devices.size(); indexDevice++) {
		if (CheckPhysicalDeviceCapabilities(devices[indexDevice]) == false) continue;
		if (CheckPhysicalDeviceExtensionProperties(devices[indexDevice]) == false) continue;
		if (CheckPhysicalDeviceQueueFamilyProperties(devices[indexDevice], familyIndex) == false) continue;

		deviceIndex = indexDevice;
		queueFamilyIndex = familyIndex;

		return true;
	}

	return false;
}

bool CVKDevice::CheckPhysicalDeviceCapabilities(VkPhysicalDevice vkPhysicalDevice) const
{
	VkPhysicalDeviceFeatures vkPhysicalDeviceFeatures;
	VkPhysicalDeviceProperties vkPhysicalDeviceProperties;

	vkGetPhysicalDeviceFeatures(vkPhysicalDevice, &vkPhysicalDeviceFeatures);
	vkGetPhysicalDeviceProperties(vkPhysicalDevice, &vkPhysicalDeviceProperties);

	if (VK_VERSION_MAJOR(vkPhysicalDeviceProperties.apiVersion) < 1) {
		return false;
	}

	// ...

	return true;
}

bool CVKDevice::CheckPhysicalDeviceExtensionProperties(VkPhysicalDevice vkPhysicalDevice) const
{
	uint32_t numExtensions;
	CALL_VK_FUNCTION_RETURN_BOOL(vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &numExtensions, nullptr));
	if (numExtensions == 0) return false;

	eastl::vector<VkExtensionProperties> extensions(numExtensions);
	CALL_VK_FUNCTION_RETURN_BOOL(vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &numExtensions, extensions.data()));

	bool bSwapchainExtension = false;
	// ...

	for (int indexExtension = 0; indexExtension < extensions.size(); indexExtension++) {
		if (stricmp(extensions[indexExtension].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
			bSwapchainExtension = true;
			continue;
		}

		// ...
	}

	if (bSwapchainExtension == false) {
		return false;
	}

	// ...

	return true;
}

bool CVKDevice::CheckPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice vkPhysicalDevice, uint32_t & queueFamilyIndex) const
{
	uint32_t numQueueFamilies;

	vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &numQueueFamilies, nullptr);
	if (numQueueFamilies == 0) return false;

	eastl::vector<VkQueueFamilyProperties> queueFamilies(numQueueFamilies);
	vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &numQueueFamilies, queueFamilies.data());

	for (int indexQueueFamilie = 0; indexQueueFamilie < queueFamilies.size(); indexQueueFamilie++) {
		if ((queueFamilies[indexQueueFamilie].queueFlags & VK_QUEUE_COMPUTE_BIT) != 0 &&
			(queueFamilies[indexQueueFamilie].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0 &&
			(queueFamilies[indexQueueFamilie].queueFlags & VK_QUEUE_TRANSFER_BIT) != 0) {
			VkBool32 surfaceSupported;
			CALL_VK_FUNCTION_RETURN_BOOL(vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, indexQueueFamilie, m_pInstance->GetSurface(), &surfaceSupported));

			if (surfaceSupported == VK_TRUE) {
				queueFamilyIndex = indexQueueFamilie;
				return true;
			}
		}
	}

	return false;
}

bool CVKDevice::CreateDevice(VkPhysicalDevice vkPhysicalDevice, uint32_t queueFamilyIndex)
{
	ASSERT(vkPhysicalDevice);

	m_vkPhysicalDevice = vkPhysicalDevice;

	CVKHelper::SetupFormat(m_vkPhysicalDevice);
	vkGetPhysicalDeviceFeatures(m_vkPhysicalDevice, &m_vkPhysicalDeviceFeatures);
	vkGetPhysicalDeviceProperties(m_vkPhysicalDevice, &m_vkPhysicalDeviceProperties);
	vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &m_vkPhysicalDeviceMemoryProperties);
	LogOutput(LOG_TAG_RENDERER, "Vulkan Device = %s\n", m_vkPhysicalDeviceProperties.deviceName);

	const float queuePpriorities[1] = { 1.0f };
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.pNext = nullptr;
	queueCreateInfo.flags = 0;
	queueCreateInfo.queueFamilyIndex = queueFamilyIndex;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.pQueuePriorities = queuePpriorities;

	const char* szSwapchainExtension = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext = nullptr;
	deviceCreateInfo.flags = 0;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
	deviceCreateInfo.enabledLayerCount = 0;
	deviceCreateInfo.ppEnabledLayerNames = nullptr;
	deviceCreateInfo.enabledExtensionCount = 1;
	deviceCreateInfo.ppEnabledExtensionNames = &szSwapchainExtension;
	deviceCreateInfo.pEnabledFeatures = &m_vkPhysicalDeviceFeatures;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateDevice(m_vkPhysicalDevice, &deviceCreateInfo, m_pInstance->GetAllocator()->GetAllocationCallbacks(), &m_vkDevice));

	return true;
}

void CVKDevice::DestroyDevice(void)
{
	ASSERT(m_vkDevice);

	vkDestroyDevice(m_vkDevice, m_pInstance->GetAllocator()->GetAllocationCallbacks());

	m_vkDevice = VK_NULL_HANDLE;
	m_vkPhysicalDevice = VK_NULL_HANDLE;
}

VkDevice CVKDevice::GetDevice(void) const
{
	ASSERT(m_vkDevice);
	return m_vkDevice;
}

VkPhysicalDevice CVKDevice::GetPhysicalDevice(void) const
{
	ASSERT(m_vkPhysicalDevice);
	return m_vkPhysicalDevice;
}

const VkPhysicalDeviceLimits& CVKDevice::GetPhysicalDeviceLimits(void) const
{
	return m_vkPhysicalDeviceProperties.limits;
}

const VkPhysicalDeviceFeatures& CVKDevice::GetPhysicalDeviceFeatures(void) const
{
	return m_vkPhysicalDeviceFeatures;
}

const VkPhysicalDeviceMemoryProperties& CVKDevice::GetPhysicalDeviceMemoryProperties(void) const
{
	return m_vkPhysicalDeviceMemoryProperties;
}

CVKInstance* CVKDevice::GetInstance(void) const
{
	ASSERT(m_pInstance);
	return m_pInstance;
}

CVKQueue* CVKDevice::GetQueue(void) const
{
	ASSERT(m_pQueue);
	return m_pQueue;
}

CVKMemoryManager* CVKDevice::GetMemoryManager(void) const
{
	ASSERT(m_pMemoryManager);
	return m_pMemoryManager;
}

CVKTransferManager* CVKDevice::GetTransferManager(void) const
{
	ASSERT(m_pTransferManager);
	return m_pTransferManager;
}
