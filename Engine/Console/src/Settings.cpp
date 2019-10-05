#include "ConsoleHeader.h"


CSettings* CSettings::pInstance = nullptr;
CSettings* CSettings::GetInstance(void)
{
	return pInstance;
}

CSettings::CSettings(void)
{
	pInstance = this;
}

CSettings::~CSettings(void)
{
	pInstance = nullptr;
}

void CSettings::SetValue(const char* szName, float value)
{
	m_values[szName] = value;
}

float CSettings::GetValue(const char* szName)
{
	return m_values[szName];
}
