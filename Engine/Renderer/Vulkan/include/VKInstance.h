#pragma once
#include "VKRenderer.h"


class CVKInstance
{
	friend class CVKRenderer;


private:
	CVKInstance(void);
	virtual ~CVKInstance(void);


public:
	static void SetLastError(VkResult err);
	static VkResult GetLastError(void);

private:
	bool EnumerateInstanceLayerProperties(eastl::vector<const char *> &enabledInstanceLayers) const;
	bool EnumerateInstanceExtensionProperties(eastl::vector<const char *> &enabledInstanceExtensions) const;

	bool CreateInstance(const eastl::vector<const char *> &enabledInstanceLayers, const eastl::vector<const char *> &enabledInstanceExtensions);
	bool CreateDebugReportCallback(void);
	void DestroyInstance(void);
	void DestroyDebugReportCallback(void);

public:
	VkInstance GetInstance(void) const;
	CVKAllocator* GetAllocator(void) const;


private:
	VkInstance m_vkInstance;

#ifdef DEBUG
	VkDebugReportCallbackEXT m_vkDebugReportCallback;
#endif

private:
	CVKAllocator *m_pAllocator;

private:
	static VkResult vkErrorCode;
};
