#version 450

#include "ShadowMapsMath.glsl"

precision highp float;
precision highp sampler2DShadow;

layout(location = 0) in vec3 inWorldPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 inVertexColor;
layout(location = 4) in vec3 inLightPosition;
layout(location = 5) in vec3 inLightDirection;
layout(location = 6) in vec3 inViewPos;
layout(location = 7) in vec4 inFragPosLightSpace;
layout(location = 8) in mat4 inLightMatrix;
layout(location = 12) in mat3 inTBN;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform sampler2D baseColorMapSampler;
layout(set = 1, binding = 1) uniform sampler2D metallicRoughnessMapSampler;
layout(set = 1, binding = 2) uniform sampler2D normalMapSampler;
layout(set = 1, binding = 3) uniform sampler2D lightOcclusionMapSampler;

layout(set = 1, binding = 4) uniform Material
{ 
	vec4 baseColorFactor;
} material;

layout(set = 2, binding = 0) uniform sampler2D shadowMapSampler;

const float baseColorLevel = 1.0;
const float ambientLevel = 0.03;
const float shadowsEffect = 0.35;

const float kPi = 3.14159265;
const float kShininess = 16.0;
const float kEnergyConservation = ( 2.0 + kShininess ) / ( 2.0 * kPi );

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a      = roughness*roughness;
	float a2     = a*a;
	float NdotH  = max(dot(N, H), 0.0);
	float NdotH2 = NdotH*NdotH;

	float num   = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = kPi * denom * denom;

	return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r*r) / 8.0;

	float num   = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2  = GeometrySchlickGGX(NdotV, roughness);
	float ggx1  = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
	vec3 baseColorMap = pow(texture(baseColorMapSampler, uv).rgb, vec3(2.2));
	vec3 metallicRoughnessMap = texture(metallicRoughnessMapSampler, uv).rgb;
	vec3 normalMap = texture(normalMapSampler, uv).rgb;
	float lightOcclusionMap = texture(lightOcclusionMapSampler, uv).r;

	vec3 normal = normalize(inNormal);
	//	if (normalMap != vec3(0.0))
	//	{
	//		normal = normalMap * 2.0 - 1.0;
	//		normal = normalize(inTBN * normal);
	//	}

	vec3 viewDir = normalize(inViewPos - inWorldPosition);
	vec3 lightDirection = normalize(inLightPosition - inWorldPosition);
	vec3 reflectLightDir = reflect(-lightDirection, normal);

	float baseColor = max(dot(normal, reflectLightDir), 0.0);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, baseColorMap, metallicRoughnessMap.b);
	vec3 Lo = vec3(0.0);

	vec3 h = normalize(viewDir + lightDirection);
	float distanceToLight = 1.0;//length(inLightPosition - inWorldPosition);
	float attenuation = 1.0 / (distanceToLight * distanceToLight);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 radiance = lightColor * attenuation;

	// cook-torrance brdf
	float NDF = DistributionGGX(normal, h, metallicRoughnessMap.g);
	float G = GeometrySmith(normal, viewDir, lightDirection, metallicRoughnessMap.g);
	vec3 F = fresnelSchlick(max(dot(h, viewDir), 0.0), F0);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metallicRoughnessMap.b;

	vec3 numerator = NDF * G * F;
	float denominator = max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDirection), 0.0) + 0.0001;
	vec3 specular = numerator / denominator;

	// add to outgoing radiance Lo
	float NdotL = max(dot(normal, lightDirection), 0.0);
	Lo += (kD * baseColorMap / kPi + specular) * radiance * NdotL;

	vec3 ambient = vec3(ambientLevel) * baseColorMap * lightOcclusionMap;
	vec3 color = ambient + Lo;
	color *= pcf(inWorldPosition, inLightMatrix, shadowMapSampler, shadowsEffect);
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / 2.2));

	outColor = vec4(
		color.r * material.baseColorFactor.r,
		color.g * material.baseColorFactor.g,
		color.b * material.baseColorFactor.b,
		material.baseColorFactor.a);
}