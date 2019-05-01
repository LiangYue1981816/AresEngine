#include "VKRenderer.h"


VkResult CVKInstance::vkErrorCode = VK_SUCCESS;
PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallback = nullptr;
PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback = nullptr;


VkBool32 VKAPI_PTR DebugReportCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objectType, uint64_t object, size_t location, int32_t messageCode, const char* pLayerPrefix, const char* pMessage, void* pUserData)
{
	if (flags & VK_DEBUG_REPORT_ERROR_BIT_EXT) {
		LogOutput(LOG_TAG_RENDERER, "Vulkan Error [%s] Code %d : %s\n", pLayerPrefix, messageCode, pMessage);
		return VK_FALSE;
	}

	if (flags & VK_DEBUG_REPORT_WARNING_BIT_EXT) {
		LogOutput(LOG_TAG_RENDERER, "Vulkan Warning [%s] Code %d : %s\n", pLayerPrefix, messageCode, pMessage);
		return VK_FALSE;
	}

	if (flags & VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT) {
		LogOutput(LOG_TAG_RENDERER, "Vulkan Preformance [%s] Code %d : %s\n", pLayerPrefix, messageCode, pMessage);
		return VK_FALSE;
	}

	if (flags & VK_DEBUG_REPORT_INFORMATION_BIT_EXT) {
		LogOutput(LOG_TAG_RENDERER, "Vulkan Information [%s] Code %d : %s\n", pLayerPrefix, messageCode, pMessage);
		return VK_FALSE;
	}

	if (flags & VK_DEBUG_REPORT_DEBUG_BIT_EXT) {
		LogOutput(LOG_TAG_RENDERER, "Vulkan Debug [%s] Code %d : %s\n", pLayerPrefix, messageCode, pMessage);
		return VK_FALSE;
	}

	return VK_TRUE;
}

void CVKInstance::SetLastError(VkResult err)
{
	vkErrorCode = err;
}

VkResult CVKInstance::GetLastError(void)
{
	return vkErrorCode;
}


CVKInstance::CVKInstance(void* hInstance, void* hWnd)
	: m_pAllocator(nullptr)
	
	, m_vkInstance(VK_NULL_HANDLE)
	, m_vkSurface(VK_NULL_HANDLE)

#ifdef DEBUG
	, m_vkDebugReportCallback(VK_NULL_HANDLE)
#endif
{
	m_pAllocator = new CVKAllocator;

	eastl::vector<const char*> enabledInstanceLayers;
	eastl::vector<const char*> enabledInstanceExtensions;
	CALL_BOOL_FUNCTION_RETURN(EnumerateInstanceLayerProperties(enabledInstanceLayers));
	CALL_BOOL_FUNCTION_RETURN(EnumerateInstanceExtensionProperties(enabledInstanceExtensions));
	CALL_BOOL_FUNCTION_RETURN(CreateInstance(enabledInstanceLayers, enabledInstanceExtensions));
	CALL_BOOL_FUNCTION_RETURN(CreateSurface(hInstance, hWnd));
}

CVKInstance::~CVKInstance(void)
{
	DestroySurface();
	DestroyInstance();

	delete m_pAllocator;
}

VkInstance CVKInstance::GetInstance(void) const
{
	return m_vkInstance;
}

VkSurfaceKHR CVKInstance::GetSurface(void) const
{
	return m_vkSurface;
}

CVKAllocator* CVKInstance::GetAllocator(void) const
{
	return m_pAllocator;
}

bool CVKInstance::EnumerateInstanceLayerProperties(eastl::vector<const char*>& enabledInstanceLayers) const
{
	enabledInstanceLayers.clear();

	uint32_t numLayers;
	CALL_VK_FUNCTION_RETURN_BOOL(vkEnumerateInstanceLayerProperties(&numLayers, nullptr));
	if (numLayers == 0) return false;

	eastl::vector<VkLayerProperties> layers(numLayers);
	CALL_VK_FUNCTION_RETURN_BOOL(vkEnumerateInstanceLayerProperties(&numLayers, layers.data()));

	for (int indexLayer = 0; indexLayer < layers.size(); indexLayer++) {
#ifdef DEBUG
#define VK_LAYER_LUNARG_STANDARD_VALIDATION "VK_LAYER_LUNARG_standard_validation"
		if (stricmp(layers[indexLayer].layerName, VK_LAYER_LUNARG_STANDARD_VALIDATION) == 0) {
			enabledInstanceLayers.emplace_back(VK_LAYER_LUNARG_STANDARD_VALIDATION);
			continue;
		}
#endif
	}

	return true;
}

