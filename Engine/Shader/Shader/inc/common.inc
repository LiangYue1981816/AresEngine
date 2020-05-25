#ifndef _COMMON_INC_
#define _COMMON_INC_

#define PI 3.1415926535897932384626433832795


highp float pow2(highp float x)
{
	return x * x;
}

highp float pow3(highp float x)
{
	return x * x * x;
}

highp vec3 pow2(highp vec3 x)
{
	return x * x;
}

highp vec3 pow3(highp vec3 x)
{
	return x * x * x;
}

highp vec4 PackFloat(highp float value)
{
	highp vec4 kEncodeMul = vec4(1.0, 255.0, 65025.0, 16581375.0);
	highp vec4 enc = kEncodeMul * value;
	enc  = fract(enc);
	enc -= enc.yzww / 255.0;
	return enc;
}

highp float UnpackFloat(highp vec4 encode) 
{
	highp vec4 kDecodeDot = vec4(1.0, 1.0 / 255.0, 1.0 / 65025.0, 1.0 / 16581375.0);
	return dot(encode, kDecodeDot);
}

mediump vec4 PackHDR(mediump vec3 color)
{
#ifdef ENCODE_HDR
/*
	color *= 1.0 / 8.0;
	mediump float m = max(max(color.r, color.g), max(color.b, 1e-6));
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

mediump vec3 UnpackHDR(mediump vec4 encode)
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

mediump vec3 BlendNormalInTangentSpace(mediump vec3 baseNormal, mediump vec3 additionalNormal)
{
	mediump vec3 newBaseNormal = vec3(baseNormal.rg, baseNormal.b + 1.0);
	mediump vec3 newAdditionalNormal = vec3(additionalNormal.rg * (-1.0), additionalNormal.b);
	
	newBaseNormal = newBaseNormal * dot(newBaseNormal, newAdditionalNormal);
	newAdditionalNormal = newAdditionalNormal * (baseNormal.b + 1.0);
	
	return normalize(newBaseNormal - newAdditionalNormal);
}

highp vec2 PoissonDisk(highp vec3 seed, highp int index)
{
	highp vec2 offset[16] = vec2[]( 
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

	highp vec4 seed4 = vec4(floor(seed * 1000.0), index);
	highp float dotProduct = dot(seed4, vec4(12.9898, 78.233, 45.164, 94.673));
	return offset[int(16.0 * fract(sin(dotProduct) * 43758.5453)) % 16];
}

mediump vec3 Gray(mediump vec3 color, mediump float percent)
{
	return (1.0 - percent) * dot(vec3(0.299, 0.587, 0.114), color) + percent * color;
}

mediump vec2 NormalEncode(mediump vec3 normal)
{
	mediump float f = dot(vec3(1.0), abs(normal));
	mediump float x = normal.x / f;
	mediump float y = normal.y / f;
	mediump float z = normal.z;
	mediump vec2 encode = vec2(
		z <= 0.0 ? ((1.0 - abs(y)) * (x >= 0.0 ? 1.0 : -1.0)) : x, 
		z <= 0.0 ? ((1.0 - abs(x)) * (y >= 0.0 ? 1.0 : -1.0)) : y);
	encode = encode * 0.5 + 0.5;
	return encode;
}

mediump vec3 NormalDecode(mediump vec2 encode)
{
	encode = encode * 2.0 - 1.0;
	mediump float f = dot(vec2(1.0), abs(encode));
	mediump float x = encode.x;
	mediump float y = encode.y;
	mediump float z = 1.0 - f;
	mediump vec3 normal = vec3(
		z < 0.0 ? ((1.0 - abs(y)) * (x >= 0.0 ? 1.0 : -1.0)) : x,
		z < 0.0 ? ((1.0 - abs(x)) * (y >= 0.0 ? 1.0 : -1.0)) : y,
		z);
	return normalize(normal);
}

mediump vec2 ViewSpaceNormalEncode(mediump vec3 normal)
{
	// https://aras-p.info/texts/CompactNormalStorage.html
	mediump float p = sqrt(normal.z * 8.0 + 8.0);
	mediump vec2 encode = vec2(normal.xy / p + 0.5);
	return encode;
}

mediump vec3 ViewSpaceNormalDecode(mediump vec2 encode)
{
	// https://aras-p.info/texts/CompactNormalStorage.html
	mediump vec2 e = encode * 4.0 - 2.0;
	mediump float f = dot(e, e);
	mediump float g = sqrt(1.0 - f / 4.0);
	mediump vec3 normal = vec3(e * g, 1.0 - f / 2.0f);
	return normal;
}

highp vec4 ScreenToViewPosition(highp vec2 screen, highp float depth, highp mat4 projectionInverseMatrix)
{
	highp vec4 clipPosition = vec4(screen * 2.0 - 1.0, depth, 1.0);       // NDC DepthRange [ 0, 1] Vulkan DirectX
//	highp vec4 clipPosition = vec4(vec3(screen, depth) * 2.0 - 1.0, 1.0); // NDC DepthRange [-1, 1] OpenGL
	highp vec4 viewPosition = projectionInverseMatrix * clipPosition;
	return viewPosition.xyzw / viewPosition.w;
}

highp vec4 ScreenToWorldPosition(highp vec2 screen, highp float depth, highp mat4 projectionInverseMatrix, highp mat4 viewInverseMatrix)
{
	highp vec4 viewPosition = ScreenToViewPosition(screen, depth, projectionInverseMatrix);
	highp vec4 worldPosition = viewInverseMatrix * viewPosition;
	return worldPosition;
}

highp float LinearDepth(highp float depth, highp float zNear, highp float zFar)
{
	highp float z = depth * 2.0 - 1.0;
	return (2.0 * zNear * zFar) / (zFar + zNear - z * (zFar - zNear));
}

highp vec3 LineIntersectionToZPlane(highp vec3 from, highp vec3 to, highp float length)
{
	highp vec3 normal = vec3(0.0, 0.0, 1.0);
	highp vec3 direction = to - from;
	highp vec3 result = from + (length - dot(normal, from)) / dot(normal, direction) * direction;
	return result;
}

bool Intersection(highp vec3 minAABBPosition, highp vec3 maxAABBPosition, highp vec3 spherePosition, highp float radius)
{
	highp float dis2 = 0.0;

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
