#pragma once
#include "GfxRenderer.h"


struct ATTRIBUTE {
	uint32_t flag;
	uint32_t size;
	uint32_t components;
	uint32_t location;
	const char *name;
};


CALL_API void SetVertexAttributes(const ATTRIBUTE *pAttributes, uint32_t count);
CALL_API uint32_t GetVertexAttributeCount(void);
CALL_API uint32_t GetVertexStride(uint32_t format);
CALL_API uint32_t GetVertexAttributeSize(uint32_t attribute);
CALL_API uint32_t GetVertexAttributeOffset(uint32_t format, uint32_t attribute);
CALL_API uint32_t GetVertexAttributeComponents(uint32_t attribute);
CALL_API uint32_t GetVertexAttributeLocation(uint32_t attribute);

CALL_API void SetInstanceAttributes(const ATTRIBUTE *pAttributes, uint32_t count);
CALL_API uint32_t GetInstanceAttributeCount(void);
CALL_API uint32_t GetInstanceStride(uint32_t format);
CALL_API uint32_t GetInstanceAttributeSize(uint32_t attribute);
CALL_API uint32_t GetInstanceAttributeOffset(uint32_t format, uint32_t attribute);
CALL_API uint32_t GetInstanceAttributeComponents(uint32_t attribute);
CALL_API uint32_t GetInstanceAttributeLocation(uint32_t attribute);
