#include "VKRenderer.h"


CVKShader::CVKShader(CVKDevice *pDevice, uint32_t name)
	: CGfxShader(name)
	, m_name(name)
	, m_pDevice(pDevice)

	, m_kind(-1)
	, m_vkShader(VK_NULL_HANDLE)
	, m_pShaderCompiler(nullptr)
{

}

CVKShader::~CVKShader(void)
{
	Destroy();
}

uint32_t CVKShader::GetName(void) const
{
	return m_name;
}

uint32_t CVKShader::GetKind(void) const
{
	return m_kind;
}

HANDLE CVKShader::GetShader(void) const
{
	return m_vkShader;
}

const spirv_cross::CompilerGLSL* CVKShader::GetShaderCompiler(void) const
{
	return m_pShaderCompiler;
}

bool CVKShader::Create(const uint32_t *words, size_t numWords, shader_kind kind)
{
	Destroy();

	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.pNext = nullptr;
	createInfo.flags = 0;
	createInfo.codeSize = sizeof(uint32_t)*numWords;
	createInfo.pCode = words;
	CALL_VK_FUNCTION_RETURN_BOOL(vkCreateShaderModule(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkShader));

	m_kind = kind;
	m_pShaderCompiler = new spirv_cross::CompilerGLSL(words, numWords);

	return true;
}

void CVKShader::Destroy(void)
{
	if (m_vkShader) {
		vkDestroyShaderModule(m_pDevice->GetDevice(), m_vkShader, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	if (m_pShaderCompiler) {
		delete m_pShaderCompiler;
	}

	m_kind = -1;
	m_vkShader = VK_NULL_HANDLE;
	m_pShaderCompiler = nullptr;
}

bool CVKShader::IsValid(void) const
{
	return m_vkShader != VK_NULL_HANDLE;
}
