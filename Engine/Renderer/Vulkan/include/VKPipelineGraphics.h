#pragma once
#include "VKRenderer.h"


class CVKPipelineGraphics : public CGfxPipelineGraphics
{
	friend class CVKPipelineGraphicsManager;


private:
	CVKPipelineGraphics(CVKDevice* pDevice, VkPipelineCache vkPipelineCache, uint32_t name);
	virtual ~CVKPipelineGraphics(void);


public:
	const VkPipelineLayout GetPipelineLayout(void) const;
	const CGfxDescriptorLayoutPtr GetDescriptorLayout(int indexDescriptorSet) const;

public:
	const uint32_t GetInputAttachmentName(uint32_t inputAttachmentIndex) const;
	const bool IsCompatibleVertexFormat(uint32_t binding, uint32_t format) const;

public:
	bool Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding);
	void Destroy(void);

public:
	bool IsTransparency(void) const;

public:
	void Bind(VkCommandBuffer vkCommandBuffer);

	void Uniform1i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0) const;
	void Uniform2i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1) const;
	void Uniform3i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2) const;
	void Uniform4i(VkCommandBuffer vkCommandBuffer, uint32_t name, int v0, int v1, int v2, int v3) const;

	void Uniform1f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0) const;
	void Uniform2f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1) const;
	void Uniform3f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2) const;
	void Uniform4f(VkCommandBuffer vkCommandBuffer, uint32_t name, float v0, float v1, float v2, float v3) const;

	void Uniform1iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const;
	void Uniform2iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const;
	void Uniform3iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const;
	void Uniform4iv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const int* value) const;

	void Uniform1fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;
	void Uniform2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;
	void Uniform3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;
	void Uniform4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;

	void UniformMatrix2fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;
	void UniformMatrix3fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;
	void UniformMatrix4fv(VkCommandBuffer vkCommandBuffer, uint32_t name, int count, const float* value) const;


private:
	PipelineState m_state;
	CVKPipeline* m_pPipeline;

private:
	CVKDevice* m_pDevice;
};
