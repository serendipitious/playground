struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 lightVec : DIRECTION;
};


float4 main(VS_OUTPUT input) : SV_TARGET {
	float depthValue = input.lightVec.z / input.lightVec.w;
	return float4(depthValue, depthValue, depthValue, 1.0f);
}
