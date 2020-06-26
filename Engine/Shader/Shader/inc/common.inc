#ifndef _COMMON_INC_
#define _COMMON_INC_

#define PI 3.1415926535897932384626433832795


float pow2(float x)
{
	return x * x;
}

float pow3(float x)
{
	return x * x * x;
}

vec3 pow2(vec3 x)
{
	return x * x;
}

vec3 pow3(vec3 x)
{
	return x * x * x;
}

vec4 PackFloat(float value)
{
	vec4 kEncodeMul = vec4(1.0, 255.0, 65025.0, 16581375.0);
	vec4 enc = kEncodeMul * value;
	enc  = fract(enc);
	enc -= enc.yzww / 255.0;
	return enc;
}

float UnpackFloat(vec4 encode) 
{
	vec4 kDecodeDot = vec4(1.0, 1.0 / 255.0, 1.0 / 65025.0, 1.0 / 16581375.0);
	return dot(encode, kDecodeDot);
}

vec4 PackHDR(vec3 color)
{
#ifdef ENCODE_HDR
/*
	color *= 1.0 / 8.0;
	float m = max(max(color.r, color.g), max(color.b, 1e-6));
	m = ceil(m * 255.0) / 255.0;
	return vec4(color / m, m);
/*/
	// https://m17design.wordpress.com/2005/09/20/hdr-and-rgbe/
	float maxComponent = max(max(color.r, color.g), color.b);
	float fExp = ceil(log2(maxComponent));
	vec4 vEncoded = vec4(color / exp2(fExp), (fExp + 128.0) / 255.0);
	return vEncoded;
//*/
#else
	return vec4(color, 1.0);
#endif
}

vec3 UnpackHDR(vec4 encode)
{
#ifdef ENCODE_HDR
/*
	return encode.rgb * encode.a * 8.0;
/*/
	// https://m17design.wordpress.com/2005/09/20/hdr-and-rgbe/
	vec3 vDecoded;
	float fExp = encode.a * 255.0 - 128.0;
	vDecoded = encode.rgb * exp2(fExp);
	return vDecoded;
//*/
#else
	return encode.rgb;
#endif
}

vec3 BlendNormalInTangentSpace(vec3 baseNormal, vec3 additionalNormal)
{
	vec3 newBaseNormal = vec3(baseNormal.rg, baseNormal.b + 1.0);
	vec3 newAdditionalNormal = vec3(additionalNormal.rg * (-1.0), additionalNormal.b);
	
	newBaseNormal = newBaseNormal * dot(newBaseNormal, newAdditionalNormal);
	newAdditionalNormal = newAdditionalNormal * (baseNormal.b + 1.0);
	
	return normalize(newBaseNormal - newAdditionalNormal);
}

vec2 PoissonDisk(vec3 seed, int index)
{
	vec2 offset[16] = vec2[]( 
		vec2(-0.94201624, -0.39906216), 
		vec2( 0.94558609, -0.76890725), 
		vec2(-0.094184101,-0.92938870), 
		vec2( 0.34495938,  0.29387760), 
		vec2(-0.91588581,  0.45771432), 
		vec2(-0.81544232, -0.87912464), 
		vec2(-0.38277543,  0.27676845), 
		vec2( 0.97484398,  0.75648379), 
		vec2( 0.44323325, -0.97511554), 
		vec2( 0.53742981, -0.47373420), 
		vec2(-0.26496911, -0.41893023), 
		vec2( 0.79197514,  0.19090188), 
		vec2(-0.24188840,  0.99706507), 
		vec2(-0.81409955,  0.91437590), 
		vec2( 0.19984126,  0.78641367), 
		vec2( 0.14383161, -0.14100790) 
	);

	vec4 seed4 = vec4(floor(seed * 1000.0), index);
	float dotProduct = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
	return offset[int(16.0 * fract(sin(dotProduct) * 43758.5453)) % 16];
}

