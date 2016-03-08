struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 eyeDir : DIRECTION;
	float3 worldPosition : POSITION;
};

struct Light {
	float3 position;
	float diffuse;
	float4 ambient;
};

cbuffer cbPerFrame {
	Light light;
	float4 eye;
};

Texture2D texture2d : register(t0);
SamplerState samplerState : register(s0);

Texture2D normalMap : register(t1);
SamplerState normalMapState : register(s1);

float4 main(VS_OUTPUT input) : SV_TARGET {
	float4 bump = normalMap.Sample(normalMapState, input.texcoord);
	bump = bump * 2.0f - 1.0f;
	input.normal = (bump.x * input.tangent) + (bump.y * input.binormal) + (bump.z * input.normal);

	input.normal = normalize(input.normal);

	//float4 color = bump;
	float4 color = texture2d.Sample(samplerState, input.texcoord);
	float4 ambient = color * light.ambient;
	float3 lightDir = normalize(light.position - input.worldPosition);
	float4 diffuseColor = clamp(dot(lightDir, input.normal), 0, 1) * color * light.diffuse;
	float3 reflectDir = -normalize(reflect(lightDir, input.normal));
	float4 specular = pow(clamp(dot(reflectDir, normalize(input.eyeDir)), 0, 1), 5) * color;

	return clamp(ambient + diffuseColor + specular, 0, 1);
}
