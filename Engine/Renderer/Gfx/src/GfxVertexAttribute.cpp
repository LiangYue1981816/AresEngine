#include "GfxRenderer.h"


struct ATTRIBUTE {
	uint32_t flag;
	uint32_t size;
	uint32_t components;
	uint32_t location;
	const char *name;
};

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

CALL_API uint32_t GetVertexStride(uint32_t format)
{
	return GetStride(format, vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
}

CALL_API uint32_t GetVertexAttributeSize(uint32_t attribute)
{
	return GetAttributeSize(attribute, vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
}

CALL_API uint32_t GetVertexAttributeOffset(uint32_t format, uint32_t attribute)
{
	return GetAttributeOffset(format, attribute, vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
}

CALL_API uint32_t GetVertexAttributeComponents(uint32_t attribute)
{
	return GetAttributeComponents(attribute, vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
}

CALL_API uint32_t GetVertexAttributeLocation(uint32_t attribute)
{
	return GetAttributeLocation(attribute, vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
}

CALL_API uint32_t GetInstanceStride(uint32_t format)
{
	return GetStride(format, instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);
}

CALL_API uint32_t GetInstanceAttributeSize(uint32_t attribute)
{
	return GetAttributeSize(attribute, instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);
}

CALL_API uint32_t GetInstanceAttributeOffset(uint32_t format, uint32_t attribute)
{
	return GetAttributeOffset(format, attribute, instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);
}

CALL_API uint32_t GetInstanceAttributeComponents(uint32_t attribute)
{
	return GetAttributeComponents(attribute, instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);
}

CALL_API uint32_t GetInstanceAttributeLocation(uint32_t attribute)
{
	return GetAttributeLocation(attribute, instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);
}
