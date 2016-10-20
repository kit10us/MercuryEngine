struct VS_IN
{
	float3 position		: POSITION0;
	float4 color		: COLOR0;
};

struct VS_OUT
{
	float4 position		: POSITION0;
	float4 color		: COLOR0;
};

struct PS_OUT
{
	float4 color		: COLOR0;
};

sampler2D Tex0;

float4x4 finalMatrix;

VS_OUT vs_main( in VS_IN vs_in )
{
	VS_OUT vs_out;
	vs_out.position = mul( float4( vs_in.position, 1.0f ), finalMatrix );
	vs_out.color = vs_in.color.bgra;
	return vs_out;
}

PS_OUT ps_main( in VS_OUT ps_in )
{
	PS_OUT ps_out;
	ps_out.color = ps_in.color;//= saturate( ps_in.color );
	return ps_out;
}
