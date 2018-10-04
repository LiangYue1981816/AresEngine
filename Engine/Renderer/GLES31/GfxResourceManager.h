#pragma once
#include "GfxRenderer.h"


class CGfxResourceManager
{
	friend class CGfxRenderer;


private:
	CGfxResourceManager(void);
	virtual ~CGfxResourceManager(void);


public:
	void SetResourcePath(const char *szPathName, const char *szExtName);
	const char* GetResourceFullName(const char *szFileName) const;


private:
	eastl::unordered_map<uint32_t, eastl::string> m_strResourceFullNames;
};
