#pragma once
#include "PreHeader.h"


#define Settings() CSettings::GetInstance()


class CALL_API CSettings
{
	friend class CEngine;


public:
	static CSettings* GetInstance(void);


private:
	CSettings(void);
	virtual ~CSettings(void);


public:
	void SetValue(const char* szName, float value);
	float GetValue(const char* szName);


private:
	eastl::map<eastl::string, float> m_values;

private:
	static CSettings* pInstance;
};
