struct VSOut
{
	float3 colour : Colour;
	float2 uv : UV;
	float4 pos : SV_Position;
};
cbuffer CBuf
{
	matrix transform;
	float Time;
};

VSOut main( float3 pos : Position, float3 colour: Colour )
{
	VSOut vso;
	vso.pos = mul(float4(pos, 1.0f),transform);
	vso.colour = colour;
	vso.uv = colour.xy;
	return vso;
}