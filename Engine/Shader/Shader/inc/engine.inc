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

#ifdef INSTANCE_ATTRIBUTE_INDEX
layout (location = 8)                                      in mediump int inInstanceIndex;
#endif


#define DESCRIPTOR_SET_PASS(b)                             layout (set = 0, binding = b)
#define DESCRIPTOR_SET_MATPASS(b)                          layout (set = 1, binding = b)
#define DESCRIPTOR_SET_MESHDRAW(b)                         layout (set = 2, binding = b)
#define DESCRIPTOR_SET_INPUTATTACHMENT(b)                  layout (set = 3, binding = b)

// Base
#define DESCRIPTOR_BIND_ENGINE                             0
#define DESCRIPTOR_BIND_CAMERA                             1
#define DESCRIPTOR_BIND_SCENE_DATA                         2
#define DESCRIPTOR_BIND_TRANSFER_SCENE_DATA                3

// Forward/Deferred
#define DESCRIPTOR_BIND_DEPTH_TEXTURE                      4
#define DESCRIPTOR_BIND_COLOR_TEXTURE                      5
#define DESCRIPTOR_BIND_GBUFFERA_TEXTURE                   6
#define DESCRIPTOR_BIND_GBUFFERB_TEXTURE                   7
#define DESCRIPTOR_BIND_GBUFFERC_TEXTURE                   8
#define DESCRIPTOR_BIND_GBUFFERD_TEXTURE                   9

// PostProcessing
#define DESCRIPTOR_BIND_SHADOW_TEXTURE                     10
#define DESCRIPTOR_BIND_SSAO_TEXTURE                       11
#define DESCRIPTOR_BIND_ADD_TEXTURE                        12


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
		mediump vec4 ambientSHLight0;                                              \
		mediump vec4 ambientSHLight1;                                              \
		mediump vec4 ambientSHLight2;                                              \
		mediump vec4 ambientSHLight3;                                              \
		mediump vec4 ambientSHLight4;                                              \
		mediump vec4 ambientSHLight5;                                              \
		mediump vec4 ambientSHLight6;                                              \
		mediump vec4 ambientSHLight7;                                              \
		mediump vec4 ambientSHLight8;                                              \
		mediump mat4 ambientSHLightRotationMatrix;                                 \
		                                                                           \
		mediump vec4 pointLightColor;                                              \
		mediump vec4 pointLightPosition;                                           \
		mediump vec4 pointLightAttenuation;                                        \
		                                                                           \
		mediump vec4 directLightColor;                                             \
		mediump vec4 directLightDirection;                                         \
		                                                                           \
		mediump vec4 fogColor;                                                     \
		mediump vec4 fogHeightParams;                                              \
		mediump vec4 fogDistanceParams;                                            \
		                                                                           \
		highp vec4 shadowLevelFactor;                                              \
		highp vec4 shadowDepth[4];                                                 \
		highp mat4 shadowViewMatrix[4];                                            \
		highp mat4 shadowViewInverseMatrix[4];                                     \
		highp mat4 shadowProjectionMatrix[4];                                      \
		highp mat4 shadowProjectionViewMatrix[4];                                  \
	} engine

	struct InstanceData
	{
		highp mat4 transformMatrix;
		highp vec4 center;

		mediump vec4 lightColor;
		mediump vec4 lightAttenuation;
	};

	struct TransferData
	{
		ivec4 index;
		InstanceData data;
	};

#define USE_SCENE_DATA_STORAGE                                                     \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_SCENE_DATA) buffer SceneData               \
	{                                                                              \
		InstanceData data[];                                                       \
	} sceneData

#define USE_TRANSFER_SCENE_DATA_STORAGE                                            \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_TRANSFER_SCENE_DATA) buffer TransferSceneData \
	{                                                                              \
		TransferData data[];                                                       \
	} transferSceneData

#define USE_DEPTH_TEXTURE_UNIFORM                                                  \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_DEPTH_TEXTURE) highp uniform sampler2D texDepth

#define USE_COLOR_TEXTURE_UNIFORM                                                  \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_COLOR_TEXTURE) mediump uniform sampler2D texColor

#define USE_SHADOW_TEXTURE_UNIFORM                                                 \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_SHADOW_TEXTURE) highp uniform sampler2D texShadow

#define USE_SSAO_TEXTURE_UNIFORM                                                   \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_SSAO_TEXTURE) mediump uniform sampler2D texSSAO

#define USE_ADD_TEXTURE_UNIFORM                                                    \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_ADD_TEXTURE) mediump uniform sampler2D texAdd


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

#define ambientLightFactor                       engine.lightFactor.x
#define pointLightFactor                         engine.lightFactor.y
#define directLightFactor                        engine.lightFactor.z
#define envLightFactor                           engine.lightFactor.w

#define SHLightRotationMatrix                    engine.ambientSHLightRotationMatrix
#define SHLight0                                 engine.ambientSHLight0
#define SHLight1                                 engine.ambientSHLight1
#define SHLight2                                 engine.ambientSHLight2
#define SHLight3                                 engine.ambientSHLight3
#define SHLight4                                 engine.ambientSHLight4
#define SHLight5                                 engine.ambientSHLight5
#define SHLight6                                 engine.ambientSHLight6
#define SHLight7                                 engine.ambientSHLight7
#define SHLight8                                 engine.ambientSHLight8

#define mainPointLightColor                      engine.pointLightColor.rgb
#define mainPointLightPosition                   engine.pointLightPosition.xyz
#define mainPointLightAttenuation                engine.pointLightAttenuation.xyz
#define mainDirectLightColor                     engine.directLightColor.rgb
#define mainDirectLightDirection                 engine.directLightDirection.xyz

#define mainFogColor                             engine.fogColor.rgb
#define mainFogHeightStart                       engine.fogHeightParams.x
#define mainFogHeightEnd                         engine.fogHeightParams.y
#define mainFogHeightDensity                     engine.fogHeightParams.z
#define mainFogDistanceStart                     engine.fogDistanceParams.x
#define mainFogDistanceEnd                       engine.fogDistanceParams.y
#define mainFogDistanceDensity                   engine.fogDistanceParams.z

#define mainShadowLevelFactor                    engine.shadowLevelFactor
#define mainShadowZNear(indexLevel)              engine.shadowDepth[indexLevel].x
#define mainShadowZFar(indexLevel)               engine.shadowDepth[indexLevel].y
#define mainShadowViewMatrix(indexLevel)         engine.shadowViewMatrix[indexLevel]
#define mainShadowViewInverseMatrix(indexLevel)  engine.shadowViewInverseMatrix[indexLevel]
#define mainShadowProjectionMatrix(indexLevel)   engine.shadowProjectionMatrix[indexLevel]
#define mainShadowProjectionViewMatrix(indexLevel) engine.shadowProjectionViewMatrix[indexLevel]

#endif
