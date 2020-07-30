#include "GLES3Renderer.h"


CGLES3Pipeline::CGLES3Pipeline(void)
	: m_program(0)
	, m_pShaders{ nullptr }
{
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS] = GLES3Renderer()->NewDescriptorLayout(DESCRIPTOR_SET_PASS);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_MATPASS] = GLES3Renderer()->NewDescriptorLayout(DESCRIPTOR_SET_MATPASS);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_MESHDRAW] = GLES3Renderer()->NewDescriptorLayout(DESCRIPTOR_SET_MESHDRAW);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_INPUTATTACHMENT] = GLES3Renderer()->NewDescriptorLayout(DESCRIPTOR_SET_INPUTATTACHMENT);
}

CGLES3Pipeline::~CGLES3Pipeline(void)
{
	Destroy();
}

const CGfxDescriptorLayoutPtr CGLES3Pipeline::GetDescriptorLayout(int indexDescriptorSet) const
{
	if (indexDescriptorSet >= 0 && indexDescriptorSet < DESCRIPTOR_SET_COUNT) {
		return m_ptrDescriptorLayouts[indexDescriptorSet];
	}
	else {
		return nullptr;
	}
}

const uint32_t CGLES3Pipeline::GetInputAttachmentName(int indexInputAttachment) const
{
	const auto& itInputAttachmentName = m_inputAttachmentNames.find(indexInputAttachment);

	if (itInputAttachmentName != m_inputAttachmentNames.end()) {
		return itInputAttachmentName->second;
	}
	else {
		return INVALID_HASHNAME;
	}
}

bool CGLES3Pipeline::CreateProgram(const CGLES3Shader* pVertexShader, const CGLES3Shader* pFragmentShader, const CGLES3Shader* pComputeShader)
{
	m_pShaders[vertex_shader] = (CGLES3Shader*)pVertexShader;
	m_pShaders[fragment_shader] = (CGLES3Shader*)pFragmentShader;
	m_pShaders[compute_shader] = (CGLES3Shader*)pComputeShader;

	m_program = glCreateProgram();
	{
		for (int indexShader = 0; indexShader < compute_shader - vertex_shader + 1; indexShader++) {
			if (m_pShaders[indexShader] && m_pShaders[indexShader]->IsValid()) {
				glAttachShader(m_program, m_pShaders[indexShader]->GetShader());
			}
		}

		glLinkProgram(m_program);
	}
	CHECK_GL_ERROR_ASSERT();

	GLint success;
	glGetProgramiv(m_program, GL_LINK_STATUS, &success);

	if (success == GL_FALSE) {
		GLsizei length = 0;
		static char szError[32 * 1024] = { 0 };

		glGetProgramInfoLog(m_program, sizeof(szError), &length, szError);

		LogOutput(LOG_ERROR, nullptr, "Program Link Error:\n");
		LogOutput(LOG_ERROR, nullptr, "%s\n", szError);

		return false;
	}

	return true;
}

