#pragma once
#include "GfxCommandBuffer.h"


class CGfxCommandBindInputTexture : public CGfxCommandBase
{
public:
	CGfxCommandBindInputTexture(const char *szName, uint32_t texture, uint32_t minFilter, uint32_t magFilter, uint32_t addressMode)
		: m_name(szName)
		, m_texture(texture)
		, m_minFilter(minFilter)
		, m_magFilter(magFilter)
		, m_addressMode(addressMode)
	{

	}
	virtual ~CGfxCommandBindInputTexture(void)
	{

	}

public:
	virtual void Execute(void) const
	{
		CGfxProfilerSample(CGfxProfiler::SAMPLE_TYPE_COMMAND_BIND_INPUTTEXTURE, "CommandBindInputTexture");
		{
			Renderer()->BindInputTexture(m_name.c_str(), m_texture, m_minFilter, m_magFilter, m_addressMode);
		}
	}


private:
	eastl::string m_name;
	uint32_t m_texture;
	uint32_t m_minFilter;
	uint32_t m_magFilter;
	uint32_t m_addressMode;
};
