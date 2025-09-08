
float calculateShadows(vec4 projectedCoordinates, sampler2D shadowMapSampler)
{
    if (projectedCoordinates.x > 1.0 || projectedCoordinates.x < 0.0
        || projectedCoordinates.y > 1.0 || projectedCoordinates.y < 0.0
        || projectedCoordinates.z > 1.0 || projectedCoordinates.z < 0.0) 
        return 1.0;

    float shadowMap = texture(shadowMapSampler, projectedCoordinates.xy).r;	
    return step(projectedCoordinates.z, shadowMap);
}


float pcf(vec3 inWorldPosition, mat4 inLightMatrix, sampler2D shadowMapSampler)
{
    vec4 projectedCoordinates = inLightMatrix * vec4(inWorldPosition.xyz, 1.0);
    projectedCoordinates /= projectedCoordinates.w;
    projectedCoordinates.xy = 0.5 * projectedCoordinates.xy + 0.5;

    vec2 texSize = textureSize(shadowMapSampler, 0);
    float offset = ((1.0 / texSize)).x;

    float shadowSum = 0;
    for (float x = -1; x <= 1; x += 1.0)
        for (float y = -1; y <= 1; y += 1.0)
            shadowSum += calculateShadows(projectedCoordinates + vec4(vec2(x, y) * offset, 0, 0), shadowMapSampler);

    return shadowSum / 9.0;
}
