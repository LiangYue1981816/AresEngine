#pragma once
#include "VKRenderer.h"


class CVKPipeline
{
	friend class CVKPipelineCompute;
	friend class CVKPipelineGraphics;


private:
	CVKPipeline(CVKDevice* pDevice, VkPipelineCache vkPipelineCache, const CGfxShader* pComputeShader);
	CVKPipeline(CVKDevice* pDevice, VkPipelineCache vkPipelineCache, const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding);
	virtual ~CVKPipeline(void);


private:
	const VkPipelineLayout GetPipelineLayout(void) const;
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(int indexDescriptorSet) const;
	const uint32_t GetInputAttachmentName(int indexInputAttachment) const;

private:
	bool CreateLayouts(void);
	bool CreateShaderStages(eastl::vector<VkPipelineShaderStageCreateInfo>& shaders);
	bool CreateVertexInputState(eastl::vector<VkVertexInputBindingDescription>& inputBindingDescriptions, eastl::vector<VkVertexInputAttributeDescription>& inputAttributeDescriptions, int vertexBinding, int instanceBinding);

private:
	bool IsCompatibleVertexFormat(uint32_t binding, uint32_t format) const;


private:
	CGfxShader* m_pShaders[compute_shader - vertex_shader + 1];
	CGfxDescriptorLayoutPtr m_ptrDescriptorLayouts[DESCRIPTOR_SET_COUNT];

	eastl::unordered_map<uint32_t, uint32_t> m_vertexFormats;                 // [binding, format]
	eastl::unordered_map<uint32_t, uint32_t> m_inputAttachmentNames;          // [input attachment index, name]
	eastl::unordered_map<uint32_t, VkPushConstantRange> m_pushConstantRanges; // [name, push constant]

private:
	VkPipeline m_vkPipeline;
	VkPipelineLayout m_vkPipelineLayout;
	VkPipelineCache m_vkPipelineCache;

private:
	CVKDevice* m_pDevice;
};
