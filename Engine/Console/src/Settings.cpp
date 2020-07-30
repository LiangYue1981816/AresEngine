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

void CSettings::GetNames(eastl::vector<eastl::string>& names) const
{
	names.clear();

	for (const auto& itVariable : m_variables) {
		names.emplace_back(itVariable.first);
	}
}

bool CSettings::SetValue(const char* szName, float value)
{
	const auto& itVariable = m_variables.find(szName);

	if (itVariable != m_variables.end()) {
		itVariable->second.value = value;
		return true;
	}
	else {
		return false;
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

eastl::string CSettings::GetDescription(const char* szName) const
{
	const auto& itVariable = m_variables.find(szName);

	if (itVariable != m_variables.end()) {
		return itVariable->second.description;
	}
	else {
		return "";
	}
}

void CSettings::Load(const char* szFileName)
{
	if (FILE *pFile = fopen(szFileName, "rb")) {
		while (!feof(pFile)) {
			char szName[256];
			float value;

			fscanf(pFile, "%s = %f", szName, &value);
			SetValue(szName, value);
		}
		fclose(pFile);
	}
}
