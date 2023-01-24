struct VSOut
{
	float2 uv : UV;
	float4 pos : SV_Position;
};
cbuffer CBuf
{
	matrix transform;
};

VSOut main( float3 pos : Position)
{
	VSOut vso;
	vso.pos = mul(float4(pos, 1.0f),transform);
	vso.uv = pos.xy;
	return vso;
}