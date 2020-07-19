#ifndef _ENGINE_INC_
#define _ENGINE_INC_


#ifdef VERTEX_ATTRIBUTE_POSITION
layout (location = 0)                                      in vec3 inPosition;
#endif
#ifdef VERTEX_ATTRIBUTE_NORMAL
layout (location = 1)                                      in vec4 inNormal;
#endif
#ifdef VERTEX_ATTRIBUTE_BINORMAL
layout (location = 2)                                      in vec4 inBinormal;
#endif
#ifdef VERTEX_ATTRIBUTE_COLOR
layout (location = 3)                                      in vec4 inColor;
#endif
#ifdef VERTEX_ATTRIBUTE_TEXCOORD0
layout (location = 4)                                      in vec2 inTexcoord0;
#endif
#ifdef VERTEX_ATTRIBUTE_TEXCOORD1
layout (location = 5)                                      in vec2 inTexcoord1;
#endif
#ifdef VERTEX_ATTRIBUTE_INDICES
layout (location = 6)                                      in vec4 inIndices;
#endif
#ifdef VERTEX_ATTRIBUTE_WEIGHTS
layout (location = 7)                                      in vec4 inWeights;
#endif

#ifdef INSTANCE_ATTRIBUTE_INDEX
#ifdef _VULKAN_
layout (location = 8)                                      in int inInstanceIndex;
#else
layout (location = 8)                                      in float inInstanceIndex;
#endif
#endif


#define DESCRIPTOR_SET_PASS(b)                             layout (set = 0, binding = b)
#define DESCRIPTOR_SET_MATPASS(b)                          layout (set = 1, binding = b)
#define DESCRIPTOR_SET_MESHDRAW(b)                         layout (set = 2, binding = b)
#define DESCRIPTOR_SET_INPUTATTACHMENT(i,b)                layout (set = 3, input_attachment_index = i, binding = b)

#define DESCRIPTOR_SET_PASS_IMAGE(b,f)                     layout (set = 0, binding = b, f)
#define DESCRIPTOR_SET_MATPASS_IMAGE(b,f)                  layout (set = 1, binding = b, f)
#define DESCRIPTOR_SET_MESHDRAW_IMAGE(b,f)                 layout (set = 2, binding = b, f)

// Base
#define DESCRIPTOR_BIND_ENGINE                             0
#define DESCRIPTOR_BIND_CAMERA                             1
#define DESCRIPTOR_BIND_SCENE_DATA                         2
#define DESCRIPTOR_BIND_TRANSFER_SCENE_DATA                3
#define DESCRIPTOR_BIND_TRANSFER_SCENE_DATA_INDEX          4
#define DESCRIPTOR_BIND_CLUSTER_DATA                       5
#define DESCRIPTOR_BIND_FULL_LIGHT_INDEX_DATA              6
#define DESCRIPTOR_BIND_CULL_LIGHT_INDEX_DATA              7

// Forward/Deferred
#define DESCRIPTOR_BIND_COLOR_IMAGE                        4
#define DESCRIPTOR_BIND_DEPTH_TEXTURE                      8
#define DESCRIPTOR_BIND_COLOR_TEXTURE                      9
#define DESCRIPTOR_BIND_GBUFFERA_TEXTURE                   10
#define DESCRIPTOR_BIND_GBUFFERB_TEXTURE                   11
#define DESCRIPTOR_BIND_GBUFFERC_TEXTURE                   12
#define DESCRIPTOR_BIND_GBUFFERD_TEXTURE                   13

// PostProcessing
#define DESCRIPTOR_BIND_SHADOW_TEXTURE                     16
#define DESCRIPTOR_BIND_SSAO_TEXTURE                       17
#define DESCRIPTOR_BIND_ADD_TEXTURE                        18
#define DESCRIPTOR_BIND_HISTOGRAM_DATA                     19


#define USE_CAMERA_UNIFORM                                                         \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_CAMERA) uniform Camera                     \
	{                                                                              \
		vec4 screen;                                                               \
		vec4 depth;                                                                \
		                                                                           \
		mat4 viewMatrix;                                                           \
		mat4 viewInverseMatrix;                                                    \
		mat4 viewInverseTransposeMatrix;                                           \
		mat4 projectionMatrix;                                                     \
		mat4 projectionViewMatrix;                                                 \
		mat4 projectionInverseMatrix;                                              \
		mat4 projectionViewInverseMatrix;                                          \
	} camera;

