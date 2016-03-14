struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 eyeDir : DIRECTION;
	float3 worldPosition : POSITION;
	float4 posInLight : POSITION2;
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

Texture2D groundTexture : register(t0);
SamplerState groundState : register(s0);

Texture2D depthTexture : register(t1);
SamplerState depthState : register(s1);

float4 main(VS_OUTPUT input) : SV_TARGET {
	input.normal = normalize(input.normal);
	float4 color = groundTexture.Sample(groundState, input.texcoord);
	float4 ambient = color * light.ambient;
	float3 lightDir = light.position - input.worldPosition;
	float4 diffuseColor = clamp(dot(-lightDir, input.normal), 0, 1) * color * light.diffuse;
	float3 reflectDir = normalize(reflect(lightDir, input.normal));
	float4 specular = pow(clamp(dot(reflectDir, normalize(input.eyeDir)), 0, 1), 5) * color;

	float2 projCoord;
	projCoord.x = input.posInLight.x / input.posInLight.w / 2.0f + 0.5f;
	projCoord.y = -input.posInLight.y / input.posInLight.w / 2.0f + 0.5f;

	float4 finalColor = clamp(ambient + diffuseColor + specular, 0, 1);

	float depthInTexture = depthTexture.Sample(depthState, projCoord).r;
	float distToLight = input.posInLight.z / input.posInLight.w - 0.1f;   // bias
	if (distToLight < depthInTexture) {
		finalColor *= 0.4;
	}
	return finalColor;

}
