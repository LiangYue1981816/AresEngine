#pragma once
#include "GfxRenderer.h"


#define VERTEX_ATTRIBUTE_POSITION                0x00000001
#define VERTEX_ATTRIBUTE_NORMAL                  0x00000002
#define VERTEX_ATTRIBUTE_BINORMAL                0x00000004
#define VERTEX_ATTRIBUTE_COLOR                   0x00000008
#define VERTEX_ATTRIBUTE_TEXCOORD0               0x00000010
#define VERTEX_ATTRIBUTE_TEXCOORD1               0x00000020
#define VERTEX_ATTRIBUTE_COUNT                   6

#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0 0x00000001
#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1 0x00000002
#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2 0x00000004
#define INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3 0x00000008
#define INSTANCE_ATTRIBUTE_COUNT                 4

#define INSTANCE_ATTRIBUTE_TRANSFORM (INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL0 | INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL1 | INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL2 | INSTANCE_ATTRIBUTE_TRANSFORM_MATRIX_COL3)


extern uint32_t GetVertexStride(uint32_t format);
extern uint32_t GetVertexAttributeSize(uint32_t attribute);
extern uint32_t GetVertexAttributeOffset(uint32_t format, uint32_t attribute);
extern uint32_t GetVertexAttributeComponents(uint32_t attribute);
extern uint32_t GetVertexAttributeLocation(uint32_t attribute);

extern uint32_t GetInstanceStride(uint32_t format);
extern uint32_t GetInstanceAttributeSize(uint32_t attribute);
extern uint32_t GetInstanceAttributeOffset(uint32_t format, uint32_t attribute);
extern uint32_t GetInstanceAttributeComponents(uint32_t attribute);
extern uint32_t GetInstanceAttributeLocation(uint32_t attribute);
