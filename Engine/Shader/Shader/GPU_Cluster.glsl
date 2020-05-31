#version 310 es
layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;

precision mediump float;
#include "engine.inc"
#include "common.inc"

USE_CAMERA_UNIFORM
USE_CLUSTER_DATA_STORAGE

// Output
// ...

// Descriptor
// ...

void main()
{
	highp int indexTile = int(gl_WorkGroupID.z * gl_NumWorkGroups.x * gl_NumWorkGroups.y + gl_WorkGroupID.y * gl_NumWorkGroups.x + gl_WorkGroupID.x);

	highp vec2 minScreenPosition = vec2(float(gl_WorkGroupID.x + uint(0)), float(gl_WorkGroupID.y + uint(0))) / vec2(float(gl_NumWorkGroups.x), float(gl_NumWorkGroups.y));
	highp vec2 maxScreenPosition = vec2(float(gl_WorkGroupID.x + uint(1)), float(gl_WorkGroupID.y + uint(1))) / vec2(float(gl_NumWorkGroups.x), float(gl_NumWorkGroups.y));
	highp float minDepthValue = -cameraZNear * pow(cameraZFar / cameraZNear, float(gl_WorkGroupID.z + uint(0)) / float(gl_NumWorkGroups.z));
	highp float maxDepthValue = -cameraZNear * pow(cameraZFar / cameraZNear, float(gl_WorkGroupID.z + uint(1)) / float(gl_NumWorkGroups.z));

	highp vec3 minViewPosition = ScreenToViewPosition(minScreenPosition, 0.0, cameraProjectionInverseMatrix).xyz;
	highp vec3 maxViewPosition = ScreenToViewPosition(maxScreenPosition, 0.0, cameraProjectionInverseMatrix).xyz;

	highp vec3 minViewPositionNear = LineIntersectionToZPlane(vec3(0.0), minViewPosition, minDepthValue);
	highp vec3 maxViewPositionNear = LineIntersectionToZPlane(vec3(0.0), maxViewPosition, minDepthValue);
	highp vec3 minViewPositionFar = LineIntersectionToZPlane(vec3(0.0), minViewPosition, maxDepthValue);
	highp vec3 maxViewPositionFar = LineIntersectionToZPlane(vec3(0.0), maxViewPosition, maxDepthValue);

	highp vec3 minAABBPosition = min(min(minViewPositionNear, maxViewPositionNear), min(minViewPositionFar, maxViewPositionFar));
	highp vec3 maxAABBPosition = max(max(minViewPositionNear, maxViewPositionNear), max(minViewPositionFar, maxViewPositionFar));

	SetCluster(indexTile, minAABBPosition, maxAABBPosition);
}
