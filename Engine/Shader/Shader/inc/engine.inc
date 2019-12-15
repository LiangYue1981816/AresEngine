#ifndef _ENGINE_INC_
#define _ENGINE_INC_


#ifdef VERTEX_ATTRIBUTE_POSITION
layout (location = 0)                                      in highp vec3 inPosition;
#endif
#ifdef VERTEX_ATTRIBUTE_NORMAL
layout (location = 1)                                      in mediump vec3 inNormal;
#endif
#ifdef VERTEX_ATTRIBUTE_BINORMAL
layout (location = 2)                                      in mediump vec3 inBinormal;
#endif
#ifdef VERTEX_ATTRIBUTE_COLOR
layout (location = 3)                                      in mediump vec3 inColor;
#endif
#ifdef VERTEX_ATTRIBUTE_TEXCOORD0
layout (location = 4)                                      in mediump vec2 inTexcoord0;
#endif
#ifdef VERTEX_ATTRIBUTE_TEXCOORD1
layout (location = 5)                                      in mediump vec2 inTexcoord1;
#endif
#ifdef VERTEX_ATTRIBUTE_INDICES
layout (location = 6)                                      in mediump vec4 inIndices;
#endif
#ifdef VERTEX_ATTRIBUTE_WEIGHTS
layout (location = 7)                                      in mediump vec4 inWeights;
#endif

#ifdef INSTANCE_ATTRIBUTE_TRANSFORM
layout (location = 8)                                      in highp vec4 inInstanceTransformMatrixCol0;
layout (location = 9)                                      in highp vec4 inInstanceTransformMatrixCol1;
layout (location = 10)                                     in highp vec4 inInstanceTransformMatrixCol2;
layout (location = 11)                                     in highp vec4 inInstanceTransformMatrixCol3;
#endif

#ifdef INSTANCE_ATTRIBUTE_POINTLIGHT
layout (location = 12)                                     in mediump vec4 inInstancePointLightColor;
layout (location = 13)                                     in mediump vec4 inInstancePointLightAttenuation;
#endif


#define DESCRIPTOR_SET_PASS(b)                             layout (set = 0, binding = b)
#define DESCRIPTOR_SET_MATPASS(b)                          layout (set = 1, binding = b)
#define DESCRIPTOR_SET_MESHDRAW(b)                         layout (set = 2, binding = b)
#define DESCRIPTOR_SET_INPUTATTACHMENT(b)                  layout (set = 3, binding = b)

// Base
#define DESCRIPTOR_BIND_ENGINE                             0
#define DESCRIPTOR_BIND_CAMERA                             1
#define DESCRIPTOR_BIND_SCENE                              2

// Forward/Deferred
#define DESCRIPTOR_BIND_DEPTH_TEXTURE                      2
#define DESCRIPTOR_BIND_COLOR_TEXTURE                      2
#define DESCRIPTOR_BIND_ALBEDO_AO_TEXTURE                  3
#define DESCRIPTOR_BIND_NORMAL_ROUGHNESS_METALLIC_TEXTURE  4

// PostProcessing
#define DESCRIPTOR_BIND_SHADOW_TEXTURE                     5
#define DESCRIPTOR_BIND_SSAO_TEXTURE                       6
#define DESCRIPTOR_BIND_ADD_TEXTURE                        5


#define USE_CAMERA_UNIFORM                                                         \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_CAMERA) uniform Camera                     \
	{                                                                              \
		highp vec4 screen;                                                         \
		highp vec4 depth;                                                          \
		                                                                           \
		highp mat4 viewMatrix;                                                     \
		highp mat4 viewInverseMatrix;                                              \
		highp mat4 viewInverseTransposeMatrix;                                     \
		highp mat4 projectionMatrix;                                               \
		highp mat4 projectionViewMatrix;                                           \
		highp mat4 projectionInverseMatrix;                                        \
		highp mat4 projectionViewInverseMatrix;                                    \
	} camera

