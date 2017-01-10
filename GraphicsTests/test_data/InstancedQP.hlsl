struct VS_IN
{
	float3 position		: POSITION;
	float4 color		: COLOR;
	float4 q			: TEXCOORD0;
	float4 p			: TEXCOORD1;
};

struct VS_OUT
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
};

cbuffer Constants : register(b0)
{
	float4x4 worldMatrix;	
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

VS_OUT vs_main( in VS_IN vs_in )
{
	VS_OUT vs_out;

	float4x4 m = {
		{1, 0, 0, 0 },
		{0, 1, 0, 0 },
		{0, 0, 1, 0 },
		{0, 0, 0, 1 }
	};

	float4 q = vs_in.q;
	float4 p = vs_in.p;


	/*
	float sqw = q.w*q.w;
	float sqx = q.x*q.x;
	float sqy = q.y*q.y;
	float sqz = q.z*q.z;

	// invs (inverse square length) is only required if quaternion is not already normalised
	float invs = 1 / (sqx + sqy + sqz + sqw);
	m[0][0] = (sqx - sqy - sqz + sqw)*invs; // since sqw + sqx + sqy + sqz =1/invs*invs
	m[1][1] = (-sqx + sqy - sqz + sqw)*invs;
	m[2][2] = (-sqx - sqy + sqz + sqw)*invs;

	float tmp1 = q.x*q.y;
	float tmp2 = q.z*q.w;
	m[1][0] = 2.0 * (tmp1 + tmp2)*invs;
	m[0][1] = 2.0 * (tmp1 - tmp2)*invs;

	tmp1 = q.x*q.z;
	tmp2 = q.y*q.w;
	m[2][0] = 2.0 * (tmp1 - tmp2)*invs;
	m[0][2] = 2.0 * (tmp1 + tmp2)*invs;
	tmp1 = q.y*q.z;
	tmp2 = q.x*q.w;
	m[2][1] = 2.0 * (tmp1 + tmp2)*invs;
	m[1][2] = 2.0 * (tmp1 - tmp2)*invs;

	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = 0;

	m[3][3] = 1;
	*/

	m[0][3] = p.x;
	m[1][3] = p.y;
	m[2][3] = p.z;

	m = mul( m, worldMatrix );
	
	vs_out.position = float4( vs_in.position, 1.0f );
	vs_out.position = mul( vs_out.position, transpose( m ) );
	vs_out.position = mul( vs_out.position, transpose( viewMatrix ) );
	vs_out.position = mul( vs_out.position, transpose( projectionMatrix ) );	
	vs_out.color = vs_in.color;
	return vs_out;
}

float4 ps_main( in VS_OUT ps_in ) : SV_TARGET
{
	return saturate( ps_in.color );
}
