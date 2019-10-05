#pragma once
#include "PreHeader.h"


class CALL_API CSettings
{
public:
	CSettings(void);
	virtual ~CSettings(void);


public:
	void SetValue(const char* szName, float value);
	float GetValue(const char* szName);


private:
	eastl::map<eastl::string, float> m_values;
};
