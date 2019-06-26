#ifndef _LIGHT_INC_
#define _LIGHT_INC_

#include "engine.inc"
#include "common.inc"


// Utils
vec2 SphericalSampleing(vec3 v)
{
	vec2 invAtan = vec2(1.0 / (2.0 * PI), 1.0 / (1.0 * PI));
	vec2 uv = vec2(atan(v.x, v.z), -asin(v.y));

	uv *= invAtan;
	uv += 0.5;

	return uv;
}

float Attenuation(float dis, float linear, float square, float constant)
{
	vec3 a = vec3(dis, dis * dis, 1.0);
	vec3 b = vec3(linear, square, constant);
	return 1.0 / dot(a, b);
//	return 1.0 / (linear * dis + square * dis * dis + constant);
}


// ToneMapping
vec3 ToneMapping(vec3 color)
{
	// https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
	float a = 2.51;
	float b = 0.03;
	float c = 2.43;
	float d = 0.59;
	float e = 0.14;
	return clamp((color * (a * color + b)) / (color * (c * color + d) + e), 0.0, 1.0);
}


// Gamma and Linear
vec3 Gamma2Linear(vec3 gamma)
{
//	return gamma * gamma;
	return pow(gamma, vec3(2.2));
}

vec4 Gamma2Linear(vec4 gamma)
{
//	return gamma * gamma;
	return pow(gamma, vec4(2.2));
}

vec3 Linear2Gamma(vec3 linear)
{
//	return sqrt(linear);
	return pow(linear, vec3(1.0 / 2.2));
}


// Lighting Functions
vec3 Ambient_SH4(vec3 albedoColor, float metallic, vec3 normal, vec4 shRed, vec4 shGreen, vec4 shBlue)
{
	float factor = 1.0;
	float x = normal.x;
	float y = normal.y;
	float z = normal.z;

	vec4 basis = vec4(1.0, factor * y, factor * z, factor * x);
	vec3 diffuse = vec3(dot(basis, shRed), dot(basis, shGreen), dot(basis, shBlue));

	return (1.0 - metallic) * diffuse * albedoColor / PI;
}

vec3 Ambient_SH9(vec3 albedoColor, float metallic, vec3 normal, vec3 shRed0, vec3 shRed1, vec3 shRed2, vec3 shGreen0, vec3 shGreen1, vec3 shGreen2, vec3 shBlue0, vec3 shBlue1, vec3 shBlue2)
{
	float factor = 1.0;
	float x = normal.x;
	float y = normal.y;
	float z = normal.z;

	vec3 basis0 = vec3(1.0, factor * y, factor * z);
	vec3 basis1 = vec3(factor * x, x * y, y * z);
	vec3 basis2 = vec3(z * z * 3.0f - 1.0f, x * z, x * x - y * y);
	vec3 diffuse = basis0 * mat3(shRed0, shGreen0, shBlue0) + basis1 * mat3(shRed1, shGreen1, shBlue1) + basis2 * mat3(shRed2, shGreen2, shBlue2);

	return (1.0 - metallic) * diffuse * albedoColor / PI;
}

vec3 Diffuse_Lambert(vec3 albedoColor)
{
	return albedoColor / PI;
}

float D_GGX(float ndh, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float d = (ndh * a2 - ndh) * ndh + 1.0;
	return a2 / (PI * d * d);
}

float G_Neumann(float ndv, float ndl)
{
	return 0.25 / max(ndv, ndl);
}

float G_Schlick(float ndv, float ndl, float roughness)
{
	float k = (roughness * roughness) * 0.5;

	float ggx1 = ndv * (1.0 - k) + k;
	float ggx2 = ndl * (1.0 - k) + k;

	return 0.25 / (ggx1 * ggx2);
}

float G_SchlickSmithGGX(float ndv, float ndl, float roughness)
{
	float r = roughness + 1.0;
	float k = (r * r) / 8.0;

	float ggx1 = ndv * (1.0 - k) + k;
	float ggx2 = ndl * (1.0 - k) + k;

	return 0.25 / (ggx1 * ggx2);
}

vec3 F_Schlick(float ndv, float metallic, vec3 albedo)
{
	vec3 F0 = mix(vec3(0.04), albedo, metallic);
	return F0 + (1.0 - F0) * pow(1.0 - ndv, 5.0);
}

