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

void CSettings::Register(const char* szName, const char* szDescription, float defaultValue)
{
	m_variables[szName].description = szDescription;
	m_variables[szName].value = defaultValue;
}

void CSettings::SetValue(const char* szName, float value)
{
	const auto& itVariable = m_variables.find(szName);

	if (itVariable != m_variables.end()) {
		itVariable->second.value = value;
	}
}

float CSettings::GetValue(const char* szName) const
{
	const auto& itVariable = m_variables.find(szName);

	if (itVariable != m_variables.end()) {
		return itVariable->second.value;
	}
	else {
		return 0.0f;
	}
}
