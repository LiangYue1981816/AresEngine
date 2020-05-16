#version 310 es
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_SCENE_DATA_STORAGE;
USE_CLUSTER_DATA_STORAGE;
USE_FULL_LIGHT_LIST_DATA_STORAGE;
USE_CULL_LIGHT_LIST_DATA_STORAGE;

// Output
// ...

// Descriptor

void main()
{
	barrier();
}