vec3 F_SchlickFast(float ndv, float metallic, vec3 albedo)
{
	vec3 F0 = mix(vec3(0.04), albedo, metallic);
	float F1 = clamp(1.0 - ndv * 1.5, 0.0, 1.0);
	return F0 + (1.0 - F0) * F1 * F1 * F1;
}

vec3 Env_Specular(vec3 specularColor, float ndv, float roughness)
{
	vec4 c0 = vec4(-1.0, -0.0275, -0.572, 0.022);
	vec4 c1 = vec4( 1.0,  0.0425,  1.04, -0.04 );
	vec4 r = roughness * c0 + c1;
	float a004 = min(r.x * r.x, exp2(-9.28 * ndv)) * r.x + r.y;
	vec2 AB = vec2(-1.04, 1.04) * a004 + r.zw;
	return specularColor * AB.x + AB.y;
}

vec3 Env_SpecularFast(vec3 specularColor, float ndv, float roughness)
{
	vec4 c0 = vec4(-1.0, -0.0275, -0.572, 0.022);
	vec4 c1 = vec4( 1.0,  0.0425,  1.04, -0.04 );
	vec4 r = roughness * c0 + c1;
	float a001 = clamp(0.3 / (ndv + 0.3), 0.0, 1.0);
	float a004 = min(r.x * r.x, a001 * a001 * a001) * r.x + r.y;
	vec2 AB = vec2(-1.04, 1.04) * a004 + r.zw;
	return specularColor * AB.x + AB.y;
}


// BRDF Functions
vec3 AmbientSH4(vec3 normal, vec3 albedoColor, float metallic)
{
	return Ambient_SH4(albedoColor, metallic, (engine.ambientLightRotationMatrix * vec4(normal, 0.0)).xyz, engine.ambientLightRed0, engine.ambientLightGreen0, engine.ambientLightBlue0);
}

vec3 AmbientSH9(vec3 normal, vec3 albedoColor, float metallic)
{
	return Ambient_SH9(albedoColor, metallic, (engine.ambientLightRotationMatrix * vec4(normal, 0.0)).xyz, engine.ambientLightRed0.xyz, engine.ambientLightRed1.xyz, engine.ambientLightRed2.xyz, engine.ambientLightGreen0.xyz, engine.ambientLightGreen1.xyz, engine.ambientLightGreen2.xyz, engine.ambientLightBlue0.xyz, engine.ambientLightBlue1.xyz, engine.ambientLightBlue2.xyz);
}

vec3 PBRLighting(vec3 normal, vec3 viewDirection, vec3 halfDirection, vec3 lightDirection, vec3 lightColor, vec3 albedoColor, vec3 specularColor, float metallic, float roughness)
{
	float ndh = max(dot(normal, halfDirection), 0.0);
	float ndv = max(dot(normal, viewDirection), 0.0);
	float ndl = max(dot(normal, lightDirection), 0.0);

#ifdef FAST_LIGHTING
	float D = D_GGX(ndh, roughness);
	float G = G_SchlickSmithGGX(ndv, ndl, roughness);
	vec3  F = F_SchlickFast(ndv, metallic, specularColor);
#else
	float D = D_GGX(ndh, roughness);
	float G = G_SchlickSmithGGX(ndv, ndl, roughness);
	vec3  F = F_Schlick(ndv, metallic, specularColor);
#endif

	vec3 factor = vec3(1.0 - metallic);
	vec3 diffuse = Diffuse_Lambert(albedoColor) * factor;
	vec3 specular = D * F * G;

	return (diffuse + specular) * ndl * lightColor;
}

vec3 EnvSpecularColor(vec3 normal, vec3 viewDirection, float roughness, sampler2D env, float maxLods)
{
	vec3 R = reflect(-viewDirection, normal);
	return Gamma2Linear(textureLod(env, SphericalSampleing(R), roughness * maxLods).rgb);
}

vec3 EnvSpecularColor(vec3 normal, vec3 viewDirection, float roughness, samplerCube env, float maxLods)
{
	vec3 R = reflect(-viewDirection, normal);
	return Gamma2Linear(textureLod(env, R, roughness * maxLods).rgb);
}

float LightingAttenuation(float dis)
{
	return Attenuation(dis, engine.mainPointLightAttenuation.x, engine.mainPointLightAttenuation.y, engine.mainPointLightAttenuation.z);
}

#endif
