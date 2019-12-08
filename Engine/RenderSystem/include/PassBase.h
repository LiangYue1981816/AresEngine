#pragma once
#include "PreHeader.h"


class CALL_API CPassBase
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
