#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxRenderTexture : public CGfxResource
{
public:
	CGfxRenderTexture(uint32_t name)
		: m_name(name)
	{

	}
	virtual ~CGfxRenderTexture(void)
	{

	}

	uint32_t GetName(void) const
	{
		return m_name;
	}


public:
	virtual HANDLE GetTexture(void) const = 0;

	virtual GfxPixelFormat GetFormat(void) const = 0;
	virtual GfxTextureType GetType(void) const = 0;

	virtual int GetWidth(void) const = 0;
	virtual int GetHeight(void) const = 0;
	virtual int GetSamples(void) const = 0;

public:
	virtual bool Create(HANDLE hExternalTexture, GfxPixelFormat format, int width, int height, int samples = 1) = 0;
	virtual bool Create(GfxPixelFormat format, int width, int height, int samples = 1, bool bTransient = false) = 0;
	virtual void Destroy(void) = 0;


private:
	uint32_t m_name;
};
