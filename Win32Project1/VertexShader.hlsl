struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

cbuffer cbPerObject
{
	float4x4 WVP;
};

VS_OUTPUT main(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD) {
	VS_OUTPUT output;
	output.pos = mul(inPos, WVP);
	output.texcoord = inTexCoord;
	return output;
}