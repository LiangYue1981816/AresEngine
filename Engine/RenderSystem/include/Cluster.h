#pragma once
#include "PreHeader.h"


class CALL_API CCluster
{
	friend class CRenderSystem;
	friend class CPassForwardShading;
	friend class CPassDeferredShading;
	friend class CPassTileDeferredShading;


public:
	CCluster(void);
	virtual ~CCluster(void);


private:
	const CGfxStorageBufferPtr GetClusterBuffer(void) const;
	const CGfxStorageBufferPtr GetCullLightListBuffer(void) const;

public:
	void SetCamera(CCamera* pCamera);

private:
	void Update(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CCamera* m_pCamera;

private:
	CGfxShader* m_pShaderCompute;
	CGfxPipelineCompute* m_pPipelineCompute;
	CGfxDescriptorSetPtr m_ptrDescriptorSet;

private:
	CGfxStorageBufferPtr m_ptrClusterBuffer;
	CGfxStorageBufferPtr m_ptrFullLightListBuffer;
	CGfxStorageBufferPtr m_ptrCullLightListBuffer;
};