vec3 Gray(vec3 color, float percent)
{
	return (1.0 - percent) * dot(vec3(0.299, 0.587, 0.114), color) + percent * color;
}

vec2 NormalEncode(vec3 normal)
{
	float f = dot(vec3(1.0), abs(normal));
	float x = normal.x / f;
	float y = normal.y / f;
	float z = normal.z;
	vec2 encode = vec2(
		z <= 0.0 ? ((1.0 - abs(y)) * (x >= 0.0 ? 1.0 : -1.0)) : x, 
		z <= 0.0 ? ((1.0 - abs(x)) * (y >= 0.0 ? 1.0 : -1.0)) : y);
	encode = encode * 0.5 + 0.5;
	return encode;
}

vec3 NormalDecode(vec2 encode)
{
	encode = encode * 2.0 - 1.0;
	float f = dot(vec2(1.0), abs(encode));
	float x = encode.x;
	float y = encode.y;
	float z = 1.0 - f;
	vec3 normal = vec3(
		z < 0.0 ? ((1.0 - abs(y)) * (x >= 0.0 ? 1.0 : -1.0)) : x,
		z < 0.0 ? ((1.0 - abs(x)) * (y >= 0.0 ? 1.0 : -1.0)) : y,
		z);
	return normalize(normal);
}

vec2 ViewSpaceNormalEncode(vec3 normal)
{
	// https://aras-p.info/texts/CompactNormalStorage.html
	float p = sqrt(normal.z * 8.0 + 8.0);
	vec2 encode = vec2(normal.xy / p + 0.5);
	return encode;
}

vec3 ViewSpaceNormalDecode(vec2 encode)
{
	// https://aras-p.info/texts/CompactNormalStorage.html
	vec2 e = encode * 4.0 - 2.0;
	float f = dot(e, e);
	float g = sqrt(1.0 - f / 4.0);
	vec3 normal = vec3(e * g, 1.0 - f / 2.0f);
	return normal;
}

vec4 ScreenToViewPosition(vec2 screen, float depth, mat4 projectionInverseMatrix)
{
	vec4 clipPosition = vec4(screen * 2.0 - 1.0, depth, 1.0);       // NDC DepthRange [ 0, 1] Vulkan DirectX
//	vec4 clipPosition = vec4(vec3(screen, depth) * 2.0 - 1.0, 1.0); // NDC DepthRange [-1, 1] OpenGL
	vec4 viewPosition = projectionInverseMatrix * clipPosition;
	return viewPosition.xyzw / viewPosition.w;
}

vec4 ScreenToWorldPosition(vec2 screen, float depth, mat4 projectionInverseMatrix, mat4 viewInverseMatrix)
{
	vec4 viewPosition = ScreenToViewPosition(screen, depth, projectionInverseMatrix);
	vec4 worldPosition = viewInverseMatrix * viewPosition;
	return worldPosition;
}

float LinearDepth(float depth, float zNear, float zFar)
{
	float z = depth * 2.0 - 1.0;
	return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));
}

vec3 LineIntersectionToZPlane(vec3 from, vec3 to, float length)
{
	vec3 normal = vec3(0.0, 0.0, 1.0);
	vec3 direction = to - from;
	vec3 result = from + (length - dot(normal, from)) / dot(normal, direction) * direction;
	return result;
}

bool Intersection(vec3 minAABBPosition, vec3 maxAABBPosition, vec3 spherePosition, float radius)
{
	float dis2 = 0.0;

	for (int i = 0; i < 3; i++) {
		if (spherePosition[i] < minAABBPosition[i]) {
			dis2 += (spherePosition[i] - minAABBPosition[i]) * (spherePosition[i] - minAABBPosition[i]);
		}
		if (spherePosition[i] > maxAABBPosition[i]) {
			dis2 += (spherePosition[i] - maxAABBPosition[i]) * (spherePosition[i] - maxAABBPosition[i]);
		}
	}

	return dis2 <= (radius * radius);
}
#endif
