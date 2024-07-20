#version 450

layout(location = 0) in vec3 inLocalPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 inColor;
layout(location = 4) in vec3 inLightPosition;

layout(location = 0) out vec4 outColor;

float minLightLevel = 0.1;
float sumLevel = 1 + minLightLevel;

void main() 
{
	vec3 norm = normalize(inNormal);
	vec3 lightDir = normalize(inLightPosition - inLocalPosition);  
	float diff = max(dot(norm, lightDir), 0.0);
	float lightLevel = (diff + minLightLevel) / sumLevel;

	vec3 result = lightLevel * inColor;
	outColor = vec4(result.rgb, 1.0);
}