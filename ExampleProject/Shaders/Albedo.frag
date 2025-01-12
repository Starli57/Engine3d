#version 450

precision highp float;
precision highp sampler2DShadow;

layout(location = 0) in vec3 inLocalPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 inDiffuseColor;
layout(location = 4) in vec3 inLightPosition;
layout(location = 5) in vec3 inLightDirection;
layout(location = 6) in vec3 inViewPos;
layout(location = 7) in vec4 inFragPosLightSpace;
layout(location = 8) in mat4 inLightMatrix;
layout(location = 12) in mat3 inTBN;

layout(location = 0) out vec4 outColor;

layout(binding = 3) uniform sampler2D shadowMapSampler;
layout(binding = 4) uniform sampler2D diffuseMapSampler;
layout(binding = 5) uniform sampler2D specularMapSampler;
layout(binding = 6) uniform sampler2D normalMapSampler;
layout(binding = 7) uniform sampler2D alphaMapSampler;

const float ambientLevel = 0.05;
const float diffuseLevel = 0.75;
const float specularLevel = 0.75;

const float shadowsEffect = 0.5;

const float kPi = 3.14159265;
const float kShininess = 32.0;
const float kEnergyConservation = ( 2.0 + kShininess ) / ( 2.0 * kPi ); 

float calculateShadows(vec4 projectedCoordinates)
{
	if (projectedCoordinates.x > 1.0 || projectedCoordinates.x < 0.0
		|| projectedCoordinates.y > 1.0 || projectedCoordinates.y < 0.0
		|| projectedCoordinates.z > 1.0 || projectedCoordinates.z < 0.0) 
		return 1.0;

    float shadowMap = texture(shadowMapSampler, projectedCoordinates.xy).r;	
	return step(projectedCoordinates.z, shadowMap);
}

float pcf()
{
	vec4 projectedCoordinates = inLightMatrix * vec4(inLocalPosition.xyz, 1.0);
    projectedCoordinates /= projectedCoordinates.w;
    projectedCoordinates.xy = 0.5 * projectedCoordinates.xy + 0.5;
	
	vec2 texSize = textureSize(shadowMapSampler, 0);
	float offset = ((1.0 / texSize)).x;

	float shadowSum = 0;
	for (float x = -1.5; x <= 1.5; x += 1.0)
		for (float y = -1.5; y <= 1.5; y += 1.0)
		//	for (float z = -1.5; z <= 1.5; z += 1.0)
				shadowSum += calculateShadows(projectedCoordinates + vec4(vec2(x, y) * offset, 0, 0));

	return max(shadowsEffect, shadowSum / 16.0);
}

void main() 
{
	vec3 diffuseMap = texture(diffuseMapSampler, uv).rgb;
	vec3 specularMap = texture(specularMapSampler, uv).rgb;
	vec3 normalMap = texture(normalMapSampler, uv).rgb;
	vec3 alphaMap = texture(alphaMapSampler, uv).rgb;

	vec3 normal = normalize(inNormal);
	if (normalMap != vec3(0.0))
	{
		normal = normalMap * 2.0 - 1.0;
		normal = normalize(inTBN * normal);
	}

	vec3 lightDirection = normalize(inLightPosition - inLocalPosition);
	vec3 reflectLightDir = reflect(-lightDirection, normal); 
	vec3 viewDir = normalize(inViewPos - inLocalPosition);

	float diffuse = max(dot(normal, reflectLightDir), 0.0);
	float specular = kEnergyConservation * pow(max(dot(viewDir, reflectLightDir), 0.0), kShininess);
	
	vec3 color = (ambientLevel * diffuseMap + diffuse * diffuseLevel * diffuseMap * inDiffuseColor + specular * specularLevel * specularMap) * pcf();
	color = vec3(color.r * alphaMap.r, color.g * alphaMap.g, color.b * alphaMap.b);

	outColor = vec4(color.rgb, alphaMap.r);
}