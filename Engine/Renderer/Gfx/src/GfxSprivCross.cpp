#include "GfxHeader.h"


CGfxSprivCross::CGfxSprivCross(void)
{

}

CGfxSprivCross::~CGfxSprivCross(void)
{
	Destroy();
}

bool CGfxSprivCross::Create(const uint32_t *words, size_t numWords, uint32_t version)
{
	Destroy();

	spirv_cross::CompilerGLSL::Options options;
	options.version = version;
	options.es = true;
	options.vertex.fixup_clipspace = false;

	spirv_cross::CompilerGLSL compiler(words, numWords);
	compiler.set_options(options);
	m_source = compiler.compile();

	const spirv_cross::ShaderResources shaderResources = compiler.get_shader_resources();
	{
		for (const auto &itPushConstant : shaderResources.push_constant_buffers) {
			const std::vector<spirv_cross::BufferRange> ranges = compiler.get_active_buffer_ranges(itPushConstant.id);
			for (uint32_t index = 0; index < ranges.size(); index++) {
				const std::string member = compiler.get_member_name(itPushConstant.base_type_id, ranges[index].index);
				const std::string name = itPushConstant.name + "." + member;
				m_pushConstantRanges[name].offset = ranges[index].offset;
				m_pushConstantRanges[name].range = ranges[index].range;
			}
		}

		for (const auto &itUniform : shaderResources.uniform_buffers) {
			if (compiler.get_type(itUniform.base_type_id).basetype == spirv_cross::SPIRType::Struct) {
				const uint32_t set = compiler.get_decoration(itUniform.id, spv::DecorationDescriptorSet);
				const uint32_t binding = compiler.get_decoration(itUniform.id, spv::DecorationBinding);
				m_uniformBlockBindings[itUniform.name][set] = binding;
			}
		}

		for (const auto &itSampledImage : shaderResources.sampled_images) {
			if (compiler.get_type(itSampledImage.base_type_id).basetype == spirv_cross::SPIRType::SampledImage) {
				const uint32_t set = compiler.get_decoration(itSampledImage.id, spv::DecorationDescriptorSet);
				const uint32_t binding = compiler.get_decoration(itSampledImage.id, spv::DecorationBinding);
				m_sampledImageBindings[itSampledImage.name][set] = binding;
			}
		}

		for (const auto &itSubpassInput : shaderResources.subpass_inputs) {
			if (compiler.get_type(itSubpassInput.base_type_id).basetype == spirv_cross::SPIRType::Image) {
				const uint32_t set = compiler.get_decoration(itSubpassInput.id, spv::DecorationDescriptorSet);
				const uint32_t binding = compiler.get_decoration(itSubpassInput.id, spv::DecorationBinding);
				m_inputAttachmentBindings[itSubpassInput.name][set] = binding;
			}
		}
	}

	return true;
}

void CGfxSprivCross::Destroy(void)
{
	m_source = "";
	m_pushConstantRanges.clear();
	m_uniformBlockBindings.clear();
	m_sampledImageBindings.clear();
	m_inputAttachmentBindings.clear();
}

const eastl::string& CGfxSprivCross::GetSource(void) const
{
	return m_source;
}

const eastl::unordered_map<eastl::string, BufferRange>& CGfxSprivCross::GetPushConstantRanges(void) const
{
	return m_pushConstantRanges;
}

const eastl::unordered_map<eastl::string, eastl::unordered_map<uint32_t, uint32_t>>& CGfxSprivCross::GetUniformBlockBindings(void) const
{
	return m_uniformBlockBindings;
}

const eastl::unordered_map<eastl::string, eastl::unordered_map<uint32_t, uint32_t>>& CGfxSprivCross::GetSampledImageBindings(void) const
{
	return m_sampledImageBindings;
}

const eastl::unordered_map<eastl::string, eastl::unordered_map<uint32_t, uint32_t>>& CGfxSprivCross::GetInputAttachmentBindings(void) const
{
	return m_inputAttachmentBindings;
}
