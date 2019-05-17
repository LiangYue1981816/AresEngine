#include "GfxHeader.h"


bool CGfxHelper::IsFormatDepthOnly(GfxPixelFormat format)
{
	switch ((int)format) {
	case GFX_PIXELFORMAT_D16_UNORM_PACK16:
	case GFX_PIXELFORMAT_D24_UNORM_PACK32:
	case GFX_PIXELFORMAT_D32_SFLOAT_PACK32:
		return true;

	default:
		return false;
	}
}

bool CGfxHelper::IsFormatStencilOnly(GfxPixelFormat format)
{
	switch ((int)format) {
	case GFX_PIXELFORMAT_S8_UINT_PACK8:
		return true;

	default:
		return false;
	}
}

bool CGfxHelper::IsFormatDepthStencil(GfxPixelFormat format)
{
	switch ((int)format) {
	case GFX_PIXELFORMAT_D16_UNORM_S8_UINT_PACK32:
	case GFX_PIXELFORMAT_D24_UNORM_S8_UINT_PACK32:
	case GFX_PIXELFORMAT_D32_SFLOAT_S8_UINT_PACK64:
		return true;

	default:
		return false;
	}
}

bool CGfxHelper::IsFormatDepthOrStencil(GfxPixelFormat format)
{
	return IsFormatStencilOnly(format) || IsFormatDepthOnly(format) || IsFormatDepthStencil(format);
}
