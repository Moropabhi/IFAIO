float4 main(float3 colour:Colour, float2 uv : UV) : SV_TARGET
{
	return float4(colour,1.0);
}