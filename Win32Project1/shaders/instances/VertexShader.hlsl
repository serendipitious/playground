struct VS_OUTPUT {
	float4 pos : SV_POSITION;
};

cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 normalTransform;
	float4 eyePosition;
};

VS_OUTPUT main(float3 inPos : POSITION, float3 offset : INSTANCEPOS) {
	VS_OUTPUT output;
	output.pos = mul(float4(inPos + offset, 1.0f), WVP);
	return output;
}