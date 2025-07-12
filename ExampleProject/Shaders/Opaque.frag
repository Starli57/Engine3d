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

layout(set = 1, binding = 0) uniform sampler2D diffuseMapSampler;
layout(set = 1, binding = 1) uniform sampler2D specularMapSampler;
layout(set = 1, binding = 2) uniform sampler2D normalMapSampler;
layout(set = 1, binding = 3) uniform sampler2D alphaMapSampler;

layout(set = 1, binding = 4) uniform Material
{ 
	float inTransparency;
} material;

layout(set = 2, binding = 0) uniform sampler2D shadowMapSampler;

const float ambientLevel = 0.05;
const float diffuseLevel = 1.0;
const float specularLevel = 1.0;

const float shadowsEffect = 0.35;

const float kPi = 3.14159265;
const float kShininess = 16.0;
const float kEnergyConservation = ( 2.0 + kShininess ) / ( 2.0 * kPi );

void main() 
{
	vec3 diffuseMap = texture(diffuseMapSampler, uv).rgb;
	vec3 specularMap = texture(specularMapSampler, uv).rgb;
	vec3 normalMap = texture(normalMapSampler, uv).rgb;
	vec4 alphaMap = texture(alphaMapSampler, uv).rgba;

	vec3 normal = normalize(inNormal);
//	if (normalMap != vec3(0.0))
//	{
//		normal = normalMap * 2.0 - 1.0;
//		normal = normalize(inTBN * normal);
//	}

	vec3 lightDirection = normalize(inLightPosition - inWorldPosition);
	vec3 reflectLightDir = reflect(-lightDirection, normal); 
	vec3 viewDir = normalize(inViewPos - inWorldPosition);

	float diffuse = max(dot(normal, reflectLightDir), 0.0);
	float specular = kEnergyConservation * pow(max(dot(viewDir, reflectLightDir), 0.0), kShininess);
	
	vec3 color = (ambientLevel * diffuseMap + diffuse * diffuseLevel * diffuseMap * inVertexColor + specular * specularLevel * specularMap);
	color *= pcf(inWorldPosition, inLightMatrix, shadowMapSampler, shadowsEffect);
	color = vec3(color.r * alphaMap.r, color.g * alphaMap.g, color.b * alphaMap.b);
	
	outColor = vec4(color.rgb, alphaMap.r * alphaMap.a * material.inTransparency);
}