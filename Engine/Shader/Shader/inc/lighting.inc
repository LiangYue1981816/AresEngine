#ifndef _LIGHT_INC_
#define _LIGHT_INC_

#include "engine.inc"
#include "common.inc"


const vec3 minLighting = vec3(0.0);
const vec3 maxLighting = vec3(10.0);
	
	
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
vec3 Ambient_SH4(vec3 normal, vec3 albedo, float metallic, vec4 shRed, vec4 shGreen, vec4 shBlue)
{
	float factor = 1.0;
	float x = normal.x;
	float y = normal.y;
	float z = normal.z;

	vec4 basis = vec4(1.0, factor * y, factor * z, factor * x);
	vec3 diffuse = vec3(dot(basis, shRed), dot(basis, shGreen), dot(basis, shBlue));
	vec3 finalLighting = vec3(1.0 - metallic) * diffuse * albedo / PI;

	finalLighting = max(finalLighting, minLighting);
	finalLighting = min(finalLighting, maxLighting);

	return finalLighting;
}

vec3 Ambient_SH9(vec3 normal, vec3 albedo, float metallic, vec3 shRed0, vec3 shRed1, vec3 shRed2, vec3 shGreen0, vec3 shGreen1, vec3 shGreen2, vec3 shBlue0, vec3 shBlue1, vec3 shBlue2)
{
	float factor = 1.0;
	float x = normal.x;
	float y = normal.y;
	float z = normal.z;

	vec3 basis0 = vec3(1.0, factor * y, factor * z);
	vec3 basis1 = vec3(factor * x, x * y, y * z);
	vec3 basis2 = vec3(z * z * 3.0f - 1.0f, x * z, x * x - y * y);
	vec3 diffuse = basis0 * mat3(shRed0, shGreen0, shBlue0) + basis1 * mat3(shRed1, shGreen1, shBlue1) + basis2 * mat3(shRed2, shGreen2, shBlue2);
	vec3 finalLighting = vec3(1.0 - metallic) * diffuse * albedo / PI;

	finalLighting = max(finalLighting, minLighting);
	finalLighting = min(finalLighting, maxLighting);
	
	return finalLighting;
}

vec3 Diffuse_Lambert(vec3 albedo)
{
	return albedo / PI;
}

float D_GGX(float ndh, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float d = (ndh * a2 - ndh) * ndh + 1.0;
	return a2 / (PI * d * d);
}

float D_Ashikhmin(float ndh, float roughness)
{
	// https://www.shadertoy.com/view/4tfBzn
	// https://knarkowicz.wordpress.com/2018/01/04/cloth-shading/
	float a2 = roughness * roughness;
	float cos2h = ndh * ndh;
	float sin2h = max(1.0 - cos2h, 0.0078125); // 2^(-14/2), so sin2h^2 > 0 in fp16
	float sin4h = sin2h * sin2h;
	float cot2 = -cos2h / (a2 * sin2h);
	return 1.0 / (PI * (4.0 * a2 + 1.0) * sin4h) * (4.0 * exp(cot2) + sin4h);
}

float D_Charlie(float ndh, float roughness)
{
	// https://www.shadertoy.com/view/4tfBzn
	// https://knarkowicz.wordpress.com/2018/01/04/cloth-shading/
	float invAlpha = 1.0 / roughness;
	float cos2h = ndh * ndh;
	float sin2h = max(1.0 - cos2h, 0.0078125); // 2^(-14/2), so sin2h^2 > 0 in fp16
	return (2.0 + invAlpha) * pow(sin2h, invAlpha * 0.5) / (2.0 * PI);
}

