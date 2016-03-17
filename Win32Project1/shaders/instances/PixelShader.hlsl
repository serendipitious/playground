struct VS_OUTPUT {
	float4 pos : SV_POSITION;
};

float4 main(VS_OUTPUT input) : SV_TARGET {
	return float4(0.3, 0.5, 0.7, 1.0);
}
