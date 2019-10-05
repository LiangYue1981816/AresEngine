#include "ConsoleHeader.h"


CSettings::CSettings(void)
{

}

CSettings::~CSettings(void)
{

}

void CSettings::SetValue(const char* szName, float value)
{
	m_values[szName] = value;
}

float CSettings::GetValue(const char* szName)
{
	return m_values[szName];
}
