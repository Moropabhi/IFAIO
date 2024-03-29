struct VSOut
{
	float3 colour : Colour;
	float2 uv : UV;
	float4 pos : SV_Position;
};


VSOut main( float4 pos : Position, float3 colour: Colour )
{
	VSOut vso;
	vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
	vso.colour = colour;
	vso.uv = colour.xy;
	return vso;
}