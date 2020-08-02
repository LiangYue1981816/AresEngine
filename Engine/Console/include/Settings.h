#pragma once
#include "PreHeader.h"


#define Settings() CSettings::GetInstance()


class CSettings
{
	friend class CEngine;


private:
	typedef struct Variable {
		eastl::string description;
		float value;
	} Variable;


public:
	static CSettings* GetInstance(void);


private:
	CSettings(void);
	virtual ~CSettings(void);


public:
	void Register(const char* szName, const char* szDescription, float defaultValue);
	void GetNames(eastl::vector<eastl::string>& names) const;

	bool SetValue(const char* szName, float value);
	float GetValue(const char* szName) const;
	eastl::string GetDescription(const char* szName) const;

public:
	void Load(const char* szFileName);
	void Save(const char* szFileName) const;


private:
	eastl::map<eastl::string, Variable> m_variables;

private:
	static CSettings* pInstance;
};
