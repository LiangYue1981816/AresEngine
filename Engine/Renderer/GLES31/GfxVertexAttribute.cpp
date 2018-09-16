#include "Engine.h"
#include "GfxRenderer.h"
#include "GfxVertexAttribute.h"


struct ATTRIBUTE {
	GLuint flag;
	GLuint size;
	GLuint components;
	GLuint location;
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
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0, 4, 4, 8,  "inInstanceTransformMatrixCol0" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1, 4, 4, 9,  "inInstanceTransformMatrixCol1" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2, 4, 4, 10, "inInstanceTransformMatrixCol2" },
	{ INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3, 4, 4, 11, "inInstanceTransformMatrixCol3" },
};

static GLuint GetStride(GLuint format, const ATTRIBUTE *attributes, GLuint count)
{
	GLuint stride = 0;

	for (GLuint indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (format & attributes[indexAttribute].flag) {
			stride += attributes[indexAttribute].components * attributes[indexAttribute].size;
		}
	}

	return stride;
}

static GLuint GetAttributeSize(GLuint attribute, const ATTRIBUTE *attributes, GLuint count)
{
	for (GLuint indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) {
			return attributes[indexAttribute].size;
		}
	}

	return 0;
}

static GLuint GetAttributeOffset(GLuint format, GLuint attribute, const ATTRIBUTE *attributes, GLuint count)
{
	GLuint offset = 0;

	for (GLuint indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) return offset;
		if (format & attributes[indexAttribute].flag) offset += attributes[indexAttribute].components * attributes[indexAttribute].size;
	}

	return -1;
}

static GLuint GetAttributeComponents(GLuint attribute, const ATTRIBUTE *attributes, GLuint count)
{
	for (GLuint indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) {
			return attributes[indexAttribute].components;
		}
	}

	return 0;
}

static GLuint GetAttributeLocation(GLuint attribute, const ATTRIBUTE *attributes, GLuint count)
{
	for (GLuint indexAttribute = 0; indexAttribute < count; indexAttribute++) {
		if (attribute == attributes[indexAttribute].flag) {
			return attributes[indexAttribute].location;
		}
	}

	return -1;
}

GLuint GetVertexStride(GLuint format)
{
	return GetStride(format, vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
}

GLuint GetVertexAttributeSize(GLuint attribute)
{
	return GetAttributeSize(attribute, vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
}

GLuint GetVertexAttributeOffset(GLuint format, GLuint attribute)
{
	return GetAttributeOffset(format, attribute, vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
}

GLuint GetVertexAttributeComponents(GLuint attribute)
{
	return GetAttributeComponents(attribute, vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
}

GLuint GetVertexAttributeLocation(GLuint attribute)
{
	return GetAttributeLocation(attribute, vertexAttributes, VERTEX_ATTRIBUTE_COUNT);
}

GLuint GetInstanceStride(GLuint format)
{
	return GetStride(format, instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);
}

GLuint GetInstanceAttributeSize(GLuint attribute)
{
	return GetAttributeSize(attribute, instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);
}

GLuint GetInstanceAttributeOffset(GLuint format, GLuint attribute)
{
	return GetAttributeOffset(format, attribute, instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);
}

GLuint GetInstanceAttributeComponents(GLuint attribute)
{
	return GetAttributeComponents(attribute, instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);
}

GLuint GetInstanceAttributeLocation(GLuint attribute)
{
	return GetAttributeLocation(attribute, instanceAttributes, INSTANCE_ATTRIBUTE_COUNT);
}
