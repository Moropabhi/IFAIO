
cbuffer CBuf
{
	float4 colour[6];
};

float4 main(uint tid:SV_PrimitiveID) : SV_TARGET
{
	return colour[tid/2];
}