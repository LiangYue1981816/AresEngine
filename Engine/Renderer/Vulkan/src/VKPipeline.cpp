#include "VKRenderer.h"


CVKPipeline::CVKPipeline(CVKDevice *pDevice)
	: m_pDevice(pDevice)

	, m_vkPipeline(VK_NULL_HANDLE)
	, m_vkPipelineLayout(VK_NULL_HANDLE)
{
	m_shaders[vertex_shader] = {};
	m_shaders[vertex_shader].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	m_shaders[vertex_shader].pNext = nullptr;
	m_shaders[vertex_shader].flags = 0;
	m_shaders[vertex_shader].stage = VK_SHADER_STAGE_VERTEX_BIT;
	m_shaders[vertex_shader].module = VK_NULL_HANDLE;
	m_shaders[vertex_shader].pName = "main";
	m_shaders[vertex_shader].pSpecializationInfo = nullptr;

	m_shaders[fragment_shader] = {};
	m_shaders[fragment_shader].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	m_shaders[fragment_shader].pNext = nullptr;
	m_shaders[fragment_shader].flags = 0;
	m_shaders[fragment_shader].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	m_shaders[fragment_shader].module = VK_NULL_HANDLE;
	m_shaders[fragment_shader].pName = "main";
	m_shaders[fragment_shader].pSpecializationInfo = nullptr;

	m_shaders[compute_shader] = {};
	m_shaders[compute_shader].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	m_shaders[compute_shader].pNext = nullptr;
	m_shaders[compute_shader].flags = 0;
	m_shaders[compute_shader].stage = VK_SHADER_STAGE_COMPUTE_BIT;
	m_shaders[compute_shader].module = VK_NULL_HANDLE;
	m_shaders[compute_shader].pName = "main";
	m_shaders[compute_shader].pSpecializationInfo = nullptr;
}

CVKPipeline::~CVKPipeline(void)
{

}