#define USE_ENGINE_UNIFORM                                                         \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_ENGINE) uniform Engine                     \
	{                                                                              \
		vec4 time;                                                                 \
		vec4 sinTime;                                                              \
		vec4 cosTime;                                                              \
		vec4 deltaTime;                                                            \
		                                                                           \
		vec4 lightFactor;                                                          \
		                                                                           \
		vec4 ambientSHLight0;                                                      \
		vec4 ambientSHLight1;                                                      \
		vec4 ambientSHLight2;                                                      \
		vec4 ambientSHLight3;                                                      \
		vec4 ambientSHLight4;                                                      \
		vec4 ambientSHLight5;                                                      \
		vec4 ambientSHLight6;                                                      \
		vec4 ambientSHLight7;                                                      \
		vec4 ambientSHLight8;                                                      \
		mat4 ambientSHLightRotationMatrix;                                         \
		                                                                           \
		vec4 pointLightColor;                                                      \
		vec4 pointLightPosition;                                                   \
		vec4 pointLightAttenuation;                                                \
		                                                                           \
		vec4 directLightColor;                                                     \
		vec4 directLightDirection;                                                 \
		                                                                           \
		vec4 fogColor;                                                             \
		vec4 fogHeightParams;                                                      \
		vec4 fogDistanceParams;                                                    \
		                                                                           \
		vec4 shadowLevelFactor;                                                    \
		vec4 shadowDepth[4];                                                       \
		mat4 shadowViewMatrix[4];                                                  \
		mat4 shadowViewInverseMatrix[4];                                           \
		mat4 shadowProjectionMatrix[4];                                            \
		mat4 shadowProjectionViewMatrix[4];                                        \
	} engine;

	struct Instance
	{
		mat4 transformMatrix;
		vec4 center;

		vec4 lightColor;
		vec4 lightAttenuation;
	};

	struct Cluster
	{
		vec4 minAABBPosition;
		vec4 maxAABBPosition;
	};

#define USE_SCENE_DATA_STORAGE                                                     \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_SCENE_DATA) buffer SceneData               \
	{                                                                              \
		Instance instances[];                                                      \
	} sceneData;

#define USE_TRANSFER_SCENE_DATA_STORAGE                                            \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_TRANSFER_SCENE_DATA) buffer TransferSceneData \
	{                                                                              \
		Instance instances[];                                                      \
	} transferSceneData;

#define USE_TRANSFER_SCENE_DATA_INDEX_STORAGE                                      \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_TRANSFER_SCENE_DATA_INDEX) buffer TransferSceneDataIndex \
	{                                                                              \
		int indexInstances[];                                                      \
	} transferSceneDataIndex;

#define USE_CLUSTER_DATA_STORAGE                                                   \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_CLUSTER_DATA) buffer ClusterData           \
	{                                                                              \
		Cluster clusters[];                                                        \
	} clusterData;

#define USE_FULL_LIGHT_INDEX_DATA_STORAGE                                           \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_FULL_LIGHT_INDEX_DATA) buffer FullLightIndexData \
	{                                                                              \
		int indexLights[];                                                         \
	} fullLightIndexData;

#define USE_CULL_LIGHT_INDEX_DATA_STORAGE                                           \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_CULL_LIGHT_INDEX_DATA) buffer CullLightIndexData \
	{                                                                              \
		int indexLights[];                                                         \
	} cullLightIndexData;

#define HISTOGRAM_SIZE 64
#define USE_HISTOGRAM_STORAGE                                                      \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_HISTOGRAM_DATA) buffer HistogramData       \
	{                                                                              \
		uint histogram[];                                                          \
	} histogramData;

#define USE_DEPTH_TEXTURE_UNIFORM                                                  \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_DEPTH_TEXTURE) uniform sampler2D texDepth;

#define USE_COLOR_TEXTURE_UNIFORM                                                  \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_COLOR_TEXTURE) uniform sampler2D texColor;

#define USE_SHADOW_TEXTURE_UNIFORM                                                 \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_SHADOW_TEXTURE) uniform sampler2D texShadow;

#define USE_SSAO_TEXTURE_UNIFORM                                                   \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_SSAO_TEXTURE) uniform sampler2D texSSAO;

#define USE_ADD_TEXTURE_UNIFORM                                                    \
	DESCRIPTOR_SET_PASS(DESCRIPTOR_BIND_ADD_TEXTURE) uniform sampler2D texAdd;

#define USE_COLOR_IMAGE_UNIFORM(format, memory)                                    \
	DESCRIPTOR_SET_PASS_IMAGE(DESCRIPTOR_BIND_COLOR_IMAGE, format) uniform memory highp image2D imgColor;


#define screenWidth                              camera.screen.x
#define screenHeight                             camera.screen.y
#define screenSize                               camera.screen.xy

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
