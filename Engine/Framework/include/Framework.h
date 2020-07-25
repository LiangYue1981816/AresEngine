#pragma once
#include "PreHeader.h"


class CFramework
{
public:
	static CFramework* GetInstance(void);
	static void Create(void);
	static void Destroy(void);


private:
	CFramework(void);
	virtual ~CFramework(void);


private:
	static CFramework* pInstance;
};
