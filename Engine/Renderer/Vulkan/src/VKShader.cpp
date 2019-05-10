#include "VKRenderer.h"


CVKShader::CVKShader(CVKDevice* pDevice, uint32_t name)
	: CGfxShader(name)
	, m_pDevice(pDevice)

	, m_kind(-1)
	, m_vkShader(VK_NULL_HANDLE)
{
	ASSERT(m_pDevice);
}

CVKShader::~CVKShader(void)
{
	Destroy();
}

VkShaderModule CVKShader::GetShader(void) const
{
	ASSERT(m_vkShader);
	return m_vkShader;
}

uint32_t CVKShader::GetKind(void) const
{
	ASSERT(m_kind != -1);
	return m_kind;
}

const CGfxSprivCross& CVKShader::GetSprivCross(void) const
{
	return m_spriv;
}

bool CVKShader::Create(const uint32_t* words, size_t numWords, shader_kind kind)
{
	Destroy();
	{
		do {
			m_kind = kind;
			m_spriv.Create(words, numWords, 310);

			VkShaderModuleCreateInfo createInfo = {};
			createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			createInfo.pNext = nullptr;
			createInfo.flags = 0;
			createInfo.codeSize = sizeof(uint32_t) * numWords;
			createInfo.pCode = words;
			CALL_VK_FUNCTION_BREAK(vkCreateShaderModule(m_pDevice->GetDevice(), &createInfo, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks(), &m_vkShader));

			return true;
		} while (false);
	}
	Destroy();
	return false;
}

void CVKShader::Destroy(void)
{
	if (m_vkShader) {
		vkDestroyShaderModule(m_pDevice->GetDevice(), m_vkShader, m_pDevice->GetInstance()->GetAllocator()->GetAllocationCallbacks());
	}

	m_kind = -1;
	m_vkShader = VK_NULL_HANDLE;
}

bool CVKShader::IsValid(void) const
{
	return m_vkShader != VK_NULL_HANDLE;
}
