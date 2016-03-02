struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 eyeDir : DIRECTION;
};

struct Light {
	float3 dir;
	float diffuse;
	float4 ambient;
};

cbuffer cbPerFrame {
	Light light;
	float4 eye;
};

Texture2D texture2d;
SamplerState samplerState;

float4 main(VS_OUTPUT input) : SV_TARGET {
	input.normal = normalize(input.normal);
	float4 color = texture2d.Sample(samplerState, input.texcoord);
	float4 ambient = color * light.ambient;
	float4 diffuseColor = clamp(dot(-light.dir, input.normal), 0, 1) * color * light.diffuse;
	float3 reflectDir = normalize(reflect(light.dir, input.normal));
	float4 specular = pow(clamp(dot(reflectDir, normalize(input.eyeDir)), 0, 1), 10) * color;

	return clamp(ambient + diffuseColor + specular, 0, 1);
}
