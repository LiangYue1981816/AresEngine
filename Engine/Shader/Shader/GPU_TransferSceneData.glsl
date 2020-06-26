#version 310 es
layout (local_size_x = 128, local_size_y = 1, local_size_z = 1) in;

precision highp float;
#include "engine.inc"
#include "common.inc"

USE_SCENE_DATA_STORAGE
USE_TRANSFER_SCENE_DATA_STORAGE
USE_TRANSFER_SCENE_DATA_INDEX_STORAGE

// Output
// ...

// Descriptor
layout(push_constant) uniform PushConstantParam {
	int numTransfers;
} Param;

void main()
{
	int numTransfers = Param.numTransfers;
	int indexTransfer = int(gl_GlobalInvocationID.x);

	if (indexTransfer < numTransfers) {
		SetInstance(GetTransferIndex(indexTransfer), GetTransferInstance(indexTransfer));
	}
}
