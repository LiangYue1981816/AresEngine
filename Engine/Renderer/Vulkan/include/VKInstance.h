#pragma once
#include "VKRenderer.h"


class CVKInstance
{
	friend class CVKRenderer;


private:
	CVKInstance(void* hInstance, void* hWnd);
	virtual ~CVKInstance(void);


public:
	static void SetLastError(VkResult err);
	static VkResult GetLastError(void);

public:
	VkInstance GetInstance(void) const;
	VkSurfaceKHR GetSurface(void) const;
	CVKAllocator* GetAllocator(void) const;

private:
	bool EnumerateInstanceLayerProperties(eastl::vector<const char*>& enabledInstanceLayers) const;
	bool EnumerateInstanceExtensionProperties(eastl::vector<const char*>& enabledInstanceExtensions) const;

	bool CreateInstance(const eastl::vector<const char*>& enabledInstanceLayers, const eastl::vector<const char*>& enabledInstanceExtensions);
	bool CreateSurface(void* hInstance, void* hWnd);

	void DestroyInstance(void);
	void DestroySurface(void);


private:
	CVKAllocator* m_pAllocator;

private:
	VkInstance m_vkInstance;
	VkSurfaceKHR m_vkSurface;

#ifdef DEBUG
	VkDebugReportCallbackEXT m_vkDebugReportCallback;
#endif

private:
	static VkResult vkErrorCode;
};
