#version 310 es
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_SCENE_DATA_STORAGE;
USE_TRANSFER_SCENE_DATA_STORAGE;

// Output
// ...

// Descriptor
// ...

void main()
{
	uint indexWork = uint(gl_GlobalInvocationID.x);
	sceneData.data[transferSceneData.data[indexWork].index.x] = transferSceneData.data[indexWork].data;
	barrier();
}