bool CVKInstance::EnumerateInstanceExtensionProperties(eastl::vector<const char*>& enabledInstanceExtensions) const
{
	enabledInstanceExtensions.clear();

	uint32_t numExtensions;
	CALL_VK_FUNCTION_RETURN_BOOL(vkEnumerateInstanceExtensionProperties(nullptr, &numExtensions, nullptr));
	if (numExtensions == 0) return false;

	eastl::vector<VkExtensionProperties> extensions(numExtensions);
	CALL_VK_FUNCTION_RETURN_BOOL(vkEnumerateInstanceExtensionProperties(nullptr, &numExtensions, extensions.data()));

	bool bSurfaceExtension = false;
	bool bPlatformSurfaceExtension = false;
	for (int indexExtension = 0; indexExtension < extensions.size(); indexExtension++) {
		if (stricmp(extensions[indexExtension].extensionName, VK_KHR_SURFACE_EXTENSION_NAME) == 0) {
			enabledInstanceExtensions.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
			bSurfaceExtension = true;
			continue;
		}

#ifdef PLATFORM_WINDOWS
		if (stricmp(extensions[indexExtension].extensionName, VK_KHR_WIN32_SURFACE_EXTENSION_NAME) == 0) {
			enabledInstanceExtensions.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
			bPlatformSurfaceExtension = true;
			continue;
		}
#endif

#ifdef PLATFORM_ANDROID
		if (stricmp(extensions[indexExtension].extensionName, VK_KHR_ANDROID_SURFACE_EXTENSION_NAME) == 0) {
			enabledInstanceExtensions.emplace_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
			bPlatformSurfaceExtension = true;
			continue;
		}
#endif

#ifdef DEBUG
		if (stricmp(extensions[indexExtension].extensionName, VK_EXT_DEBUG_REPORT_EXTENSION_NAME) == 0) {
			enabledInstanceExtensions.emplace_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
			continue;
		}
#endif

		// ...
	}

	if (bSurfaceExtension == false) {
		return false;
	}

	if (bPlatformSurfaceExtension == false) {
		return false;
	}

	// ...

	return true;
}

bool CVKInstance::CreateInstance(const eastl::vector<const char*>& enabledInstanceLayers, const eastl::vector<const char*>& enabledInstanceExtensions)
{
	VkApplicationInfo appInfo = {};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pNext = nullptr;
	appInfo.pApplicationName = "";
	appInfo.applicationVersion = 1;
	appInfo.pEngineName = "";
	appInfo.engineVersion = 1;
	appInfo.apiVersion = VK_API_VERSION_1_0;

	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledLayerCount = enabledInstanceLayers.size();
	createInfo.ppEnabledLayerNames = enabledInstanceLayers.data();
	createInfo.enabledExtensionCount = enabledInstanceExtensions.size();
	createInfo.ppEnabledExtensionNames = enabledInstanceExtensions.data();
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateInstance(&createInfo, m_pAllocator->GetAllocationCallbacks(), &m_vkInstance));

#ifdef DEBUG
	vkCreateDebugReportCallback = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(m_vkInstance, "vkCreateDebugReportCallbackEXT");
	vkDestroyDebugReportCallback = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(m_vkInstance, "vkDestroyDebugReportCallbackEXT");
	if (vkCreateDebugReportCallback == nullptr) return false;
	if (vkDestroyDebugReportCallback == nullptr) return false;

	VkDebugReportCallbackCreateInfoEXT debugInfo = {};
	debugInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CREATE_INFO_EXT;
	debugInfo.pNext = nullptr;
	debugInfo.pUserData = nullptr;
	debugInfo.pfnCallback = DebugReportCallback;
	debugInfo.flags =
		VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
		VK_DEBUG_REPORT_DEBUG_BIT_EXT |
		VK_DEBUG_REPORT_ERROR_BIT_EXT |
		VK_DEBUG_REPORT_WARNING_BIT_EXT |
		VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateDebugReportCallback(m_vkInstance, &debugInfo, m_pAllocator->GetAllocationCallbacks(), &m_vkDebugReportCallback));
#endif

	return true;
}

bool CVKInstance::CreateSurface(void* hInstance, void* hWnd)
{
#ifdef PLATFORM_WINDOWS
	VkWin32SurfaceCreateInfoKHR surfaceInfo = {};
	surfaceInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.pNext = nullptr;
	surfaceInfo.flags = 0;
	surfaceInfo.hinstance = (HINSTANCE)hInstance;
	surfaceInfo.hwnd = (HWND)hWnd;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateWin32SurfaceKHR(m_vkInstance, &surfaceInfo, m_pAllocator->GetAllocationCallbacks(), &m_vkSurface));
	return true;
#endif

#ifdef PLATFORM_ANDROID
	VkAndroidSurfaceCreateInfoKHR surfaceInfo = {};
	surfaceInfo.sType = VK_STRUCTURE_TYPE_ANDROID_SURFACE_CREATE_INFO_KHR;
	surfaceInfo.pNext = nullptr;
	surfaceInfo.flags = 0;
	surfaceInfo.window = (ANativeWindow*)hWnd;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateAndroidSurfaceKHR(m_vkInstance, &surfaceInfo, m_pAllocator->GetAllocationCallbacks(), &m_vkSurface));
	return true;
#endif
}

void CVKInstance::DestroyInstance(void)
{
#ifdef DEBUG
	if (vkDestroyDebugReportCallback && m_vkDebugReportCallback) {
		vkDestroyDebugReportCallback(m_vkInstance, m_vkDebugReportCallback, m_pAllocator->GetAllocationCallbacks());
	}

	m_vkDebugReportCallback = VK_NULL_HANDLE;
#endif

	if (m_vkInstance) {
		vkDestroyInstance(m_vkInstance, m_pAllocator->GetAllocationCallbacks());
	}

	m_vkInstance = VK_NULL_HANDLE;
}

void CVKInstance::DestroySurface(void)
{
	if (m_vkSurface) {
		vkDestroySurfaceKHR(m_vkInstance, m_vkSurface, m_pAllocator->GetAllocationCallbacks());
	}

	m_vkSurface = VK_NULL_HANDLE;
}
