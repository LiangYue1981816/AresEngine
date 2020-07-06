#version 310 es

#ifdef VERTEX_SHADER
precision highp float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM
USE_SCENE_DATA_STORAGE

// Output
// ...

// Descriptor
// ...

void main()
{
	mat4 worldMatrix = sceneData.instances[int(inInstanceIndex)].transformMatrix;
	vec3 worldPosition = (worldMatrix * vec4(inPosition.xyz, 1.0)).xyz;

	gl_Position = cameraProjectionViewMatrix * vec4(worldPosition, 1.0);
}
#endif

#ifdef FRAGMENT_SHADER
precision highp float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_ENGINE_UNIFORM

// Input
// ...

// Output
layout (location = 0) out vec4 outFragColor;

// Descriptor
// ...

void main()
{
	outFragColor = PackHDR(vec3(1.0, 0.0, 0.0));
}
#endif
