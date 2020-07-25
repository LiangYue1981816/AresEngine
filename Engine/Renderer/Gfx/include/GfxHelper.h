#pragma once
#include "PreHeader.h"


class CGfxHelper
{
public:
	static bool IsFormatDepthOnly(GfxPixelFormat format);
	static bool IsFormatStencilOnly(GfxPixelFormat format);
	static bool IsFormatDepthAndStencil(GfxPixelFormat format);
	static bool IsFormatDepthOrStencil(GfxPixelFormat format);
	static bool IsFormatColor(GfxPixelFormat format);
};
