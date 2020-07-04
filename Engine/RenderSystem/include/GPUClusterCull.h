#pragma once
#include "PreHeader.h"


class CALL_API CGPUCluster
{
	friend class CRenderSystem;


private:
	CGPUCluster(void);
	virtual ~CGPUCluster(void);


public:
	const CGfxStorageBufferPtr GetClusterBuffer(void) const;
	const CGfxStorageBufferPtr GetCullLightListBuffer(void) const;

public:
	void SetCamera(CCamera* pCamera);

private:
	void Compute(CTaskPool& taskPool, CTaskGraph& taskGraph, CGfxCommandBufferPtr ptrCommandBuffer);


private:
	CCamera* m_pCamera;

private:
	CGfxShader* m_pClusterShaderCompute;
	CGfxPipelineCompute* m_pClusterPipelineCompute;
	CGfxDescriptorSetPtr m_ptrClusterDescriptorSet;

private:
	CGfxShader* m_pClusterCullShaderCompute;
	CGfxPipelineCompute* m_pClusterCullPipelineCompute;
	CGfxDescriptorSetPtr m_ptrClusterCullDescriptorSet;

private:
	CGfxStorageBufferPtr m_ptrClusterBuffer;
	CGfxStorageBufferPtr m_ptrFullLightListBuffer;
	CGfxStorageBufferPtr m_ptrCullLightListBuffer;
};
