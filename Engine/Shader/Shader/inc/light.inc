#ifndef _LIGHT_INC_
#define _LIGHT_INC_

#include "engine.inc"
#include "common.inc"


// Utils
mediump vec2 SphericalSampleing(mediump vec3 v)
{
	mediump vec2 invAtan = vec2(1.0 / (2.0 * PI), 1.0 / (1.0 * PI));
	mediump vec2 uv = vec2(atan(v.x, v.z), -asin(v.y));

	uv *= invAtan;
	uv += 0.5;

	return uv;
}

mediump float Attenuation(mediump float dis, mediump float linear, mediump float square, mediump float constant)
{
	mediump vec3 a = vec3(dis, dis * dis, 1.0);
	mediump vec3 b = vec3(linear, square, constant);
	return 1.0 / dot(a, b);
//	return 1.0 / (linear * dis + square * dis * dis + constant);
}


// ToneMapping
mediump vec3 ToneMapping(mediump vec3 color)
{
	// https://knarkowicz.wordpress.com/2016/01/06/aces-filmic-tone-mapping-curve/
	mediump float a = 2.51;
	mediump float b = 0.03;
	mediump float c = 2.43;
	mediump float d = 0.59;
	mediump float e = 0.14;
	return clamp((color * (a * color + b)) / (color * (c * color + d) + e), 0.0, 1.0);
}


// Gamma and Linear
mediump vec3 Gamma2Linear(mediump vec3 gamma)
{
//	return gamma * gamma;
	return pow(gamma, vec3(2.2));
}

mediump vec4 Gamma2Linear(mediump vec4 gamma)
{
//	return gamma * gamma;
	return pow(gamma, vec4(2.2));
}

mediump vec3 Linear2Gamma(mediump vec3 linear)
{
//	return sqrt(linear);
	return pow(linear, vec3(1.0 / 2.2));
}


// Lighting Functions
mediump vec3 Ambient_SH4(mediump vec3 normal, mediump vec3 albedo, mediump float metallic, mediump vec4 shRed, mediump vec4 shGreen, mediump vec4 shBlue)
{
	mediump float factor = 1.0;
	mediump float x = normal.x;
	mediump float y = normal.y;
	mediump float z = normal.z;

	mediump vec4 basis = vec4(1.0, factor * y, factor * z, factor * x);
	mediump vec3 diffuse = vec3(dot(basis, shRed), dot(basis, shGreen), dot(basis, shBlue));

	return vec3(1.0 - metallic) * diffuse * albedo / PI;
}

mediump vec3 Ambient_SH9(mediump vec3 normal, mediump vec3 albedo, mediump float metallic, mediump vec3 shRed0, mediump vec3 shRed1, mediump vec3 shRed2, mediump vec3 shGreen0, mediump vec3 shGreen1, mediump vec3 shGreen2, mediump vec3 shBlue0, mediump vec3 shBlue1, mediump vec3 shBlue2)
{
	mediump float factor = 1.0;
	mediump float x = normal.x;
	mediump float y = normal.y;
	mediump float z = normal.z;

	mediump vec3 basis0 = vec3(1.0, factor * y, factor * z);
	mediump vec3 basis1 = vec3(factor * x, x * y, y * z);
	mediump vec3 basis2 = vec3(z * z * 3.0f - 1.0f, x * z, x * x - y * y);
	mediump vec3 diffuse = basis0 * mat3(shRed0, shGreen0, shBlue0) + basis1 * mat3(shRed1, shGreen1, shBlue1) + basis2 * mat3(shRed2, shGreen2, shBlue2);

	return vec3(1.0 - metallic) * diffuse * albedo / PI;
}

mediump vec3 Diffuse_Lambert(mediump vec3 albedo)
{
	return albedo / PI;
}

mediump float D_GGX(mediump float ndh, mediump float roughness)
{
	mediump float a = roughness * roughness;
	mediump float a2 = a * a;
	mediump float d = (ndh * a2 - ndh) * ndh + 1.0;
	return a2 / (PI * d * d);
}

mediump float G_Neumann(mediump float ndv, mediump float ndl)
{
	return 0.25 / max(ndv, ndl);
}

mediump float G_Schlick(mediump float ndv, mediump float ndl, mediump float roughness)
{
	mediump float k = (roughness * roughness) * 0.5;

	mediump float ggx1 = ndv * (1.0 - k) + k;
	mediump float ggx2 = ndl * (1.0 - k) + k;

	return 0.25 / (ggx1 * ggx2);
}

