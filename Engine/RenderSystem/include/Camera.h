#pragma once
#include "PreHeader.h"


class CALL_API CCamera
{
	friend class CRenderQueue;
	friend class CRenderSystem;
	friend class CPassBase;
	friend class CPassPreZ;
	friend class CPassShadow;
	friend class CPassDefault;
	friend class CPassForwardLighting;
	friend class CPassDeferredLighting;
	friend class CPassBlit;
	friend class CPassCopyColor;
	friend class CPassCopyDepthStencil;
	friend class CPassSSAO;
	friend class CPassBlurBox;
	friend class CPassBlurHorizontal;
	friend class CPassBlurVertical;
	friend class CPassBlendAdd;
	friend class CPassLuminanceThreshold;
	friend class CPassColorGrading;
	friend class CPassFinal;


public:
	CCamera(void);
	virtual ~CCamera(void);


private:
	CRenderQueue* GetRenderQueue(void) const;

private:
	CGfxCamera* GetCamera(void) const;
	CGfxUniformCamera* GetCameraUniform(void) const;

public:
	void SetScissor(float x, float y, float width, float height);
	void SetViewport(float x, float y, float width, float height);
	void SetPerspective(float fovy, float aspect, float zNear, float zFar);
	void SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	void SetLookat(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

private:
	void Update(CTaskPool& taskPool, CTaskGraph& taskGraph, uint32_t mask, bool bComputeLOD) const;


private:
	CRenderQueue* m_pRenderQueue;

private:
	CGfxCamera* m_pCamera;
	CGfxUniformCamera* m_pCameraUniform;
};