bool CGLES3Pipeline::CreateLayouts(void)
{
	for (int indexShader = 0; indexShader < compute_shader - vertex_shader + 1; indexShader++) {
		if (m_pShaders[indexShader] && m_pShaders[indexShader]->IsValid()) {
			for (const auto& itPushConstant : m_pShaders[indexShader]->GetSprivCross().GetPushConstantRanges()) {
				SetUniformLocation(itPushConstant.first.c_str());
			}

			for (const auto& itUniformBlock : m_pShaders[indexShader]->GetSprivCross().GetUniformBlockBindings()) {
				SetUniformBlockBinding(itUniformBlock.first.c_str(), itUniformBlock.second.binding);
				m_ptrDescriptorLayouts[itUniformBlock.second.set]->SetUniformBlockBinding(HashValue(itUniformBlock.first.c_str()), itUniformBlock.second.binding);
			}

			for (const auto& itStorageBlock : m_pShaders[indexShader]->GetSprivCross().GetStorageBlockBindings()) {
				SetStorageBlockBinding(itStorageBlock.first.c_str(), itStorageBlock.second.binding);
				m_ptrDescriptorLayouts[itStorageBlock.second.set]->SetStorageBlockBinding(HashValue(itStorageBlock.first.c_str()), itStorageBlock.second.binding);
			}

			for (const auto& itStorageImage : m_pShaders[indexShader]->GetSprivCross().GetStorageImageBindings()) {
				SetStorageImageBinding(itStorageImage.first.c_str(), itStorageImage.second.binding);
				m_ptrDescriptorLayouts[itStorageImage.second.set]->SetStorageImageBinding(HashValue(itStorageImage.first.c_str()), itStorageImage.second.binding);
			}

			for (const auto& itSampledImage : m_pShaders[indexShader]->GetSprivCross().GetSampledImageBindings()) {
				SetSampledImageBinding(itSampledImage.first.c_str());
				m_ptrDescriptorLayouts[itSampledImage.second.set]->SetSampledImageBinding(HashValue(itSampledImage.first.c_str()), itSampledImage.second.binding);
			}

			for (const auto& itInputAttachment : m_pShaders[indexShader]->GetSprivCross().GetInputAttachmentBindings()) {
				SetInputAttachmentBinding(itInputAttachment.first.c_str(), itInputAttachment.second.indexInputAttachment);
				m_ptrDescriptorLayouts[itInputAttachment.second.set]->SetInputAttachmentBinding(HashValue(itInputAttachment.first.c_str()), itInputAttachment.second.binding);
			}
		}
	}

	CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS]->Create());
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrDescriptorLayouts[DESCRIPTOR_SET_MATPASS]->Create());
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrDescriptorLayouts[DESCRIPTOR_SET_MESHDRAW]->Create());
	CALL_BOOL_FUNCTION_RETURN_BOOL(m_ptrDescriptorLayouts[DESCRIPTOR_SET_INPUTATTACHMENT]->Create());

	return true;
}

bool CGLES3Pipeline::CreateVertexFormat(int vertexBinding, int instanceBinding)
{
	ASSERT(m_pShaders[vertex_shader]);
	ASSERT(m_pShaders[vertex_shader]->IsValid());

	const eastl::vector<eastl::string>& vertexAttributes = m_pShaders[vertex_shader]->GetSprivCross().GetVertexAttributes();

	uint32_t vertexFormat = 0;
	uint32_t instanceFormat = 0;

	for (const auto& itVertexAttribute : vertexAttributes) {
		vertexFormat |= GetVertexAttribute(itVertexAttribute.c_str());
		instanceFormat |= GetInstanceAttribute(itVertexAttribute.c_str());
	}

	if (vertexFormat) {
		m_vertexFormats[vertexBinding] = vertexFormat;
	}

	if (instanceFormat) {
		m_vertexFormats[instanceBinding] = instanceFormat;
	}

	return true;
}

bool CGLES3Pipeline::Create(const CGfxShader* pComputeShader)
{
	Destroy();
	{
		do {
			ASSERT(pComputeShader);
			ASSERT(pComputeShader->IsValid());
			ASSERT(pComputeShader->GetKind() == compute_shader);

			CALL_BOOL_FUNCTION_BREAK(CreateProgram(nullptr, nullptr, (const CGLES3Shader*)pComputeShader));
			CALL_BOOL_FUNCTION_BREAK(CreateLayouts());

			return true;
		} while (false);
	}
	Destroy();
	return true;
}

bool CGLES3Pipeline::Create(const CGfxRenderPass* pRenderPass, const CGfxShader* pVertexShader, const CGfxShader* pFragmentShader, const PipelineState& state, int indexSubpass, int vertexBinding, int instanceBinding)
{
	Destroy();
	{
		do {
			ASSERT(pRenderPass);
			ASSERT(pVertexShader);
			ASSERT(pVertexShader->IsValid());
			ASSERT(pVertexShader->GetKind() == vertex_shader);
			ASSERT(pFragmentShader);
			ASSERT(pFragmentShader->IsValid());
			ASSERT(pFragmentShader->GetKind() == fragment_shader);

			CALL_BOOL_FUNCTION_BREAK(CreateProgram((const CGLES3Shader*)pVertexShader, (const CGLES3Shader*)pFragmentShader, nullptr));
			CALL_BOOL_FUNCTION_BREAK(CreateLayouts());
			CALL_BOOL_FUNCTION_BREAK(CreateVertexFormat(vertexBinding, instanceBinding));

			return true;
		} while (false);
	}
	Destroy();
	return true;
}

