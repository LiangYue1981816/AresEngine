#pragma once
#include "GfxRenderer.h"


class CALL_API CGfxHelper
{
public:
	static bool IsFormatDepthOnly(GfxPixelFormat format);
	static bool IsFormatStencilOnly(GfxPixelFormat format);
	static bool IsFormatDepthStencil(GfxPixelFormat format);
	static bool IsFormatDepthOrStencil(GfxPixelFormat format);
};
