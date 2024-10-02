#version 450

layout(location = 0) in vec3 inLocalPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 inColor;
layout(location = 4) in vec3 inLightPosition;
layout(location = 5) in vec3 inViewPos;

layout(location = 0) out vec4 outColor;

layout(binding = 3) uniform sampler2D texSampler;

float ambientLevel = 0.05;
float diffuseLevel = 0.75;
float specularLevel = 0.2;

const float kPi = 3.14159265;
const float kShininess = 32.0;
const float kEnergyConservation = ( 2.0 + kShininess ) / ( 2.0 * kPi ); 

void main() 
{
	vec3 textureColor = texture(texSampler, uv).rgb;

	vec3 normal = normalize(inNormal);
	vec3 lightDir = normalize(inLightPosition - inLocalPosition);    
	vec3 reflectLightDir = reflect(-lightDir, normal); 
	vec3 viewDir = normalize(inViewPos - inLocalPosition);

	float diff = max(dot(normal, lightDir), 0.0);

	//Blinn-Phong specular
//	vec3 halfwayDir = normalize(lightDir + viewDir);  
//	float specular = pow(max(dot(normal, halfwayDir), 0.0), kShininess);

	//energy conservant specular
	float specular = kEnergyConservation * pow(max(dot(viewDir, reflectLightDir), 0.0), kShininess);

	//phong specular
//	float specular = pow(max(dot(viewDir, reflectLightDir), 0.0), kShininess);

	vec3 result = (ambientLevel + diff * diffuseLevel + specular * specularLevel) * inColor * textureColor;
	outColor = vec4(result.rgb, 1.0);
}