void CGLES3Pipeline::Destroy(void)
{
	if (m_program) {
		glDeleteProgram(m_program);
	}

	m_program = 0;

	m_pShaders[vertex_shader] = nullptr;
	m_pShaders[fragment_shader] = nullptr;
	m_pShaders[compute_shader] = nullptr;

	m_uniformLocations.clear();
	m_uniformBlockBindings.clear();
	m_storageBlockBindings.clear();
	m_storageImageBindings.clear();
	m_sampledImageBindings.clear();
	m_sampledImageTextureUnits.clear();
	m_inputAttachmentNames.clear();
	m_vertexFormats.clear();

	m_ptrDescriptorLayouts[DESCRIPTOR_SET_PASS]->Destroy(true);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_MATPASS]->Destroy(true);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_MESHDRAW]->Destroy(true);
	m_ptrDescriptorLayouts[DESCRIPTOR_SET_INPUTATTACHMENT]->Destroy(true);
}

void CGLES3Pipeline::SetUniformLocation(const char* szName)
{
	uint32_t name = HashValue(szName);

	if (m_uniformLocations.find(name) == m_uniformLocations.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_uniformLocations[name] = location;
		}

		CHECK_GL_ERROR_ASSERT();
	}
}

void CGLES3Pipeline::SetUniformBlockBinding(const char* szName, uint32_t binding)
{
	uint32_t name = HashValue(szName);

	if (m_uniformBlockBindings.find(name) == m_uniformBlockBindings.end()) {
		uint32_t indexBinding = glGetProgramResourceIndex(m_program, GL_UNIFORM_BLOCK, szName);

		if (indexBinding != GL_INVALID_INDEX) {
			m_uniformBlockBindings[name] = binding;
		}

		CHECK_GL_ERROR_ASSERT();
	}
}

void CGLES3Pipeline::SetStorageBlockBinding(const char* szName, uint32_t binding)
{
	uint32_t name = HashValue(szName);

	if (m_storageBlockBindings.find(name) == m_storageBlockBindings.end()) {
		uint32_t indexBinding = glGetProgramResourceIndex(m_program, GL_SHADER_STORAGE_BLOCK, szName);

		if (indexBinding != GL_INVALID_INDEX) {
			m_storageBlockBindings[name] = binding;
		}
	}
}

void CGLES3Pipeline::SetStorageImageBinding(const char* szName, uint32_t binding)
{
	uint32_t name = HashValue(szName);

	if (m_storageImageBindings.find(name) == m_storageImageBindings.end()) {
		m_storageImageBindings[name] = binding;
	}
}

void CGLES3Pipeline::SetSampledImageBinding(const char* szName)
{
	uint32_t name = HashValue(szName);

	if (m_sampledImageBindings.find(name) == m_sampledImageBindings.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_sampledImageTextureUnits[name] = m_sampledImageBindings.size();
			m_sampledImageBindings[name] = location;
		}

		CHECK_GL_ERROR_ASSERT();
	}
}

void CGLES3Pipeline::SetInputAttachmentBinding(const char* szName, int indexInputAttachment)
{
	uint32_t name = HashValue(szName);

	if (m_sampledImageBindings.find(name) == m_sampledImageBindings.end()) {
		uint32_t location = glGetUniformLocation(m_program, szName);

		if (location != GL_INVALID_INDEX) {
			m_sampledImageTextureUnits[name] = m_sampledImageBindings.size();
			m_sampledImageBindings[name] = location;
			m_inputAttachmentNames[indexInputAttachment] = name;
		}

		CHECK_GL_ERROR_ASSERT();
	}
}

bool CGLES3Pipeline::IsCompatibleVertexFormat(uint32_t binding, uint32_t format) const
{
	const auto& itFormat = m_vertexFormats.find(binding);

	if (itFormat != m_vertexFormats.end()) {
		return itFormat->second == format && itFormat->second != 0;
	}
	else {
		return false;
	}
}

void CGLES3Pipeline::Bind(const PipelineState* pState) const
{
	ASSERT(m_program);

	if (pState) {
		GLBindState(pState);
	}

	GLUseProgram(m_program);
	CHECK_GL_ERROR_ASSERT();
}

