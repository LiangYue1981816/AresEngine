#version 310 es
layout (local_size_x = 4) in;

precision mediump float;
#include "engine.inc"
#include "common.inc"


USE_SCENE_DATA_STORAGE;
USE_TRANSFER_SCENE_DATA_STORAGE;

void main()
{
	uint indexWork = uint(gl_GlobalInvocationID.x);

	uint numWorkGroup = uint(gl_NumWorkGroups.x);
	uint numWorkGroupSize = uint(gl_WorkGroupSize.x);
	uint numWorkTotalSize = uint(numWorkGroup * numWorkGroupSize);

	uint numTransferTotal = uint(transferSceneData.data.length());
	uint numTransferAlign = ((numTransferTotal + numWorkGroupSize - uint(1)) / numWorkGroupSize) * numWorkGroupSize;

	if (numWorkTotalSize < numTransferTotal) {
		uint numTransferWork = numTransferAlign / numWorkTotalSize;

		uint indexBegin = indexWork * numTransferWork;
		if (indexBegin >= numTransferTotal) {
			return;
		}

		uint indexEnd = indexBegin + numTransferWork;
		if (indexEnd > numTransferTotal) {
			indexEnd = numTransferTotal;
		}

		for (uint index = indexBegin; index < indexEnd; index++) {
			sceneData.data[transferSceneData.data[index].index] = transferSceneData.data[index].data;
		}
	}
	else if (indexWork < numTransferTotal) {
		sceneData.data[transferSceneData.data[indexWork].index] = transferSceneData.data[indexWork].data;
	}

	barrier();
}
