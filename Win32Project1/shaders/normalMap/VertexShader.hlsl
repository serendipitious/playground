struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 eyeDir : DIRECTION;
	float3 worldPosition : POSITION;
};

struct VS_INPUT {
	float4 inPos : POSITION;
	float2 inTexCoord : TEXCOORD;
	float3 inNormal : NORMAL;
	float3 inTangent : TANGENT;
	float3 inBinormal : BINORMAL;
};

cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 normalTransform;
	float4 eyePosition;
};

VS_OUTPUT main(VS_INPUT input) {
	VS_OUTPUT output;
	output.pos = mul(input.inPos, WVP);
	output.texcoord = input.inTexCoord;
	output.normal = mul(input.inNormal, normalTransform);
	// TODO transform eyePosition?
	output.eyeDir = eyePosition - input.inPos;
	output.worldPosition = input.inPos;

	output.tangent = input.inTangent;
	output.binormal = input.inBinormal;
	return output;
}