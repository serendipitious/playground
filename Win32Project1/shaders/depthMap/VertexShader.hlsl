struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 lightVec : DIRECTION;
};

cbuffer cbPerObject {
	float4x4 lightView;
	float4x4 lightProject;
	float4 lightPos;
};

VS_OUTPUT main(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 inNormal : NORMAL) {
	VS_OUTPUT output;
	inPos = mul(lightView, inPos);
	inPos = mul(lightProject, inPos);
	output.pos = inPos;
	output.lightVec = (lightPos - inPos) * 0.01;
	return output;
}