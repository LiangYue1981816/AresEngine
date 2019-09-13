#include "GfxHeader.h"

#include "ThirdParty/spirv-cross/spirv_cfg.hpp"
#include "ThirdParty/spirv-cross/spirv_msl.hpp"
#include "ThirdParty/spirv-cross/spirv_glsl.hpp"


CGfxSprivCross::CGfxSprivCross(void)
{

}

CGfxSprivCross::~CGfxSprivCross(void)
{

}

const eastl::string& CGfxSprivCross::Create(const uint32_t* words, size_t numWords, uint32_t version)
{
	spirv_cross::CompilerGLSL::Options options;
	options.version = version;
	options.es = true;
	options.vertex.fixup_clipspace = true;

	spirv_cross::CompilerGLSL compiler(words, numWords);
	compiler.set_common_options(options);
	m_source = compiler.compile().c_str();

	const spirv_cross::ShaderResources shaderResources = compiler.get_shader_resources();
	{
		m_vertexAttributes.clear();
		m_pushConstantRanges.clear();
		m_uniformBlockBindings.clear();
		m_sampledImageBindings.clear();
		m_inputAttachmentBindings.clear();

		for (const auto& itVertexAttribute : shaderResources.stage_inputs) {
			m_vertexAttributes.emplace_back(itVertexAttribute.name.c_str());
		}

		for (const auto& itPushConstant : shaderResources.push_constant_buffers) {
			const spirv_cross::SmallVector<spirv_cross::BufferRange> ranges = compiler.get_active_buffer_ranges(itPushConstant.id);
			for (int indexRange = 0; indexRange < ranges.size(); indexRange++) {
				const std::string member = compiler.get_member_name(itPushConstant.base_type_id, ranges[indexRange].index);
				const std::string name = itPushConstant.name + "." + member;
				m_pushConstantRanges[name.c_str()].offset = ranges[indexRange].offset;
				m_pushConstantRanges[name.c_str()].range = ranges[indexRange].range;
			}
		}

		for (const auto& itUniform : shaderResources.uniform_buffers) {
			if (compiler.get_type(itUniform.base_type_id).basetype == spirv_cross::SPIRType::Struct) {
				m_uniformBlockBindings[itUniform.name.c_str()].set = compiler.get_decoration(itUniform.id, spv::DecorationDescriptorSet);
				m_uniformBlockBindings[itUniform.name.c_str()].binding = compiler.get_decoration(itUniform.id, spv::DecorationBinding);
			}
		}

		for (const auto& itSampledImage : shaderResources.sampled_images) {
			if (compiler.get_type(itSampledImage.base_type_id).basetype == spirv_cross::SPIRType::SampledImage) {
				m_sampledImageBindings[itSampledImage.name.c_str()].set = compiler.get_decoration(itSampledImage.id, spv::DecorationDescriptorSet);
				m_sampledImageBindings[itSampledImage.name.c_str()].binding = compiler.get_decoration(itSampledImage.id, spv::DecorationBinding);
			}
		}

		for (const auto& itSubpassInput : shaderResources.subpass_inputs) {
			if (compiler.get_type(itSubpassInput.base_type_id).basetype == spirv_cross::SPIRType::Image) {
				m_inputAttachmentBindings[itSubpassInput.name.c_str()].set = compiler.get_decoration(itSubpassInput.id, spv::DecorationDescriptorSet);
				m_inputAttachmentBindings[itSubpassInput.name.c_str()].binding = compiler.get_decoration(itSubpassInput.id, spv::DecorationBinding);
				m_inputAttachmentBindings[itSubpassInput.name.c_str()].inputAttachmentIndex = compiler.get_decoration(itSubpassInput.id, spv::DecorationInputAttachmentIndex);
			}
		}
	}

	return m_source;
}

const eastl::string& CGfxSprivCross::GetSource(void) const
{
	return m_source;
}

const eastl::vector<eastl::string>& CGfxSprivCross::GetVertexAttributes(void) const
{
	return m_vertexAttributes;
}

const eastl::unordered_map<eastl::string, PushConstantRange>& CGfxSprivCross::GetPushConstantRanges(void) const
{
	return m_pushConstantRanges;
}

const eastl::unordered_map<eastl::string, DescriptorSetBinding>& CGfxSprivCross::GetUniformBlockBindings(void) const
{
	return m_uniformBlockBindings;
}

const eastl::unordered_map<eastl::string, DescriptorSetBinding>& CGfxSprivCross::GetSampledImageBindings(void) const
{
	return m_sampledImageBindings;
}

const eastl::unordered_map<eastl::string, InputAttachmentBinding>& CGfxSprivCross::GetInputAttachmentBindings(void) const
{
	return m_inputAttachmentBindings;
}
