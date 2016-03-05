struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 eyeDir : DIRECTION;
	float3 worldPosition : POSITION;
};

cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 normalTransform;
	float4 eyePosition;
};

VS_OUTPUT main(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 inNormal : NORMAL) {
	VS_OUTPUT output;
	output.pos = mul(inPos, WVP);
	output.texcoord = inTexCoord;
	output.normal = mul(inNormal, normalTransform);
	// TODO transform eyePosition?
	output.eyeDir = eyePosition - inPos;
	output.worldPosition = inPos;
	return output;
}