#pragma once
#include "PreHeader.h"


class CPassBase
{
	friend class CCamera;
	friend class CRenderSystem;


protected:
	CPassBase(CRenderSystem* pRenderSystem);
	virtual ~CPassBase(void);


protected:
	CCamera* m_pCamera;
	CRenderSystem* m_pRenderSystem;
};