float G_Ashikhmin(float ndv, float ndl)
{
	// https://www.shadertoy.com/view/4tfBzn
	// https://knarkowicz.wordpress.com/2018/01/04/cloth-shading/
	return 1.0 / (4.0 * (ndl + ndv - ndl * ndv));
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

vec3 F_Schlick(vec3 albedo, float specular, float metallic, float cosTheta)
{
	// https://en.wikipedia.org/wiki/Schlick%27s_approximation
#ifdef FAST_LIGHTING
	vec3 F0 = mix(vec3(0.08 * specular), albedo, metallic);
	float F1 = clamp(1.0 - cosTheta * 1.5, 0.0, 1.0);
	return F0 + (1.0 - F0) * F1 * F1 * F1;
#else
	vec3 F0 = mix(vec3(0.08 * specular), albedo, metallic);
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
#endif
}

vec3 F_SchlickSkin(float cosTheta)
{
	// https://en.wikipedia.org/wiki/Schlick%27s_approximation
#ifdef FAST_LIGHTING
	vec3 F0 = vec3(0.028);
	float F1 = clamp(1.0 - cosTheta * 1.5, 0.0, 1.0);
	return F0 + (1.0 - F0) * F1 * F1 * F1;
#else
	vec3 F0 = vec3(0.028);
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
#endif
}

vec3 F_SchlickRoughness(vec3 albedo, float specular, float metallic, float roughness, float cosTheta)
{
#ifdef FAST_LIGHTING
	vec3 F0 = mix(vec3(0.08 * specular), albedo, metallic);
	float F1 = clamp(1.0 - cosTheta * 1.5, 0.0, 1.0);
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * F1 * F1 * F1;
#else
	vec3 F0 = mix(vec3(0.08 * specular), albedo, metallic);
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
#endif
}

vec3 Env_Specular(vec3 fresnel, float ndv, float roughness)
{
#ifdef FAST_LIGHTING
	vec4 c0 = vec4(-1.0, -0.0275, -0.572, 0.022);
	vec4 c1 = vec4( 1.0,  0.0425,  1.04, -0.04 );
	vec4 r = roughness * c0 + c1;
	float a001 = clamp(0.3 / (ndv + 0.3), 0.0, 1.0);
	float a004 = min(r.x * r.x, a001 * a001 * a001) * r.x + r.y;
	vec2 AB = vec2(-1.04, 1.04) * a004 + r.zw;
	return fresnel * AB.x + AB.y;
#else
	vec4 c0 = vec4(-1.0, -0.0275, -0.572, 0.022);
	vec4 c1 = vec4( 1.0,  0.0425,  1.04, -0.04 );
	vec4 r = roughness * c0 + c1;
	float a004 = min(r.x * r.x, exp2(-9.28 * ndv)) * r.x + r.y;
	vec2 AB = vec2(-1.04, 1.04) * a004 + r.zw;
	return fresnel * AB.x + AB.y;
#endif
}


// BRDF Functions
vec3 AmbientSH4(vec3 normal, vec3 albedo, float metallic)
{
	return Ambient_SH4((SHLightRotationMatrix * vec4(normal, 0.0)).xyz, albedo, metallic, SHLight0, SHLight3, SHLight6);
}

vec3 AmbientSH9(vec3 normal, vec3 albedo, float metallic)
{
	return Ambient_SH9((SHLightRotationMatrix * vec4(normal, 0.0)).xyz, albedo, metallic, SHLight0.xyz, SHLight1.xyz, SHLight2.xyz, SHLight3.xyz, SHLight4.xyz, SHLight5.xyz, SHLight6.xyz, SHLight7.xyz, SHLight8.xyz);
}

vec3 PBRLighting(vec3 normal, vec3 viewDirection, vec3 lightDirection, vec3 lightColor, vec3 albedo, float specular, float metallic, float roughness)
{
	vec3 halfDirection = normalize(viewDirection + lightDirection);

	float ndh = max(dot(normal, halfDirection), 0.0);
	float ndv = max(dot(normal, viewDirection), 0.0);
	float ndl = max(dot(normal, lightDirection), 0.0);
	float vdh = max(dot(viewDirection, halfDirection), 0.0);

	float D = D_GGX(ndh, roughness);
	float G = G_SchlickSmithGGX(ndv, ndl, roughness);
	vec3  F = F_Schlick(albedo, specular, metallic, vdh);

	vec3 diffuseLighting = Diffuse_Lambert(albedo) * (1.0 - metallic);
	vec3 specularLighting = D * F * G;
	vec3 finalLighting = (diffuseLighting + specularLighting) * ndl * lightColor;

	finalLighting = max(finalLighting, minLighting);
	finalLighting = min(finalLighting, maxLighting);

	return finalLighting;
}

vec3 ClothLighting(vec3 normal, vec3 viewDirection, vec3 lightDirection, vec3 lightColor, vec3 albedo, float specular, float roughness)
{
	vec3 halfDirection = normalize(viewDirection + lightDirection);

	float ndh = max(dot(normal, halfDirection), 0.0);
	float ndv = max(dot(normal, viewDirection), 0.0);
	float ndl = max(dot(normal, lightDirection), 0.0);
	float vdh = max(dot(viewDirection, halfDirection), 0.0);

	float D = D_Charlie(ndh, roughness);
	float G = G_Ashikhmin(ndv, ndl);
	vec3  F = F_Schlick(albedo, specular, 0.0, vdh);

	vec3 diffuseLighting = Diffuse_Lambert(albedo);
	vec3 specularLighting = D * F * G;
	vec3 finalLighting = (diffuseLighting + specularLighting) * ndl * lightColor;

	finalLighting = max(finalLighting, minLighting);
	finalLighting = min(finalLighting, maxLighting);

	return finalLighting;
}

float PHBeckmann(float ndoth, float m)
{
	float alpha = acos(ndoth);
	float ta = tan(alpha);
	float val = 1.0 / (m * m * pow(ndoth, 4.0)) * exp(-(ta * ta) / (m * m));
	return val;
}

vec3 SkinLighting(vec3 normalBase, vec3 normalDetails, vec3 position, vec3 viewDirection, vec3 lightDirection, vec3 lightColor, vec3 albedo, float roughness, sampler2D texSkinLUT)
{
	vec3 halfDirection = normalize(viewDirection + lightDirection);

	float ndl = max(dot(normalBase, lightDirection), 0.0);
	float ndh = max(dot(normalDetails, halfDirection), 0.0);
	float hdh = max(dot(viewDirection + lightDirection, viewDirection + lightDirection), 0.0);
	float vdh = max(dot(viewDirection, halfDirection), 0.0);
	float wrapped_ndl = clamp(ndl * 0.5 + 0.5, 0.0, 1.0);

	float tuneCurvature = 0.025;
	float curvature = 1.0 - clamp(tuneCurvature * length(fwidth(normalBase)) / length(fwidth(position)), 0.0, 1.0);

	vec3 diffuseLighting = Diffuse_Lambert(albedo) * 0.15;
	vec3 scatterLighting = texture(texSkinLUT, vec2(wrapped_ndl, curvature)).rgb * albedo * 0.1;
	vec3 specularLighting = max(pow(texture(texSkinLUT, vec2(ndh, roughness)).a * 2.0, 10.0) * F_SchlickSkin(vdh) / hdh, 0.0) * max(ndl, 0.0) * 0.5;
	vec3 finalLighting = (diffuseLighting + scatterLighting + specularLighting) * lightColor;

	finalLighting = max(finalLighting, minLighting);
	finalLighting = min(finalLighting, maxLighting);

	return finalLighting;
}

vec3 EnvLighting(vec3 normal, vec3 viewDirection, vec3 env, vec3 albedo, float specular, float metallic, float roughness)
{
	float ndv = max(dot(normal, viewDirection), 0.0);
	vec3 fresnel = F_SchlickRoughness(albedo, specular, metallic, roughness, ndv);
	return Env_Specular(fresnel, ndv, roughness) * env * albedo;
}

vec3 EnvLighting(vec3 normal, vec3 viewDirection, vec3 albedo, float specular, float metallic, float roughness, sampler2D texEnv, float maxLods)
{
	vec3 R = reflect(-viewDirection, normal);
	vec3 env = Gamma2Linear(textureLod(texEnv, SphericalSampleing(R), roughness * maxLods).rgb);
	return EnvLighting(normal, viewDirection, env, albedo, specular, metallic, roughness);
}

vec3 EnvLighting(vec3 normal, vec3 viewDirection, vec3 albedo, float specular, float metallic, float roughness, samplerCube texEnv, float maxLods)
{
	vec3 R = reflect(-viewDirection, normal);
	vec3 env = Gamma2Linear(textureLod(texEnv, R, roughness * maxLods).rgb);
	return EnvLighting(normal, viewDirection, env, albedo, specular, metallic, roughness);
}

float LightingAttenuation(float dis)
{
	return Attenuation(dis, mainPointLightAttenuation.x, mainPointLightAttenuation.y, mainPointLightAttenuation.z);
}

#endif
