#version 450

layout(location = 0) in vec3 inLocalPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 inColor;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

float minLightLevel = 0.1;
float sumLevel = 1 + minLightLevel;

void main() 
{
	vec3 textureColor = texture(texSampler, uv).rgb;

	vec3 lightPos = vec3(400, 500, 200);

	vec3 norm = normalize(inNormal);
	vec3 lightDir = normalize(lightPos - inLocalPosition);  
	float diff = max(dot(norm, lightDir), 0.0);
	float lightLevel = (diff + minLightLevel) / sumLevel;

	vec3 result = lightLevel * textureColor;
	outColor = vec4(result.rgb, 1.0);
}