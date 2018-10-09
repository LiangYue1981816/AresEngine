#pragma once
#include "Engine.h"
#include "RenderSolutionBase.h"


class CRenderSolutionDefault : public CRenderSolutionBase
{
	friend class CEngine;


private:
	CRenderSolutionDefault(int screenWidth, int screenHeight);
	virtual ~CRenderSolutionDefault(void);


public:
	virtual void SetEnableMSAA(bool bEnable, int width, int height, int samples = 4);

private:
	virtual void Render(int indexQueue);
	virtual void Present(int indexQueue);
	virtual void Clearup(int indexQueue);


private:
	bool m_bEnableMSAA;
	CGfxTexture2DPtr m_ptrColorTextureMSAA;
	CGfxFrameBufferPtr m_ptrFrameBufferMSAA;
};
