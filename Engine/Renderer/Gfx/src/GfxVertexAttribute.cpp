#include "GfxHeader.h"


static const ATTRIBUTE vertexAttributes[VERTEX_ATTRIBUTE_COUNT] = {
	{ VERTEX_ATTRIBUTE_POSITION,  4, 3, 0, "inPosition"  },
	{ VERTEX_ATTRIBUTE_NORMAL,    4, 3, 1, "inNormal"    },
	{ VERTEX_ATTRIBUTE_BINORMAL,  4, 3, 2, "inBinormal"  },
	{ VERTEX_ATTRIBUTE_COLOR,     4, 3, 3, "inColor"     },
	{ VERTEX_ATTRIBUTE_TEXCOORD0, 4, 2, 4, "inTexcoord0" },
	{ VERTEX_ATTRIBUTE_TEXCOORD1, 4, 2, 5, "inTexcoord1" },
};

static const ATTRIBUTE instanceAttributes[INSTANCE_ATTRIBUTE_COUNT] = {
	// Transform
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0, 4, 4, 8,  "inInstanceTransformMatrixCol0" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1, 4, 4, 9,  "inInstanceTransformMatrixCol1" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2, 4, 4, 10, "inInstanceTransformMatrixCol2" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3, 4, 4, 11, "inInstanceTransformMatrixCol3" },

	// PointLight
	{ INSTANCE_ATTRIBUTE_POINTLIGHT_COLOR,      4, 4, 12, "inInstancePointLightColor"       },
	{ INSTANCE_ATTRIBUTE_POINTLIGHT_ATTENUATION,4, 4, 13, "inInstancePointLightAttenuation" },
};


static const ATTRIBUTE *pVertexAttributes = vertexAttributes;
static uint32_t numVertexAttributes = VERTEX_ATTRIBUTE_COUNT;

static const ATTRIBUTE *pInstanceAttributes = instanceAttributes;
static uint32_t numInstanceAttributes = INSTANCE_ATTRIBUTE_COUNT;


static uint32_t GetStride(uint32_t format, const ATTRIBUTE *attributes, uint32_t count)
{
	uint32_t stride = 0;

	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (format & attributes[indexAttribute].flag) {
			stride += attributes[indexAttribute].components * attributes[indexAttribute].size;
		}
	}

	return stride;
}

static uint32_t GetAttributeSize(uint32_t attribute, const ATTRIBUTE *attributes, uint32_t count)
{
	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) {
			return attributes[indexAttribute].size;
		}
	}

	return 0;
}

static uint32_t GetAttributeOffset(uint32_t format, uint32_t attribute, const ATTRIBUTE *attributes, uint32_t count)
{
	uint32_t offset = 0;

	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) return offset;
		if (format & attributes[indexAttribute].flag) offset += attributes[indexAttribute].components * attributes[indexAttribute].size;
	}

	return -1;
}

static uint32_t GetAttributeComponents(uint32_t attribute, const ATTRIBUTE *attributes, uint32_t count)
{
	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) {
			return attributes[indexAttribute].components;
		}
	}

	return 0;
}

static uint32_t GetAttributeLocation(uint32_t attribute, const ATTRIBUTE *attributes, uint32_t count)
{
	for (uint32_t indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) {
			return attributes[indexAttribute].location;
		}
	}

	return -1;
}


CALL_API void SetVertexAttributes(ATTRIBUTE *pAttributes, uint32_t count)
{
	pVertexAttributes = pAttributes;
	numVertexAttributes = count;
}

CALL_API uint32_t GetVertexStride(uint32_t format)
{
	return GetStride(format, pVertexAttributes, numVertexAttributes);
}

CALL_API uint32_t GetVertexAttributeSize(uint32_t attribute)
{
	return GetAttributeSize(attribute, pVertexAttributes, numVertexAttributes);
}

CALL_API uint32_t GetVertexAttributeOffset(uint32_t format, uint32_t attribute)
{
	return GetAttributeOffset(format, attribute, pVertexAttributes, numVertexAttributes);
}

CALL_API uint32_t GetVertexAttributeComponents(uint32_t attribute)
{
	return GetAttributeComponents(attribute, pVertexAttributes, numVertexAttributes);
}

CALL_API uint32_t GetVertexAttributeLocation(uint32_t attribute)
{
	return GetAttributeLocation(attribute, pVertexAttributes, numVertexAttributes);
}


CALL_API void SetInstanceAttributes(ATTRIBUTE *pAttributes, uint32_t count)
{
	pInstanceAttributes = pAttributes;
	numInstanceAttributes = count;
}

CALL_API uint32_t GetInstanceStride(uint32_t format)
{
	return GetStride(format, pInstanceAttributes, numInstanceAttributes);
}

CALL_API uint32_t GetInstanceAttributeSize(uint32_t attribute)
{
	return GetAttributeSize(attribute, pInstanceAttributes, numInstanceAttributes);
}

CALL_API uint32_t GetInstanceAttributeOffset(uint32_t format, uint32_t attribute)
{
	return GetAttributeOffset(format, attribute, pInstanceAttributes, numInstanceAttributes);
}

CALL_API uint32_t GetInstanceAttributeComponents(uint32_t attribute)
{
	return GetAttributeComponents(attribute, pInstanceAttributes, numInstanceAttributes);
}

CALL_API uint32_t GetInstanceAttributeLocation(uint32_t attribute)
{
	return GetAttributeLocation(attribute, pInstanceAttributes, numInstanceAttributes);
}
