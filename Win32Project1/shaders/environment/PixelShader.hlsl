struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float3 texcoord : TEXCOORD;
};


TextureCube SkyMap;
SamplerState samplerState;

float4 main(VS_OUTPUT input) : SV_TARGET {
	return SkyMap.Sample(samplerState, input.texcoord);
}