bool CGLES3Pipeline::BindDescriptorSet(const CGfxDescriptorSetPtr ptrDescriptorSet) const
{
	ASSERT(ptrDescriptorSet);

	const CGfxDescriptorLayoutPtr ptrDescriptorLayout = ptrDescriptorSet->GetDescriptorLayout();
	ASSERT(ptrDescriptorLayout);
	ASSERT(ptrDescriptorLayout->GetSetIndex() >= 0 && ptrDescriptorLayout->GetSetIndex() < DESCRIPTOR_SET_COUNT);

	if (ptrDescriptorLayout->IsCompatible(m_ptrDescriptorLayouts[ptrDescriptorLayout->GetSetIndex()]) == false) {
		return false;
	}

	for (const auto& itUniformBlock : m_uniformBlockBindings) {
		if (const DescriptorBufferInfo* pDescriptorBufferInfo = ptrDescriptorSet->GetDescriptorBufferInfo(itUniformBlock.first)) {
			((CGLES3UniformBuffer*)pDescriptorBufferInfo->ptrUniformBuffer.GetPointer())->Bind(itUniformBlock.second, pDescriptorBufferInfo->offset, pDescriptorBufferInfo->range);
		}
	}

	for (const auto& itStorageBlock : m_storageBlockBindings) {
		if (const DescriptorBufferInfo* pDescriptorBufferInfo = ptrDescriptorSet->GetDescriptorBufferInfo(itStorageBlock.first)) {
			((CGLES3StorageBuffer*)pDescriptorBufferInfo->ptrStorageBuffer.GetPointer())->Bind(itStorageBlock.second, pDescriptorBufferInfo->offset, pDescriptorBufferInfo->range);
		}
	}

	for (const auto& itStorageImage : m_storageImageBindings) {
		if (const DescriptorImageInfo* pDescriptorImageInfo = ptrDescriptorSet->GetDescriptorImageInfo(itStorageImage.first)) {
			if (pDescriptorImageInfo->ptrImage2D) {
				((CGLES3Texture2D*)pDescriptorImageInfo->ptrImage2D.GetPointer())->BindImageTexture(itStorageImage.second, pDescriptorImageInfo->level);
				continue;
			}

			if (pDescriptorImageInfo->ptrImage2DArray) {
				((CGLES3Texture2DArray*)pDescriptorImageInfo->ptrImage2DArray.GetPointer())->BindImageTexture(itStorageImage.second, pDescriptorImageInfo->level, pDescriptorImageInfo->layer);
				continue;
			}

			if (pDescriptorImageInfo->ptrImageCubemap) {
				((CGLES3TextureCubemap*)pDescriptorImageInfo->ptrImageCubemap.GetPointer())->BindImageTexture(itStorageImage.second, pDescriptorImageInfo->level, pDescriptorImageInfo->layer);
				continue;
			}

			if (pDescriptorImageInfo->ptrImageRenderTexture) {
				((CGLES3RenderTexture*)pDescriptorImageInfo->ptrImageRenderTexture.GetPointer())->BindImageTexture(itStorageImage.second);
				continue;
			}
		}
	}

	for (const auto& itSampledImage : m_sampledImageBindings) {
		if (const DescriptorImageInfo* pDescriptorImageInfo = ptrDescriptorSet->GetDescriptorImageInfo(itSampledImage.first)) {
			const auto& itTextureUnit = m_sampledImageTextureUnits.find(itSampledImage.first);

			if (pDescriptorImageInfo->ptrTexture2D) {
				GLUniform1i(itSampledImage.second, itTextureUnit->second);
				((CGLES3Sampler*)pDescriptorImageInfo->pSampler)->Bind(itTextureUnit->second);
				((CGLES3Texture2D*)pDescriptorImageInfo->ptrTexture2D.GetPointer())->Bind(itTextureUnit->second);
				continue;
			}

			if (pDescriptorImageInfo->ptrTexture2DArray) {
				GLUniform1i(itSampledImage.second, itTextureUnit->second);
				((CGLES3Sampler*)pDescriptorImageInfo->pSampler)->Bind(itTextureUnit->second);
				((CGLES3Texture2DArray*)pDescriptorImageInfo->ptrTexture2DArray.GetPointer())->Bind(itTextureUnit->second);
				continue;
			}

			if (pDescriptorImageInfo->ptrTextureCubemap) {
				GLUniform1i(itSampledImage.second, itTextureUnit->second);
				((CGLES3Sampler*)pDescriptorImageInfo->pSampler)->Bind(itTextureUnit->second);
				((CGLES3TextureCubemap*)pDescriptorImageInfo->ptrTextureCubemap.GetPointer())->Bind(itTextureUnit->second);
				continue;
			}

			if (pDescriptorImageInfo->ptrRenderTexture) {
				GLUniform1i(itSampledImage.second, itTextureUnit->second);
				((CGLES3Sampler*)pDescriptorImageInfo->pSampler)->Bind(itTextureUnit->second);
				((CGLES3RenderTexture*)pDescriptorImageInfo->ptrRenderTexture.GetPointer())->Bind(itTextureUnit->second);
				continue;
			}

			if (pDescriptorImageInfo->ptrInputAttachmentTexture) {
				GLUniform1i(itSampledImage.second, itTextureUnit->second);
				((CGLES3Sampler*)pDescriptorImageInfo->pSampler)->Bind(itTextureUnit->second);
				((CGLES3RenderTexture*)pDescriptorImageInfo->ptrInputAttachmentTexture.GetPointer())->Bind(itTextureUnit->second);
				continue;
			}
		}
	}

	return true;
}

