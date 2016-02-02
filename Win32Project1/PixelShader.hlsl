struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
};

struct Light {
	float3 dir;
	float4 ambient;
	float4 diffuse;
};

cbuffer cbPerFrame {
	Light light;
};

Texture2D texture2d;
SamplerState samplerState;

float4 main(VS_OUTPUT input) : SV_TARGET {
	input.normal = normalize(input.normal);
	float4 diffuse = texture2d.Sample(samplerState, input.texcoord);

	float3 finalColor = diffuse * light.ambient;
	finalColor += saturate(dot(light.dir, input.normal) * light.diffuse * diffuse);

	return float4(finalColor, diffuse.a);
}
