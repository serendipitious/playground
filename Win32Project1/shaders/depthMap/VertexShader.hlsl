struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 lightVec : DIRECTION;
};


cbuffer cbPerObject : register(b0) {
	float4x4 world;
}

cbuffer cbPerObject : register(b1) {
	float4x4 lightView;
	float4x4 lightProject;
	float4 lightPos;
};

VS_OUTPUT main(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 inNormal : NORMAL) {
	VS_OUTPUT output;
	inPos.w = 1.0f;
	inPos = mul(inPos, world);
	inPos = mul(inPos, lightView);
	inPos = mul(inPos, lightProject);
	output.pos = inPos;
	output.lightVec = inPos;
	return output;
}