void CGLES3Pipeline::Uniform1i(uint32_t name, int v0) const
{
	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform1i(itLocation->second, v0);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform2i(uint32_t name, int v0, int v1) const
{
	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform2i(itLocation->second, v0, v1);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform3i(uint32_t name, int v0, int v1, int v2) const
{
	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform3i(itLocation->second, v0, v1, v2);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform4i(uint32_t name, int v0, int v1, int v2, int v3) const
{
	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform4i(itLocation->second, v0, v1, v2, v3);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform1f(uint32_t name, float v0) const
{
	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform1f(itLocation->second, v0);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform2f(uint32_t name, float v0, float v1) const
{
	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform2f(itLocation->second, v0, v1);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform3f(uint32_t name, float v0, float v1, float v2) const
{
	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform3f(itLocation->second, v0, v1, v2);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform4f(uint32_t name, float v0, float v1, float v2, float v3) const
{
	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform4f(itLocation->second, v0, v1, v2, v3);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform1iv(uint32_t name, int count, const int* value) const
{
	ASSERT(value);

	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform1iv(itLocation->second, count, value);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform2iv(uint32_t name, int count, const int* value) const
{
	ASSERT(value);

	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform2iv(itLocation->second, count, value);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform3iv(uint32_t name, int count, const int* value) const
{
	ASSERT(value);

	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform3iv(itLocation->second, count, value);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform4iv(uint32_t name, int count, const int* value) const
{
	ASSERT(value);

	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform4iv(itLocation->second, count, value);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform1fv(uint32_t name, int count, const float* value) const
{
	ASSERT(value);

	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform1fv(itLocation->second, count, value);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform2fv(uint32_t name, int count, const float* value) const
{
	ASSERT(value);

	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform2fv(itLocation->second, count, value);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform3fv(uint32_t name, int count, const float* value) const
{
	ASSERT(value);

	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform3fv(itLocation->second, count, value);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::Uniform4fv(uint32_t name, int count, const float* value) const
{
	ASSERT(value);

	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniform4fv(itLocation->second, count, value);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::UniformMatrix2fv(uint32_t name, int count, const float* value) const
{
	ASSERT(value);

	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniformMatrix2fv(itLocation->second, count, value);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::UniformMatrix3fv(uint32_t name, int count, const float* value) const
{
	ASSERT(value);

	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniformMatrix3fv(itLocation->second, count, value);
	}

	CHECK_GL_ERROR_ASSERT();
}

void CGLES3Pipeline::UniformMatrix4fv(uint32_t name, int count, const float* value) const
{
	ASSERT(value);

	const auto& itLocation = m_uniformLocations.find(name);

	if (itLocation != m_uniformLocations.end()) {
		GLUniformMatrix4fv(itLocation->second, count, value);
	}

	CHECK_GL_ERROR_ASSERT();
}
