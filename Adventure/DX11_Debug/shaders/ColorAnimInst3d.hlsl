struct VS_IN
{
	float3 position		: POSITION;
	float4 color		: COLOR;
	float4 matInf		: MatInf;
	matrix <float, 4, 4> world[2]		: Matrix;
	uint instanceID     : SV_InstanceID;
};

struct VS_OUT
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR;
};

cbuffer Constants : register(b0)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

VS_OUT vs_main( in VS_IN vs_in )
{
	VS_OUT vs_out;

	// Initial position from input.
	vs_out.position = float4( vs_in.position, 1.0f );

	// Affect position as necessary by skinning...

	int mat1_index = floor( vs_in.matInf.y );
	if( mat1_index > -1 )
 	{
		float1 influence = (vs_in.matInf.y - mat1_index ) * 10.0f;
		//mat1_index += vs_in.instanceID * 2;
		vs_out.position += mul( vs_out.position, transpose( vs_in.world[mat1_index] ) * influence );
	}

	/*
	int mat2_index = floor( vs_in.matInf.z );
	if( mat2_index > -1 )
	{
		float1 influence = (vs_in.matInf.z - mat2_index ) * 10.0f;
		float4x4 m = vs_in.world[mat2_index] * influence;
		final = m * final;
	}
	*/

	// Affect position by 0, as is world transform.
	vs_out.position = mul( vs_out.position, transpose( vs_in.world[0] ) );

	vs_out.position = mul( vs_out.position, transpose( viewMatrix ) );
	vs_out.position = mul( vs_out.position, transpose( projectionMatrix ) );	
	vs_out.color = vs_in.color;
	return vs_out;
}

float4 ps_main( in VS_OUT ps_in ) : SV_TARGET
{
	return saturate( ps_in.color );
}