#define USE_ENGINE_UNIFORM                                                         \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_ENGINE) uniform Engine                     \
	{                                                                              \
		mediump vec4 time;                                                         \
		mediump vec4 sinTime;                                                      \
		mediump vec4 cosTime;                                                      \
		mediump vec4 deltaTime;                                                    \
		                                                                           \
		mediump vec4 lightFactor;                                                  \
		                                                                           \
		mediump vec4 ambientLightSH0;                                              \
		mediump vec4 ambientLightSH1;                                              \
		mediump vec4 ambientLightSH2;                                              \
		mediump vec4 ambientLightSH3;                                              \
		mediump vec4 ambientLightSH4;                                              \
		mediump vec4 ambientLightSH5;                                              \
		mediump vec4 ambientLightSH6;                                              \
		mediump vec4 ambientLightSH7;                                              \
		mediump vec4 ambientLightSH8;                                              \
		mediump mat4 ambientLightRotationMatrix;                                   \
		                                                                           \
		mediump vec4 mainPointLightColor;                                          \
		mediump vec4 mainPointLightPosition;                                       \
		mediump vec4 mainPointLightAttenuation;                                    \
		                                                                           \
		mediump vec4 mainDirectLightColor;                                         \
		mediump vec4 mainDirectLightDirection;                                     \
		                                                                           \
		mediump vec4 mainFogColor;                                                 \
		mediump vec4 mainFogHeightParams;                                          \
		mediump vec4 mainFogDistanceParams;                                        \
		                                                                           \
		highp vec4 mainShadowLevelFactor;                                          \
		highp vec4 mainShadowDepth[4];                                             \
		highp mat4 mainShadowViewMatrix[4];                                        \
		highp mat4 mainShadowViewInverseMatrix[4];                                 \
		highp mat4 mainShadowProjectionMatrix[4];                                  \
		highp mat4 mainShadowProjectionViewMatrix[4];                              \
	} engine

#define USE_DEPTH_TEXTURE_UNIFORM                                                  \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_DEPTH_TEXTURE) highp uniform sampler2D texDepth

#define USE_COLOR_TEXTURE_UNIFORM                                                  \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_COLOR_TEXTURE) mediump uniform sampler2D texColor

#define USE_ALBEDO_AO_TEXTURE_UNIFORM                                              \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_ALBEDO_AO_TEXTURE) mediump uniform sampler2D texAlbedoAO

#define USE_NORMAL_ROUGHNESS_METALLIC_TEXTURE_UNIFORM                              \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_NORMAL_ROUGHNESS_METALLIC_TEXTURE) mediump uniform sampler2D texNormalRoughnessMetallic

#define USE_SHADOW_TEXTURE_UNIFORM                                                 \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_SHADOW_TEXTURE) highp uniform sampler2D texShadow

#define USE_SSAO_TEXTURE_UNIFORM                                                   \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_SSAO_TEXTURE) mediump uniform sampler2D texSSAO

#define USE_ADD_TEXTURE_UNIFORM                                                    \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_ADD_TEXTURE) mediump uniform sampler2D texAdd


USE_CAMERA_UNIFORM;
#define screenWidth                              camera.screen.x
#define screenHeight                             camera.screen.y

#define cameraZNear                              camera.depth.x
#define cameraZFar                               camera.depth.y

#define cameraViewMatrix                         camera.viewMatrix
#define cameraViewInverseMatrix                  camera.viewInverseMatrix
#define cameraViewInverseTransposeMatrix         camera.viewInverseTransposeMatrix
#define cameraProjectionMatrix                   camera.projectionMatrix
#define cameraProjectionViewMatrix               camera.projectionViewMatrix
#define cameraProjectionInverseMatrix            camera.projectionInverseMatrix
#define cameraProjectionViewInverseMatrix        camera.projectionViewInverseMatrix

USE_ENGINE_UNIFORM;
#define mainShadowLevelFactor                    engine.mainShadowLevelFactor
#define mainShadowZNear(indexLevel)              engine.mainShadowDepth[indexLevel].x
#define mainShadowZFar(indexLevel)               engine.mainShadowDepth[indexLevel].y
#define mainShadowViewMatrix(indexLevel)         engine.mainShadowViewMatrix[indexLevel]
#define mainShadowViewInverseMatrix(indexLevel)  engine.mainShadowViewInverseMatrix[indexLevel]
#define mainShadowProjectionMatrix(indexLevel)   engine.mainShadowProjectionMatrix[indexLevel]
#define mainShadowProjectionViewMatrix(indexLevel) engine.mainShadowProjectionViewMatrix[indexLevel]

#define ambientLightFactor                       engine.lightFactor.x
#define pointLightFactor                         engine.lightFactor.y
#define directLightFactor                        engine.lightFactor.z
#define envLightFactor                           engine.lightFactor.w

#define mainPointLightColor                      engine.mainPointLightColor.rgb
#define mainPointLightPosition                   engine.mainPointLightPosition.xyz
#define mainDirectLightColor                     engine.mainDirectLightColor.rgb
#define mainDirectLightDirection                 engine.mainDirectLightDirection.xyz

#define mainFogColor                             engine.mainFogColor.rgb
#define mainFogHeightStart                       engine.mainFogHeightParams.x
#define mainFogHeightEnd                         engine.mainFogHeightParams.y
#define mainFogHeightDensity                     engine.mainFogHeightParams.z
#define mainFogDistanceStart                     engine.mainFogDistanceParams.x
#define mainFogDistanceEnd                       engine.mainFogDistanceParams.y
#define mainFogDistanceDensity                   engine.mainFogDistanceParams.z

#endif
