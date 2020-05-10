#version 310 es
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

precision mediump float;
#include "engine.inc"
#include "common.inc"

// Output
// ...

// Descriptor

void main()
{
	barrier();
}
