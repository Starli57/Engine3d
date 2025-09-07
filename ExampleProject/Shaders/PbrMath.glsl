
#include "Constants.glsl"

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

    float num = NdotV;
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

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// http://www.thetenthplanet.de/archives/1180
mat3 CalculateTBN( vec3 N, vec3 p, vec2 uv )
{
    // get edge vectors of the pixel triangle
    vec3 dp1 = dFdx( p );
    vec3 dp2 = dFdy( p );
    vec2 duv1 = dFdx( uv );
    vec2 duv2 = dFdy( uv );

    if (length(duv1) <= 1e-2) {
        duv1 = vec2(1.0, 0.0);
    }

    if (length(duv2) <= 1e-2) {
        duv2 = vec2(0.0, -1.0);
    }

    // solve the linear system
    vec3 dp2perp = cross( dp2, N );
    vec3 dp1perp = cross( N, dp1 );
    vec3 T = dp2perp * duv1.x + dp1perp * duv2.x;
    vec3 B = dp2perp * duv1.y + dp1perp * duv2.y;

    // construct a scale-invariant frame
    float invmax = inversesqrt( max( dot(T,T), dot(B,B) ) );

    // calculate handedness of the resulting cotangent frame
    float w = dot(cross(N, T), B) < 0.0 ? -1.0 : 1.0;

    // adjust tangent if needed
    T = T * w;

    if (gl_FrontFacing == false) {
        N *= -1.0f;
        T *= -1.0f;
        B *= -1.0f;
    }

    return mat3( T * invmax, B * invmax, N );
}

// https://github.com/PacktPublishing/3D-Graphics-Rendering-Cookbook-Second-Edition/blob/main/data/shaders/gltf/PBR.sp
vec3 ApplyNormalMap(vec3 n, vec3 v, vec3 normalSample, vec2 uv)
{
    vec3 map = normalize( 2.0 * normalSample - vec3(1.0) );
    mat3 TBN = CalculateTBN(n, v, uv);

    return normalize(TBN * map);
}