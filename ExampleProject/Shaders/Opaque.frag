#version 450

#include "PbrMath.glsl"
#include "ShadowMapsMath.glsl"
#include "Constants.glsl"

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
layout(set = 1, binding = 4) uniform sampler2D emissionMapSampler;

layout(set = 1, binding = 5) uniform Material
{ 
	vec4 baseColorFactor;
	float metallicFactor;
	float roughnessFactor;
	float alphaCutoffFactor;
} material;

layout(set = 2, binding = 0) uniform sampler2D shadowMapSampler;

const float baseColorLevel = 1.0;
const float ambientLevel = 0.03;
const float shadowsEffect = 0.35;
const float gamma = 2.2;

void main()
{
	
	vec4 baseColorMap = pow(texture(baseColorMapSampler, uv), vec4(gamma));
	baseColorMap = vec4(
		baseColorMap.r * material.baseColorFactor.r * inVertexColor.r,
		baseColorMap.g * material.baseColorFactor.g * inVertexColor.g,
		baseColorMap.b * material.baseColorFactor.b * inVertexColor.b,
		baseColorMap.a * material.baseColorFactor.a);

	if (material.alphaCutoffFactor > baseColorMap.a)
	{
		discard;
	}
	
	vec3 metallicRoughnessMap = texture(metallicRoughnessMapSampler, uv).rgb;
	vec3 normalMap = texture(normalMapSampler, uv).rgb;
	vec3 emissiveMap = texture(emissionMapSampler, uv).rgb;
		
	float lightOcclusionMap = texture(lightOcclusionMapSampler, uv).r;

	float metalness = metallicRoughnessMap.b * material.metallicFactor;
	float roughness = metallicRoughnessMap.g * material.roughnessFactor;
	
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
	F0 = mix(F0, baseColorMap.rgb, metalness);
	vec3 Lo = vec3(0.0);

	vec3 h = normalize(viewDir + lightDirection);
	float distanceToLight = length(inLightPosition - inWorldPosition);
	float attenuation = 10000.0 / distanceToLight;// / (distanceToLight * distanceToLight);
	vec3 lightColor = vec3(1.0, 1.0, 1.0);
	vec3 radiance = lightColor * attenuation;

	// cook-torrance brdf
	float NDF = DistributionGGX(normal, h, roughness);
	float G = GeometrySmith(normal, viewDir, lightDirection, roughness);
	vec3 F = FresnelSchlick(max(dot(h, viewDir), 0.0), F0);

	vec3 kS = F;
	vec3 kD = vec3(1.0) - kS;
	kD *= 1.0 - metalness;

	vec3 numerator = NDF * G * F;
	float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDirection), 0.0) + 0.0001;
	vec3 specular = numerator / denominator;

	// add to outgoing radiance Lo
	float NdotL = max(dot(normal, lightDirection), 0.0);
	Lo += (kD * baseColorMap.rgb / kPi + specular) * radiance * NdotL;

	vec3 ambient = vec3(ambientLevel) * baseColorMap.rgb * lightOcclusionMap;
	vec3 color = ambient + Lo;
	
	color *= pcf(inWorldPosition, inLightMatrix, shadowMapSampler, shadowsEffect);
	color += emissiveMap;
	
	color = color / (color + vec3(1.0));
	color = pow(color, vec3(1.0 / gamma));

	outColor = vec4(color.rgb, material.baseColorFactor.a);
}