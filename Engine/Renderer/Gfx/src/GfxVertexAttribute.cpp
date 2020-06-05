#include "GfxHeader.h"


static const ATTRIBUTE* pVertexAttributes = nullptr;
static uint32_t numVertexAttributes = 0;

static const ATTRIBUTE* pInstanceAttributes = nullptr;
static uint32_t numInstanceAttributes = 0;


static uint32_t GetSize(uint32_t type)
{
	switch (type) {
	case GFX_DATA_SINT8:   return 1;
	case GFX_DATA_UINT8:   return 1;
	case GFX_DATA_SINT16:  return 2;
	case GFX_DATA_UINT16:  return 2;
	case GFX_DATA_SINT32:  return 4;
	case GFX_DATA_UINT32:  return 4;
	case GFX_DATA_FLOAT16: return 2;
	case GFX_DATA_FLOAT32: return 4;
	default:               return 0;
	}
}

static uint32_t GetStride(uint32_t format, const ATTRIBUTE* attributes, uint32_t count)
{
	uint32_t stride = 0;

	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (format & attributes[indexAttribute].flag) {
			stride += attributes[indexAttribute].components * GetSize(attributes[indexAttribute].type);
		}
	}

	return stride;
}

static uint32_t GetAttribute(const char* szName, const ATTRIBUTE* attributes, uint32_t count)
{
	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (strcmp(attributes[indexAttribute].name, szName) == 0) {
			return attributes[indexAttribute].flag;
		}
	}

	return 0;
}

static uint32_t GetAttributeSize(uint32_t attribute, const ATTRIBUTE* attributes, uint32_t count)
{
	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) {
			return GetSize(attributes[indexAttribute].type);
		}
	}

	return 0;
}

static uint32_t GetAttributeOffset(uint32_t format, uint32_t attribute, const ATTRIBUTE* attributes, uint32_t count)
{
	uint32_t offset = 0;

	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) return offset;
		if (format & attributes[indexAttribute].flag) offset += attributes[indexAttribute].components * GetSize(attributes[indexAttribute].type);
	}

	return -1;
}

static uint32_t GetAttributeComponents(uint32_t attribute, const ATTRIBUTE* attributes, uint32_t count)
{
	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) {
			return attributes[indexAttribute].components;
		}
	}

	return 0;
}

static uint32_t GetAttributeLocation(uint32_t attribute, const ATTRIBUTE* attributes, uint32_t count)
{
	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) {
			return attributes[indexAttribute].location;
		}
	}

	return -1;
}

static uint32_t GetAttributeType(uint32_t attribute, const ATTRIBUTE* attributes, uint32_t count)
{
	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) {
			return attributes[indexAttribute].type;
		}
	}

	return -1;
}

static uint32_t GetAttributeFormat(uint32_t attribute, const ATTRIBUTE* attributes, uint32_t count)
{
	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) {
			return attributes[indexAttribute].format;
		}
	}

	return -1;
}

static uint32_t GetAttributeNormalized(uint32_t attribute, const ATTRIBUTE* attributes, uint32_t count)
{
	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) {
			return attributes[indexAttribute].normalized;
		}
	}

	return FALSE;
}


CALL_API void SetVertexAttributes(const ATTRIBUTE* pAttributes, uint32_t count)
{
	pVertexAttributes = pAttributes;
	numVertexAttributes = count;
}

CALL_API uint32_t GetVertexAttributeCount(void)
{
	return numVertexAttributes;
}

CALL_API uint32_t GetVertexStride(uint32_t vertexFormat)
{
	return GetStride(vertexFormat, pVertexAttributes, numVertexAttributes);
}

CALL_API uint32_t GetVertexAttribute(const char* szName)
{
	return GetAttribute(szName, pVertexAttributes, numVertexAttributes);
}

CALL_API uint32_t GetVertexAttributeSize(uint32_t attribute)
{
	return GetAttributeSize(attribute, pVertexAttributes, numVertexAttributes);
}

CALL_API uint32_t GetVertexAttributeOffset(uint32_t vertexFormat, uint32_t attribute)
{
	return GetAttributeOffset(vertexFormat, attribute, pVertexAttributes, numVertexAttributes);
}

CALL_API uint32_t GetVertexAttributeComponents(uint32_t attribute)
{
	return GetAttributeComponents(attribute, pVertexAttributes, numVertexAttributes);
}

CALL_API uint32_t GetVertexAttributeLocation(uint32_t attribute)
{
	return GetAttributeLocation(attribute, pVertexAttributes, numVertexAttributes);
}

CALL_API uint32_t GetVertexAttributeType(uint32_t attribute)
{
	return GetAttributeType(attribute, pVertexAttributes, numVertexAttributes);
}

CALL_API uint32_t GetVertexAttributeFormat(uint32_t attribute)
{
	return GetAttributeFormat(attribute, pVertexAttributes, numVertexAttributes);
}

CALL_API uint32_t GetVertexAttributeNormalized(uint32_t attribute)
{
	return GetAttributeNormalized(attribute, pVertexAttributes, numVertexAttributes);
}


CALL_API void SetInstanceAttributes(const ATTRIBUTE* pAttributes, uint32_t count)
{
	pInstanceAttributes = pAttributes;
	numInstanceAttributes = count;
}

CALL_API uint32_t GetInstanceAttributeCount(void)
{
	return numInstanceAttributes;
}

CALL_API uint32_t GetInstanceStride(uint32_t instanceFormat)
{
	return GetStride(instanceFormat, pInstanceAttributes, numInstanceAttributes);
}

CALL_API uint32_t GetInstanceAttribute(const char* szName)
{
	return GetAttribute(szName, pInstanceAttributes, numInstanceAttributes);
}

CALL_API uint32_t GetInstanceAttributeSize(uint32_t attribute)
{
	return GetAttributeSize(attribute, pInstanceAttributes, numInstanceAttributes);
}

CALL_API uint32_t GetInstanceAttributeOffset(uint32_t instanceFormat, uint32_t attribute)
{
	return GetAttributeOffset(instanceFormat, attribute, pInstanceAttributes, numInstanceAttributes);
}

CALL_API uint32_t GetInstanceAttributeComponents(uint32_t attribute)
{
	return GetAttributeComponents(attribute, pInstanceAttributes, numInstanceAttributes);
}

CALL_API uint32_t GetInstanceAttributeLocation(uint32_t attribute)
{
	return GetAttributeLocation(attribute, pInstanceAttributes, numInstanceAttributes);
}

CALL_API uint32_t GetInstanceAttributeType(uint32_t attribute)
{
	return GetAttributeType(attribute, pInstanceAttributes, numInstanceAttributes);
}

CALL_API uint32_t GetInstanceAttributeFormat(uint32_t attribute)
{
	return GetAttributeFormat(attribute, pInstanceAttributes, numInstanceAttributes);
}

CALL_API uint32_t GetInstanceAttributeNormalized(uint32_t attribute)
{
	return GetAttributeNormalized(attribute, pInstanceAttributes, numInstanceAttributes);
}
