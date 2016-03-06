struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float2 texcoord : TEXCOORD;
	float3 normal : NORMAL;
	float3 eyeDir : DIRECTION;
	float3 worldPosition : POSITION;
	float4 posInLight : POSITION2;
};

cbuffer cbPerObject : register(b0)
{
	float4x4 WVP;
	float4x4 normalTransform;
	float4 eyePosition;
};

cbuffer cbPerObject : register(b1)
{
	float4x4 lightView;
	float4x4 lightProject;
	float4 lightPos;
};


VS_OUTPUT main(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 inNormal : NORMAL) {
	VS_OUTPUT output;
	output.pos = mul(inPos, WVP);
	output.texcoord = inTexCoord;
	output.normal = mul(inNormal, normalTransform);
	// TODO transform eyePosition?
	output.eyeDir = eyePosition - inPos;
	output.worldPosition = inPos;


	inPos = mul(inPos, lightView);
	inPos = mul(inPos, lightProject);
	output.posInLight = inPos;

	return output;
}