mediump float G_SchlickSmithGGX(mediump float ndv, mediump float ndl, mediump float roughness)
{
	mediump float r = roughness + 1.0;
	mediump float k = (r * r) / 8.0;

	mediump float ggx1 = ndv * (1.0 - k) + k;
	mediump float ggx2 = ndl * (1.0 - k) + k;

	return 0.25 / (ggx1 * ggx2);
}

mediump vec3 F_Schlick(mediump vec3 specular, mediump float ndv, mediump float metallic)
{
#ifdef FAST_LIGHTING
	mediump vec3 F0 = mix(vec3(0.04), specular, metallic);
	mediump float F1 = clamp(1.0 - ndv * 1.5, 0.0, 1.0);
	return F0 + (1.0 - F0) * F1 * F1 * F1;
#else
	mediump vec3 F0 = mix(vec3(0.04), specular, metallic);
	return F0 + (1.0 - F0) * pow(1.0 - ndv, 5.0);
#endif
}

mediump vec3 F_SchlickSkin(mediump float ndv)
{
#ifdef FAST_LIGHTING
	mediump vec3 F0 = vec3(0.028);
	mediump float F1 = clamp(1.0 - ndv * 1.5, 0.0, 1.0);
	return F0 + (1.0 - F0) * F1 * F1 * F1;
#else
	mediump vec3 F0 = vec3(0.028);
	return F0 + (1.0 - F0) * pow(1.0 - ndv, 5.0);
#endif
}

mediump vec3 F_SchlickRoughness(mediump vec3 specular, mediump float ndv, mediump float metallic, mediump float roughness)
{
#ifdef FAST_LIGHTING
	mediump vec3 F0 = mix(vec3(0.04), specular, metallic);
	mediump float F1 = clamp(1.0 - ndv * 1.5, 0.0, 1.0);
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * F1 * F1 * F1;
#else
	mediump vec3 F0 = mix(vec3(0.04), specular, metallic);
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - ndv, 5.0);
#endif
}

mediump vec3 Env_Specular(mediump vec3 specular, mediump float ndv, mediump float roughness)
{
#ifdef FAST_LIGHTING
	mediump vec4 c0 = vec4(-1.0, -0.0275, -0.572, 0.022);
	mediump vec4 c1 = vec4( 1.0,  0.0425,  1.04, -0.04 );
	mediump vec4 r = roughness * c0 + c1;
	mediump float a001 = clamp(0.3 / (ndv + 0.3), 0.0, 1.0);
	mediump float a004 = min(r.x * r.x, a001 * a001 * a001) * r.x + r.y;
	mediump vec2 AB = vec2(-1.04, 1.04) * a004 + r.zw;
	return specular * AB.x + AB.y;
#else
	mediump vec4 c0 = vec4(-1.0, -0.0275, -0.572, 0.022);
	mediump vec4 c1 = vec4( 1.0,  0.0425,  1.04, -0.04 );
	mediump vec4 r = roughness * c0 + c1;
	mediump float a004 = min(r.x * r.x, exp2(-9.28 * ndv)) * r.x + r.y;
	mediump vec2 AB = vec2(-1.04, 1.04) * a004 + r.zw;
	return specular * AB.x + AB.y;
#endif
}


// BRDF Functions
mediump vec3 AmbientSH4(mediump vec3 normal, mediump vec3 albedo, mediump float metallic)
{
	return Ambient_SH4((engine.ambientLightRotationMatrix * vec4(normal, 0.0)).xyz, albedo, metallic, engine.ambientLightSH0, engine.ambientLightSH3, engine.ambientLightSH6);
}

mediump vec3 AmbientSH9(mediump vec3 normal, mediump vec3 albedo, mediump float metallic)
{
	return Ambient_SH9((engine.ambientLightRotationMatrix * vec4(normal, 0.0)).xyz, albedo, metallic, engine.ambientLightSH0.xyz, engine.ambientLightSH1.xyz, engine.ambientLightSH2.xyz, engine.ambientLightSH3.xyz, engine.ambientLightSH4.xyz, engine.ambientLightSH5.xyz, engine.ambientLightSH6.xyz, engine.ambientLightSH7.xyz, engine.ambientLightSH8.xyz);
}

mediump vec3 Fresnel(mediump vec3 normal, mediump vec3 viewDirection, mediump vec3 specular, mediump float metallic)
{
	mediump float ndv = max(dot(normal, viewDirection), 0.0);
	return F_Schlick(specular, ndv, metallic);
}

mediump vec3 FresnelSkin(mediump vec3 normal, mediump vec3 viewDirection)
{
	mediump float ndv = max(dot(normal, viewDirection), 0.0);
	return F_SchlickSkin(ndv);
}

