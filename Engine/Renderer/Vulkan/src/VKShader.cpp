#include "VKRenderer.h"


CVKShader::CVKShader(CVKDevice* pDevice, uint32_t name, const uint32_t* words, size_t numWords, shader_kind kind)
	: CGfxShader(name)
	, m_pDevice(pDevice)

	, m_kind(kind)

	, m_vkShader(VK_NULL_HANDLE)
{
	m_spriv.Create(words, numWords, 310);

	VkShaderModuleCreateInfo shaderCreateInfo = {};
	shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderCreateInfo.pNext = nullptr;
	shaderCreateInfo.flags = 0;
	shaderCreateInfo.codeSize = sizeof(uint32_t) * numWords;
	shaderCreateInfo.pCode = words;
	CALL_VK_FUNCTION_ASSERT(vkCreateShaderModule(m_pDevice->GetDevice(), &shaderCreateInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkShader));
}

CVKShader::~CVKShader(void)
{
	vkDestroyShaderModule(m_pDevice->GetDevice(), m_vkShader, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
}

void CVKShader::Release(void)
{
	delete this;
}

VkShaderModule CVKShader::GetShader(void) const
{
	return m_vkShader;
}

bool CVKShader::IsValid(void) const
{
	return m_vkShader != VK_NULL_HANDLE;
}

shader_kind CVKShader::GetKind(void) const
{
	return m_kind;
}

const CGfxSprivCross& CVKShader::GetSprivCross(void) const
{
	return m_spriv;
}
