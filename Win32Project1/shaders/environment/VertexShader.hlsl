struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float3 texcoord : TEXCOORD;
};

cbuffer cbPerObject : register(b0)
{
	float4x4 WVP;
};

cbuffer cameraTranslation : register(b1) {
	float4x4 cameraTrans;
};

VS_OUTPUT main(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 inNormal : NORMAL) {
	VS_OUTPUT output;
	output.pos = mul(inPos, mul(cameraTrans, WVP)).xyww;
	output.texcoord = inPos;
	return output;
}