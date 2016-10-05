struct VS_IN
{
	float4 position		: POSITION0;
	float4 diffuse		: COLOR0;
	float4 specular		: COLOR1;
	float2 texCoord 	: TEXCOORD0;
};

struct VS_OUT
{
	float4 position		: POSITION0;
	float4 color		: COLOR0;
	float4 specular		: COLOR1;
	float2 texCoord 	: TEXCOORD;
};

struct PS_OUT
{
	float4 color		: COLOR0;
};

// Vertex shaders...
float4x4 finalMatrix;

// Pixel shaders...
sampler2D Tex0;
float4 colorMultiply;
float4 colorAdd;

VS_OUT vs_main( in VS_IN vs_in )
{
	VS_OUT vs_out;
	vs_out.position = mul( vs_in.position, finalMatrix );
	vs_out.texCoord = vs_in.texCoord;
	vs_out.color = vs_in.diffuse;
	vs_out.specular = vs_in.specular;
	return vs_out;
}

PS_OUT ps_nontextured( in VS_OUT ps_in )
{
	PS_OUT ps_out;
	ps_out.color = saturate( ps_in.color * colorMultiply + ps_in.specular + colorAdd );
	return ps_out;
}

PS_OUT ps_textured( in VS_OUT ps_in )
{
	PS_OUT ps_out;
	ps_out.color = saturate( tex2D( Tex0, ps_in.texCoord ) *ps_in.color * colorMultiply );
	ps_out.color.rgb = saturate( ps_out.color.rgb + ps_in.specular + colorAdd );
	return ps_out;
};

