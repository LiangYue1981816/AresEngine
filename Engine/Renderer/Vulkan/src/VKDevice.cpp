#include "VKRenderer.h"


CVKDevice::CVKDevice(CVKInstance *pInstance)
	: m_pInstance(pInstance)

	, m_vkDevice(VK_NULL_HANDLE)
	, m_vkPhysicalDevice(VK_NULL_HANDLE)

	, m_queueFamilyIndex(0)
	, m_pComputeQueue(nullptr)
	, m_pGraphicsQueue(nullptr)
	, m_pTransferQueue(nullptr)
{
	uint32_t queueFamilyIndex;
	VkPhysicalDevice vkPhysicalDevice;
	eastl::vector<VkPhysicalDevice> devices;
	CALL_BOOL_FUNCTION_RETURN(EnumeratePhysicalDevices(devices));
	CALL_BOOL_FUNCTION_RETURN(SelectPhysicalDevices(devices, vkPhysicalDevice, queueFamilyIndex));
	CALL_BOOL_FUNCTION_RETURN(CreateDevice(vkPhysicalDevice, queueFamilyIndex));

	m_pComputeQueue = new CVKQueue(this, queueFamilyIndex, 0);
	m_pGraphicsQueue = new CVKQueue(this, queueFamilyIndex, 1);
	m_pTransferQueue = new CVKQueue(this, queueFamilyIndex, 2);
}

CVKDevice::~CVKDevice(void)
{
	delete m_pComputeQueue;
	delete m_pGraphicsQueue;
	delete m_pTransferQueue;

	DestroyDevice();
}

bool CVKDevice::EnumeratePhysicalDevices(eastl::vector<VkPhysicalDevice> &devices) const
{
	devices.clear();

	uint32_t numDevices;
	CALL_VK_FUNCTION_RETURN_BOOL(vkEnumeratePhysicalDevices(m_pInstance->GetInstance(), &numDevices, nullptr));
	if (numDevices == 0) return false;

	devices.resize(numDevices);
	CALL_VK_FUNCTION_RETURN_BOOL(vkEnumeratePhysicalDevices(m_pInstance->GetInstance(), &numDevices, devices.data()));

	return true;
}

bool CVKDevice::SelectPhysicalDevices(eastl::vector<VkPhysicalDevice> &devices, VkPhysicalDevice &vkPhysicalDevice, uint32_t &queueFamilyIndex) const
{
	uint32_t familyIndex = UINT32_MAX;

	for (uint32_t index = 0; index < devices.size(); index++) {
		if (CheckPhysicalDeviceCapabilities(devices[index]) == false) continue;
		if (CheckPhysicalDeviceExtensionProperties(devices[index]) == false) continue;
		if (CheckPhysicalDeviceQueueFamilyProperties(devices[index], familyIndex) == false) continue;

		queueFamilyIndex = familyIndex;
		vkPhysicalDevice = devices[index];

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

	return true;
}

bool CVKDevice::CheckPhysicalDeviceExtensionProperties(VkPhysicalDevice vkPhysicalDevice) const
{
	uint32_t numExtensions;
	CALL_VK_FUNCTION_RETURN_BOOL(vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &numExtensions, nullptr));
	if (numExtensions == 0) return false;

	eastl::vector<VkExtensionProperties> extensions(numExtensions);
	CALL_VK_FUNCTION_RETURN_BOOL(vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &numExtensions, extensions.data()));

	for (uint32_t index = 0; index < numExtensions; index++) {
		if (stricmp(extensions[index].extensionName, VK_KHR_SWAPCHAIN_EXTENSION_NAME) == 0) {
			return true;
		}
	}

	return false;
}

