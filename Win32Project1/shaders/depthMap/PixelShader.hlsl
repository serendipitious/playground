struct VS_OUTPUT {
	float4 pos : SV_POSITION;
	float4 lightVec : DIRECTION;
};


float4 main(VS_OUTPUT input) : SV_TARGET {
	return length(input.lightVec);
}