mediump vec3 FresnelRoughness(mediump vec3 normal, mediump vec3 viewDirection, mediump vec3 specular, mediump float metallic, mediump float roughness)
{
	mediump float ndv = max(dot(normal, viewDirection), 0.0);
	return F_SchlickRoughness(specular, ndv, metallic, roughness);
}

mediump vec3 PBRLighting(mediump vec3 normal, mediump vec3 viewDirection, mediump vec3 lightDirection, mediump vec3 lightColor, mediump vec3 albedo, mediump vec3 fresnel, mediump float metallic, mediump float roughness)
{
	mediump vec3 halfDirection = normalize(viewDirection + lightDirection);

	mediump float ndh = max(dot(normal, halfDirection), 0.0);
	mediump float ndv = max(dot(normal, viewDirection), 0.0);
	mediump float ndl = max(dot(normal, lightDirection), 0.0);

	mediump float D = D_GGX(ndh, roughness);
	mediump float G = G_SchlickSmithGGX(ndv, ndl, roughness);
	mediump vec3  F = fresnel;

	mediump vec3 diffuse = Diffuse_Lambert(albedo) * (1.0 - metallic) * (1.0 - fresnel);
	mediump vec3 specular = D * F * G;

	return (diffuse + specular) * ndl * lightColor;
}

mediump float PHBeckmann(mediump float ndoth, mediump float m)
{
	mediump float alpha = acos(ndoth);
	mediump float ta = tan(alpha);
	mediump float val = 1.0 / (m * m * pow(ndoth, 4.0)) * exp(-(ta * ta) / (m * m));
	return val;
}

mediump vec3 SkinLighting(mediump vec3 normalBase, mediump vec3 normalDetails, highp vec3 position, mediump vec3 viewDirection, mediump vec3 lightDirection, mediump vec3 lightColor, mediump vec3 albedo, mediump vec3 fresnel, mediump float roughness, mediump sampler2D texPreIntegratedSkinLUT)
{
	mediump vec3 halfDirection = normalize(viewDirection + lightDirection);

	mediump float ndl = dot(normalBase, lightDirection);
	mediump float ndh = dot(normalDetails, halfDirection);
	mediump float hdh = dot(viewDirection + lightDirection, viewDirection + lightDirection);
	mediump float wrapped_ndl = clamp(ndl * 0.5 + 0.5, 0.0, 1.0);

	highp float tuneCurvature = 0.001;
	highp float curvature = clamp(tuneCurvature * length(fwidth(normalBase)) / length(fwidth(position)), 0.0, 1.0);

	mediump vec3 diffuse = Diffuse_Lambert(albedo) * (1.0 - fresnel) * max(ndl, 0.0);
	mediump vec3 scatter = texture(texPreIntegratedSkinLUT, vec2(wrapped_ndl, curvature)).rgb * albedo * (1.0 - wrapped_ndl) * 1.0;
	mediump vec3 specular = max(pow(texture(texPreIntegratedSkinLUT, vec2(ndh, roughness)).a * 2.0, 10.0) * fresnel / hdh, 0.0) * max(ndl, 0.0) * 0.277778;

	return (diffuse + scatter + specular) * lightColor;
}

mediump vec3 EnvLighting(mediump vec3 normal, mediump vec3 viewDirection, mediump vec3 env, mediump vec3 albedo, mediump vec3 fresnel, mediump float roughness)
{
	mediump float ndv = max(dot(normal, viewDirection), 0.0);
	return Env_Specular(fresnel, ndv, roughness) * env * albedo;
}

mediump vec3 EnvLighting(mediump vec3 normal, mediump vec3 viewDirection, mediump vec3 albedo, mediump vec3 fresnel, mediump float roughness, mediump sampler2D texEnv, mediump float maxLods)
{
	mediump vec3 R = reflect(-viewDirection, normal);
	mediump vec3 env = Gamma2Linear(textureLod(texEnv, SphericalSampleing(R), roughness * maxLods).rgb);
	return EnvLighting(normal, viewDirection, env, albedo, fresnel, roughness);
}

mediump vec3 EnvLighting(mediump vec3 normal, mediump vec3 viewDirection, mediump vec3 albedo, mediump vec3 fresnel, mediump float roughness, mediump samplerCube texEnv, mediump float maxLods)
{
	mediump vec3 R = reflect(-viewDirection, normal);
	mediump vec3 env = Gamma2Linear(textureLod(texEnv, R, roughness * maxLods).rgb);
	return EnvLighting(normal, viewDirection, env, albedo, fresnel, roughness);
}

mediump float LightingAttenuation(mediump float dis)
{
	return Attenuation(dis, engine.mainPointLightAttenuation.x, engine.mainPointLightAttenuation.y, engine.mainPointLightAttenuation.z);
}

#endif
