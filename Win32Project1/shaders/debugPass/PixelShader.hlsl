struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

Texture2D texture2d;
SamplerState samplerState;

float4 main(VS_OUTPUT input) : SV_TARGET {
	return texture2d.Sample(samplerState, input.texcoord);
}