bool CVKDevice::CheckPhysicalDeviceQueueFamilyProperties(VkPhysicalDevice vkPhysicalDevice, uint32_t &queueFamilyIndex) const
{
	queueFamilyIndex = UINT32_MAX;

	uint32_t numQueueFamilies;
	vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &numQueueFamilies, nullptr);
	if (numQueueFamilies == 0) return false;

	eastl::vector<VkQueueFamilyProperties> queueFamilies(numQueueFamilies);
	vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &numQueueFamilies, queueFamilies.data());

	for (uint32_t index = 0; index < numQueueFamilies; index++) {
		if (queueFamilies[index].queueFlags & VK_QUEUE_COMPUTE_BIT &&
			queueFamilies[index].queueFlags & VK_QUEUE_GRAPHICS_BIT &&
			queueFamilies[index].queueFlags & VK_QUEUE_TRANSFER_BIT) {
			VkBool32 surfaceSupported;
			CALL_VK_FUNCTION_RETURN_BOOL(vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, index, m_pInstance->GetSurface(), &surfaceSupported));

			if (surfaceSupported == VK_TRUE) {
				queueFamilyIndex = index;
				return true;
			}
		}
	}

	return false;
}

bool CVKDevice::CreateDevice(VkPhysicalDevice vkPhysicalDevice, uint32_t queueFamilyIndex)
{
	m_queueFamilyIndex = queueFamilyIndex;
	m_vkPhysicalDevice = vkPhysicalDevice;

	vkGetPhysicalDeviceFeatures(m_vkPhysicalDevice, &m_vkPhysicalDeviceFeatures);
	vkGetPhysicalDeviceProperties(m_vkPhysicalDevice, &m_vkPhysicalDeviceProperties);
	vkGetPhysicalDeviceMemoryProperties(m_vkPhysicalDevice, &m_vkPhysicalDeviceMemoryProperties);

	float queuePpriorities[3] = { 1.0f, 1.0f, 1.0f };
	VkDeviceQueueCreateInfo queueCreateInfo[1] = {};
	queueCreateInfo[0].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo[0].pNext = nullptr;
	queueCreateInfo[0].flags = 0;
	queueCreateInfo[0].queueCount = 3;
	queueCreateInfo[0].queueFamilyIndex = queueFamilyIndex;
	queueCreateInfo[0].pQueuePriorities = queuePpriorities;

	const char *szSwapchainExtension = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.pNext = nullptr;
	deviceCreateInfo.flags = 0;
	deviceCreateInfo.queueCreateInfoCount = 1;
	deviceCreateInfo.pQueueCreateInfos = queueCreateInfo;
	deviceCreateInfo.enabledLayerCount = 0;
	deviceCreateInfo.ppEnabledLayerNames = nullptr;
	deviceCreateInfo.enabledExtensionCount = 1;
	deviceCreateInfo.ppEnabledExtensionNames = &szSwapchainExtension;
	deviceCreateInfo.pEnabledFeatures = &m_vkPhysicalDeviceFeatures;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateDevice(vkPhysicalDevice, &deviceCreateInfo, m_pInstance->GetAllocator()->GetAllocationCallbacks(), &m_vkDevice));

	return true;
}

void CVKDevice::DestroyDevice(void)
{
	if (m_vkDevice) {
		vkDestroyDevice(m_vkDevice, m_pInstance->GetAllocator()->GetAllocationCallbacks());
	}

	m_queueFamilyIndex = 0;
	m_vkDevice = VK_NULL_HANDLE;
	m_vkPhysicalDevice = VK_NULL_HANDLE;
}

VkDevice CVKDevice::GetDevice(void) const
{
	return m_vkDevice;
}

VkPhysicalDevice CVKDevice::GetPhysicalDevice(void) const
{
	return m_vkPhysicalDevice;
}

CVKQueue* CVKDevice::GetComputeQueue(void) const
{
	return m_pComputeQueue;
}

CVKQueue* CVKDevice::GetGraphicsQueue(void) const
{
	return m_pGraphicsQueue;
}

CVKQueue* CVKDevice::GetTransferQueue(void) const
{
	return m_pTransferQueue;
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
