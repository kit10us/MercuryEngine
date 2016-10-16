// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/VertexDeclaration.h>
#include <dxi/VertexShader.h>
#include <dxi/win/DXDevice.h>
#include <dxi/core/Game.h>
#include <unify/ColorUnit.h>

using namespace dxi;


VertexElement CommonVertexElement::Position( unsigned int slot )
{
#if defined( DIRECTX9 )
	VertexElement positionE = {};
	positionE.Stream = slot;
	positionE.Type = D3DDECLTYPE_FLOAT3;
	positionE.Usage = D3DDECLUSAGE_POSITION;
	positionE.UsageIndex = 0;
	return positionE;
#elif defined( DIRECTX11 )
	VertexElement positionE = {};
	positionE.InputSlot = slot;
	positionE.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	positionE.SemanticName = "POSITION";
	positionE.SemanticIndex = 0;
	positionE.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	return positionE;
#endif
}

VertexElement CommonVertexElement::Normal( unsigned int slot )
{
#if defined( DIRECTX9 )
	VertexElement normalE = {};
	normalE.Stream = slot;
	normalE.Type = D3DDECLTYPE_FLOAT3;
	normalE.Usage = D3DDECLUSAGE_NORMAL;
	normalE.UsageIndex = 0;
	return normalE;
#elif defined( DIRECTX11 )
	VertexElement normalE = {};
	normalE.InputSlot = slot;
	normalE.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	normalE.SemanticName = "NORMAL";
	normalE.SemanticIndex = 0;
	normalE.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	return normalE;
#endif
}

VertexElement CommonVertexElement::Diffuse( unsigned int slot )
{
#if defined( DIRECTX9 )
	VertexElement diffuseE = {};
	diffuseE.Stream = slot;
	diffuseE.Type = D3DDECLTYPE_D3DCOLOR;
	diffuseE.Usage = D3DDECLUSAGE_COLOR;
	diffuseE.UsageIndex = 0;
	return diffuseE;
#elif defined( DIRECTX11 )
	VertexElement diffuseE = {};
	diffuseE.InputSlot = slot;
	diffuseE.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	diffuseE.SemanticName = "COLOR";
	diffuseE.SemanticIndex = 0;
	diffuseE.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	return diffuseE;
#endif
}

VertexElement CommonVertexElement::Specular( unsigned int slot )
{
#if defined( DIRECTX9 )
	VertexElement specularE = {};
	specularE.Stream = slot;
	specularE.Type = D3DDECLTYPE_D3DCOLOR;
	specularE.Usage = D3DDECLUSAGE_COLOR;
	specularE.UsageIndex = 1;
	return specularE;
#elif defined( DIRECTX11 )
	VertexElement specularE = {};
	specularE.InputSlot = slot;
	specularE.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	specularE.SemanticName = "COLOR";
	specularE.SemanticIndex = 1;
	specularE.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	return specularE;
#endif
}

VertexElement CommonVertexElement::TexCoords( unsigned int slot )
{
#if defined( DIRECTX9 )
	VertexElement texE = {};
	texE.Stream = slot;
	texE.Type = D3DDECLTYPE_FLOAT2;
	texE.Usage = D3DDECLUSAGE_TEXCOORD;
	texE.UsageIndex = 0;
	return texE;
#elif defined( DIRECTX11 )
	VertexElement texE = {};
	texE.InputSlot = slot;
	texE.Format = DXGI_FORMAT_R32G32_FLOAT;
	texE.SemanticName = "TEXCOORD";
	texE.SemanticIndex = 0;
	texE.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	return texE;
#endif
}

VertexElement CommonVertexElement::TexCoords2( unsigned int slot )
{
#if defined( DIRECTX9 )
	VertexElement texE2 = {};
	texE2.Stream = slot;
	texE2.Type = D3DDECLTYPE_FLOAT2;
	texE2.Usage = D3DDECLUSAGE_TEXCOORD;
	texE2.UsageIndex = 1;
	return texE2;
#elif defined( DIRECTX11 )
	VertexElement texE2 = {};
	texE2.InputSlot = slot;
	texE2.Format = DXGI_FORMAT_R32G32_FLOAT;
	texE2.SemanticName = "TEXCOORD";
	texE2.SemanticIndex = 1;
	texE2.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	return texE2;
#endif
}

#if defined( DIRECTX9 )
typedef D3DDECLTYPE Format;
#elif defined( DIRECTX11 )
typedef DXGI_FORMAT Format;
#endif

struct WriteMethod
{
	enum TYPE
	{
		Float1,
		Float2,
		Float3,
		Float4,
		Color,
		Unknown,
	};

#if defined( DIRECTX9 )
	static TYPE FromElement( const VertexElement & element )
	{
		switch( element.Type )
		{
		case D3DDECLTYPE_FLOAT1: return Float1;
		case D3DDECLTYPE_FLOAT2: return Float2;
		case D3DDECLTYPE_FLOAT3: return Float3;
		case D3DDECLTYPE_FLOAT4: return Float4;
		case D3DDECLTYPE_D3DCOLOR: return Color;
		case D3DDECLTYPE_UNUSED: return Unknown;
		}
		assert( 0 );
		throw 0;
	}
#elif defined( DIRECTX11 )
	static TYPE FromElement( const VertexElement & element )
	{
		switch( element.Format )
		{
		case DXGI_FORMAT_R32_FLOAT: return Float1;
		case DXGI_FORMAT_R32G32_FLOAT: return Float2;
		case DXGI_FORMAT_R32G32B32_FLOAT: return Float3;
		case DXGI_FORMAT_R32G32B32A32_FLOAT: return Float4;
		case DXGI_FORMAT_R8G8B8A8_UNORM: return Color;
		}
		assert( 0 );
		throw 0;
	}
#endif
};

size_t Offset( const VertexElement & element )
{
#if defined( DIRECTX9 )
	return element.Offset;
#elif defined( DIRECTX11 )
	return element.AlignedByteOffset;
#endif
}

// This is the automatically assumed size of the
size_t SizeOf( Format format )
{
#if defined( DIRECTX9 )
	switch( format )
	{
	case D3DDECLTYPE_FLOAT1:
		return sizeof( float );
	case D3DDECLTYPE_FLOAT2:
		return sizeof( float ) * 2;
	case D3DDECLTYPE_FLOAT3:
		return sizeof( float ) * 3;
	case D3DDECLTYPE_FLOAT4:
		return sizeof( float ) * 4;
	case D3DDECLTYPE_D3DCOLOR:
		return sizeof( D3DCOLOR );
	case D3DDECLTYPE_UBYTE4:
		return sizeof( unsigned char ) * 4;
	case D3DDECLTYPE_SHORT2:
		return sizeof( short ) * 2;
	case D3DDECLTYPE_SHORT4:
		return sizeof( short ) * 4;
	case D3DDECLTYPE_UBYTE4N:
		return sizeof( unsigned char ) * 4;
	case D3DDECLTYPE_SHORT2N:
		return sizeof( short ) * 2;
	case D3DDECLTYPE_SHORT4N:
		return sizeof( short ) * 4;
	case D3DDECLTYPE_USHORT2N:
		return sizeof( unsigned short ) * 2;
	case D3DDECLTYPE_USHORT4N:
		return sizeof( unsigned short ) * 4;
	case D3DDECLTYPE_UDEC3:
		return 4;
	case D3DDECLTYPE_DEC3N:
		return 4;
	case D3DDECLTYPE_FLOAT16_2:
		return 2 * 2;
	case D3DDECLTYPE_FLOAT16_4:
		return 2 * 4;
	case D3DDECLTYPE_UNUSED:
		return 0;
	default:
		throw unify::Exception( "Vertex declaration type not support!" );
	}
#elif defined( DIRECTX11 )	
	switch( format )
	{
	case DXGI_FORMAT_R32_FLOAT:
		return sizeof( float );
	case DXGI_FORMAT_R32G32_FLOAT:
		return sizeof( float ) * 2;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		return sizeof( float ) * 3;
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return sizeof( float ) * 4;
	case DXGI_FORMAT_R8G8B8A8_UNORM:
		return sizeof( unsigned int );
	case DXGI_FORMAT_R8G8B8A8_UINT:
		return sizeof( unsigned char ) * 4;
	case DXGI_FORMAT_R16G16_SINT:
		return sizeof( short ) * 2;
	case DXGI_FORMAT_R16G16B16A16_SINT:
		return sizeof( short ) * 4;
	case DXGI_FORMAT_R16G16_SNORM:
		return sizeof( short ) * 2;
	case DXGI_FORMAT_R16G16B16A16_SNORM:
		return sizeof( short ) * 4;
	case DXGI_FORMAT_R16G16_UNORM:
		return sizeof( unsigned short ) * 2;
	case DXGI_FORMAT_R16G16B16A16_UNORM:
		return sizeof( unsigned short ) * 4;
	case DXGI_FORMAT_R10G10B10A2_UINT:
		return 4;
	case DXGI_FORMAT_R10G10B10A2_UNORM:
		return 4;
	case DXGI_FORMAT_R16G16_FLOAT:
		return 4;
	case DXGI_FORMAT_R16G16B16A16_FLOAT:
		return 8;
	case DXGI_FORMAT_UNKNOWN:
		return 0;
	default:
		throw unify::Exception( "Vertex declaration type not support!" );
	}
#endif
}

size_t SizeOf( VertexElement element )
{
#if defined( DIRECTX9 )
	return SizeOf( (Format)element.Type );
#elif defined( DIRECTX11 )
	return SizeOf( element.Format );
#endif
}

void ConvertType( const std::string & type, Format & format, size_t & size )
{
	if( unify::StringIsAny( type, { "FLOAT1", "FLOAT", "R32_FLOAT" } ) )
	{
#if defined( DIRECT9 )
		format = D3DDECLTYPE_FLOAT1;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R32_FLOAT;
#endif
	}
	else if( unify::StringIsAny( type, { "FLOAT2", "TEXCOORD", "R32G32_FLOAT" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_FLOAT2;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R32G32_FLOAT;
#endif
	}
	else if( unify::StringIsAny( type, { "FLOAT3", "R32G32B32_FLOAT" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_FLOAT3;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R32G32B32_FLOAT;
#endif
	}
	else if( unify::StringIsAny( type, { "FLOAT4", "COLORUNIT", "R32G32B32A32_FLOAT" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_FLOAT4;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R32G32B32A32_FLOAT;
#endif
	}
	else if( unify::StringIsAny( type, { "D3DCOLOR", "COLOR", "R8G8B8A8_UNORM" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_D3DCOLOR;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif
	}
	else if( unify::StringIsAny( type, { "UBYTE4", "R8G8B8A8_UINT" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_UBYTE4;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R8G8B8A8_UINT;
#endif
	}
	else if( unify::StringIsAny( type, { "SHORT2", "R16G16_SINT" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_SHORT2;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R16G16_SINT;
#endif
	}
	else if( unify::StringIsAny( type, { "SHORT4", "R16G16B16A16_SINT" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_SHORT4;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R16G16B16A16_SINT;
#endif
	}
	else if( unify::StringIsAny( type, { "UBYTE4N" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_UBYTE4N;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R8G8B8A8_UNORM;
#endif
	}
	else if( unify::StringIsAny( type, { "SHORT2N", "R16G16_SNORM" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_SHORT2N;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R16G16_SNORM;
#endif
	}
	else if( unify::StringIsAny( type, { "SHORT4N", "R16G16B16A16_SNORM" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_SHORT4N;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R16G16B16A16_SNORM;
#endif
	}
	else if( unify::StringIsAny( type, { "USHORT2N", "R16G16_UNORM" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_USHORT2N;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R16G16_UNORM;
#endif
	}
	else if( unify::StringIsAny( type, { "USHORT4N", "R16G16B16A16_UNORM" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_USHORT4N;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R16G16B16A16_UNORM;
#endif
	}
	else if( unify::StringIsAny( type, { "UDEC3", "R10G10B10A2_UINT" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_UDEC3;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R10G10B10A2_UINT;
#endif
	}
	else if( unify::StringIsAny( type, { "DEC3N", "R10G10B10A2_UNORM" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_DEC3N;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R10G10B10A2_UNORM;
#endif
	}
	else if( unify::StringIsAny( type, { "FLOAT16_2", "R16G16_FLOAT" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_FLOAT16_2;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R16G16_FLOAT;
#endif
	}
	else if( unify::StringIsAny( type, { "FLOAT16_4", "R16G16B16A16_FLOAT" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_FLOAT16_4;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_R16G16B16A16_FLOAT;
#endif
	}
	else if( unify::StringIsAny( type, { "UNUSED", "UNKNOWN" } ) )
	{
#if defined( DIRECTX9 )
		format = D3DDECLTYPE_UNUSED;
#elif defined( DIRECTX11 )
		format = DXGI_FORMAT_UNKNOWN;
#endif
	}
	else
	{
		throw unify::Exception( "Failed to convert string Vertex Declaration type!" );
	}
	size = SizeOf( format );
}

/*
D3DDECLMETHOD ConvertVertexDeclarationMethod( const std::string & method )
{
if ( unify::StringIs( "DEFAULT", method ) )
{
return D3DDECLMETHOD_DEFAULT;
}
else if (  unify::StringIs( "PARTIALU", method ) )
{
return D3DDECLMETHOD_PARTIALU;
}
else if (  unify::StringIs( "PARTIALV", method ) )
{
return D3DDECLMETHOD_PARTIALV;
}
else if (  unify::StringIs( "CROSSUV", method ) )
{
return D3DDECLMETHOD_CROSSUV;
}
else if (  unify::StringIs( "UV", method ) )
{
return D3DDECLMETHOD_UV;
}
else if (  unify::StringIs( "LOOKUP", method ) )
{
return D3DDECLMETHOD_LOOKUP;
}
else if (  unify::StringIs( "LOOKUPPRESAMPLED", method ) )
{
return D3DDECLMETHOD_LOOKUPPRESAMPLED;
}
else
{
throw unify::Exception( "Failed to convert string Vertex Declaration Method!" );
}
}
*/

#if defined( DIRECTX9 )
typedef D3DDECLUSAGE Semantic;
#elif defined( DIRECTX11 )
typedef char* Semantic;
#endif

Semantic ConvertUsage( const std::string & usage )
{
	if( unify::StringIs( "POSITION", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_POSITION;
#elif defined( DIRECTX11 )
		return "POSITION";
#endif
	}
	else if( unify::StringIs( "BLENDWEIGHT", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_BLENDWEIGHT;
#elif defined( DIRECTX11 )
		return "BLENDWEIGHT";
#endif
	}
	else if( unify::StringIs( "BLENDINDICES", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_BLENDINDICES;
#elif defined( DIRECTX11 )
		return "BLENDINDICES";
#endif
	}
	else if( unify::StringIs( "NORMAL", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_NORMAL;
#elif defined( DIRECTX11 )
		return "NORMAL";
#endif
	}
	else if( unify::StringIs( "PSIZE", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_PSIZE;
#elif defined( DIRECTX11 )
		return "PSIZE";
#endif
	}
	else if( unify::StringIs( "TEXCOORD", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_TEXCOORD;
#elif defined( DIRECTX11 )
		return "TEXCOORD";
#endif
	}
	else if( unify::StringIs( "TANGENT", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_TANGENT;
#elif defined( DIRECTX11 )
		return "TANGENT";
#endif
	}
	else if( unify::StringIs( "BINORMAL", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_BINORMAL;
#elif defined( DIRECTX11 )
		return "BINORMAL";
#endif
	}
	else if( unify::StringIs( "TESSFACTOR", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_TESSFACTOR;
#elif defined( DIRECTX11 )
		return "TESSFACTOR";
#endif
	}
	else if( unify::StringIs( "POSITIONT", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_POSITIONT;
#elif defined( DIRECTX11 )
		return "POSITIONT";
#endif
	}
	else if( unify::StringIs( "COLOR", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_COLOR;
#elif defined( DIRECTX11 )
		return "COLOR";
#endif
	}
	else if( unify::StringIs( "FOG", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_FOG;
#elif defined( DIRECTX11 )
		return "FOG";
#endif
	}
	else if( unify::StringIs( "DEPTH", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_DEPTH;
#elif defined( DIRECTX11 )
		return "DEPTH";
#endif
	}
	else if( unify::StringIs( "SAMPLE", usage ) )
	{
#if defined( DIRECTX9 )
		return D3DDECLUSAGE_SAMPLE;
#elif defined( DIRECTX11 )
		return "SAMPLE";
#endif
	}
	else
	{
		throw unify::Exception( "Failed to convert string Vertex Declaration usage!" );
	}
}

/*
std::string VertexElementToName( D3DDECLUSAGE usage, unsigned char usageIndex )
{
switch( usage )
{
case D3DDECLUSAGE_POSITION:
return "POSITION";
case D3DDECLUSAGE_BLENDWEIGHT:
return "BLENDWEIGHT";
case D3DDECLUSAGE_BLENDINDICES:
return "BLENDINDICES";
case D3DDECLUSAGE_NORMAL:
return "NORMAL";
case D3DDECLUSAGE_PSIZE:
return "PSIZE";
case D3DDECLUSAGE_TEXCOORD:
return std::string( "TEXCOORD" ) + unify::Cast< std::string >( usageIndex );
case D3DDECLUSAGE_TANGENT:
return "TANGENT";
case D3DDECLUSAGE_BINORMAL:
return "BINORMAL";
case D3DDECLUSAGE_TESSFACTOR:
return "TESSFACTOR";
case D3DDECLUSAGE_POSITIONT:
return "POSITIONT";
case D3DDECLUSAGE_COLOR:
switch( usageIndex )
{
case 0:
default:
return "DIFFUSE";
case 1:
return "SPECULAR";
}
case D3DDECLUSAGE_FOG:
return "FOG";
case D3DDECLUSAGE_DEPTH:
return "DEPTH";
case D3DDECLUSAGE_SAMPLE:
return "SAMPLE";
default:
throw unify::Exception( "Failed to convert string Vertex Declaration usage!" );
}
}
*/

void VertexNameToElement( const std::string & name, Semantic & usageOut, unsigned char & usageIndexOut )
{
	if( unify::StringIs( name, "POSITION" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_POSITION;
#elif defined( DIRECTX11 )
		usageOut = "POSITION";
#endif
		usageIndexOut = 0;
	}
	else if( unify::StringIs( name, "BLENDWEIGHT" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_BLENDWEIGHT;
#elif defined( DIRECTX11 )
		usageOut = "BLENDWEIGHT";
#endif
		usageIndexOut = 0;
	}
	else if( unify::StringIs( name, "BLENDINDICES" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_BLENDINDICES;
#elif defined( DIRECTX11 )
		usageOut = "BLENDINDICES";
#endif
		usageIndexOut = 0;
	}
	else if( unify::StringIs( name, "NORMAL" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_NORMAL;
#elif defined( DIRECTX11 )
		usageOut = "NORMAL";
#endif
		usageIndexOut = 0;
	}
	else if( unify::StringIs( name, "PSIZE" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_PSIZE;
#elif defined( DIRECTX11 )
		usageOut = "PSIZE";
#endif
		usageIndexOut = 0;
	}
	else if( unify::BeginsWith( name, "TEXCOORD" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_TEXCOORD;
#elif defined( DIRECTX11 )
		usageOut = "TEXCOORD";
#endif
		std::string n = name.substr( strlen( "TEXCOORD" ) );
		usageIndexOut = unify::Cast< unsigned char >( n );
	}
	else if( unify::BeginsWith( name, "TEX" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_TEXCOORD;
#elif defined( DIRECTX11 )
		usageOut = "TEXCOORD";
#endif
		std::string n = name.substr( strlen( "TEX" ) );
		usageIndexOut = unify::Cast< unsigned char >( n );
	}
	else if( unify::StringIs( name, "TANGENT" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_TANGENT;
#elif defined( DIRECTX11 )
		usageOut = "TANGENT";
#endif
		usageIndexOut = 0;
	}
	else if( unify::StringIs( name, "BINORMAL" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_BINORMAL;
#elif defined( DIRECTX11 )
		usageOut = "BINORMAL";
#endif
		usageIndexOut = 0;
	}
	else if( unify::StringIs( name, "TESSFACTOR" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_TESSFACTOR;
#elif defined( DIRECTX11 )
		usageOut = "TESSFACTOR";
#endif
		usageIndexOut = 0;
	}
	else if( unify::StringIs( name, "POSITIONT" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_POSITIONT;
#elif defined( DIRECTX11 )
		usageOut = "POSITIONT";
#endif
		usageIndexOut = 0;
	}
	else if( unify::StringIs( name, "DIFFUSE" ) || unify::StringIs( name, "COLOR" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_COLOR;
#elif defined( DIRECTX11 )
		usageOut = "COLOR";
#endif
		usageIndexOut = 0;
	}
	else if( unify::StringIs( name, "SPECULAR" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_COLOR;
#elif defined( DIRECTX11 )
		usageOut = "COLOR";
#endif
		usageIndexOut = 1;
	}
	else if( unify::StringIs( name, "FOG" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_FOG;
#elif defined( DIRECTX11 )
		usageOut = "FOG";
#endif
		usageIndexOut = 0;
	}
	else if( unify::StringIs( name, "DEPTH" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_DEPTH;
#elif defined( DIRECTX11 )
		usageOut = "DEPTH";
#endif
		usageIndexOut = 0;
	}
	else if( unify::StringIs( name, "SAMPLE" ) )
	{
#if defined( DIRECTX9 )
		usageOut = D3DDECLUSAGE_SAMPLE;
#elif defined( DIRECTX11 )
		usageOut = "SAMPLE";
#endif
		usageIndexOut = 0;
	}
	else
	{
		throw unify::Exception( "Failed to convert string Vertex Declaration usage/semantic!" );
	}
}



// TODO:
/*
// DX9:
WORD Stream;
WORD Offset;
BYTE Type;
BYTE Method;
BYTE Usage;
BYTE UsageIndex;

// DX11:
LPCSTR                     SemanticName;
UINT                       SemanticIndex;
DXGI_FORMAT                Format;
UINT                       InputSlot;
UINT                       AlignedByteOffset;
D3D11_INPUT_CLASSIFICATION InputSlotClass;
UINT                       InstanceDataStepRate;
*/

/*
struct VertexElement
{
const char *               SemanticName;			// Usage
unsigned int               SemanticIndex;			// UsageIndex
unsigned int               Format;					// Type
unsigned int               InputSlot;				// Stream
unsigned int               AlignedByteOffset;		// Offset
//D3D11_INPUT_CLASSIFICATION InputSlotClass;	    // <NA>
//UINT                       InstanceDataStepRate;	// <NA>
};
*/

/*
#if defined( DIRECTX9 )

D3DVERTEXELEMENT9 ToDX( VertexElement element )
{
	D3DVERTEXELEMENT9 out;
	out.Usage = ConvertUsage( element.SemanticName );
	out.UsageIndex = element.SemanticIndex;
	out.Offset = element.AlignedByteOffset;
	out.Stream = element.InputSlot;
	switch ( element.Format )
	{
	case VertexElement::Format::Float1: out.Type = D3DDECLTYPE_FLOAT1; break;
	case VertexElement::Format::Float2: out.Type = D3DDECLTYPE_FLOAT2; break;
	case VertexElement::Format::Float3: out.Type = D3DDECLTYPE_FLOAT3; break;
	case VertexElement::Format::Float4: out.Type = D3DDECLTYPE_FLOAT4; break;
	case VertexElement::Format::Color: out.Type = D3DDECLTYPE_D3DCOLOR; break;
	case VertexElement::Format::Unknown: out.Type = D3DDECLTYPE_UNUSED; break;
	}
	return out;
}

#elif defined( DIRECTX11 )

D3D11_INPUT_ELEMENT_DESC ToDX( VertexElement & elmenet )
{
	assert( 0 ); // TODO:
}

#endif
*/




class VertexDeclaration::Pimpl
{
public:
#if defined( DIRECTX9 )
	CComPtr< IDirect3DVertexDeclaration9 > m_layout;
#elif defined( DIRECTX11 )
	CComPtr< ID3D11InputLayout > m_layout;
#endif
};		

VertexDeclaration::VertexDeclaration( core::IRenderer * renderer )
	: m_pimpl( new Pimpl )
	, m_renderer( renderer )
{
}

VertexDeclaration::VertexDeclaration( core::IRenderer * renderer, const qxml::Element & element )
	: m_pimpl( new Pimpl )
	, m_renderer( renderer )
{
	// Accumulate elements.
	m_totalSizeInBytes = 0; // Automatically increases based on asusmed element size.
	for( const auto child : element.Children() )
	{
		if( child.IsTagName( "element" ) )
		{
			unsigned short slot = 0;
			if( child.HasAttributes( "stream" ) )
			{
				slot = child.GetAttributeElse< unsigned short >( "stream", 0 );
			}
			else if( child.HasAttributes( "slot" ) )
			{
				slot = child.GetAttributeElse< unsigned short >( "slot", 0 );
			}

			//unsigned short offset = child.GetAttributeElse< unsigned short >( "offset", m_totalSizeInBytes );

			Format format;
			size_t size;
			ConvertType( child.GetAttribute( "type" )->GetString(), format, size );

			//D3DDECLMETHOD method = child.HasAttributes( "method" ) ? ConvertVertexDeclarationMethod( child.GetAttribute( "method" )->GetString() ) : D3DDECLMETHOD_DEFAULT;

			std::string name = child.GetAttribute( "name" )->GetString();

			Semantic usage{};
			unsigned char usageIndex = child.GetAttributeElse< unsigned char >( "usageindex", 0 );
			if( child.HasAttributes( "usage" ) )
			{
				usage = ConvertUsage( child.GetAttribute( "usage" )->GetString() );
			}
			else // Get the usage from the name...
			{
				VertexNameToElement( name, usage, usageIndex );
			}

			VertexElement element{};
#if defined( DIRECTX9 )
			element.Stream = slot;
			element.Offset = (WORD)m_totalSizeInBytes; //offset;
			element.Type = format;
			//element.Method = method;
			element.Usage = usage;
			element.UsageIndex = usageIndex;
#elif defined( DIRECTX11 )
			element.InputSlot = slot;
			element.AlignedByteOffset = m_totalSizeInBytes;
			element.Format = format;
			// TODO: method 
			element.SemanticName = usage;
			element.SemanticIndex = usageIndex;
#endif
			m_elementMap[name] = m_elements.size(); // The next index is size, so this is what we will reference from our map.
			m_elements.push_back( element );

			//m_totalSizeInBytes = offset + size;
			m_totalSizeInBytes += size;
		}
	}

	if( m_elements.size() == 0 )
	{
		throw unify::Exception( "No vertex elements found for vertex declaration!" );
	}

#if defined( DIRECTX9 )
	VertexElement end = D3DDECL_END();
	m_elements.push_back( end );
#elif defined( DIRECTX11 )
	// Size of array is passed in now.
#endif 
}

VertexDeclaration::VertexDeclaration( core::IRenderer * renderer, const qjson::Object & object )
	: m_pimpl( new Pimpl )
	, m_renderer( renderer )
{
	// Accumulate elements.
	m_totalSizeInBytes = 0; // Automatically increases based on asusmed element size.
	for( auto itr : object )
		//for( const qxml::Element * itr = element.GetFirstChild(), *end = 0; itr != end; itr = itr->GetNext() )
	{
		size_t elementSizeInBytes = 0;
		const qjson::Object * member = dynamic_cast< const qjson::Object * >(itr.GetValue().get());
		if( member != nullptr )
		{
			int x( 0 ); x;
			/*
			unsigned short stream = itr.GetAttributeElse< unsigned short >( "stream", 0 );
			//unsigned short offset = itr->GetAttributeElse< unsigned short >( "offset", m_totalSizeInBytes );
			D3DDECLTYPE type = ConvertVertexDeclarationType( itr->GetAttribute( "type" )->GetString() );
			D3DDECLMETHOD method = itr->HasAttributes( "method" ) ? ConvertVertexDeclarationMethod( itr->GetAttribute( "method" )->GetString() ) : D3DDECLMETHOD_DEFAULT;
			D3DDECLUSAGE usage = ConvertVertexDeclarationUsage( itr->GetAttribute( "usage" )->GetString() );
			unsigned char usageIndex = itr->GetAttributeElse< unsigned char >( "usageindex", 0 );
			std::string name = element.HasAttributes( "name" ) ? element.GetAttribute( "name" )->GetString() : VertexElementToName( usage, usageIndex );

			VertexElement element;
			element.Stream = stream;
			element.Offset = (WORD)m_totalSizeInBytes; //offset;
			element.Type = type;
			element.Method = method;
			element.Usage = usage;
			element.UsageIndex = usageIndex;
			m_elementMap[name] = m_elements.size(); // The next index is size, so this is what we will reference from our map.
			m_elements.push_back( element );

			size = SizeOfVertexDeclarationElement( type );
			*/
		}
		else // Simple string as type.
		{
			unsigned short slot = 0;
			Format format;
			size_t size;
			ConvertType( itr.GetValue()->ToString(), format, size );

			//D3DDECLMETHOD method = D3DDECLMETHOD_DEFAULT;
			std::string name = itr.GetName();

			Semantic usage = {};
			unsigned char usageIndex = 0;
			VertexNameToElement( name, usage, usageIndex );

			VertexElement element{};
#if defined( DIRECTX9 )
			element.Stream = slot;
			element.Offset = (WORD)m_totalSizeInBytes; //offset;
			element.Type = format;
			//element.Method = method;
			element.Usage = usage;
			element.UsageIndex = usageIndex;
#elif defined( DIRECTX11 )
			element.InputSlot = slot;
			element.AlignedByteOffset = m_totalSizeInBytes;
			element.Format = format;
			// TODO: method 
			element.SemanticName = usage;
			element.SemanticIndex = usageIndex;
#endif

			m_elementMap[name] = m_elements.size(); // The next index is size, so this is what we will reference from our map.
			m_elements.push_back( element );

			elementSizeInBytes = size;
		}
		//m_totalSizeInBytes = offset + size;
		m_totalSizeInBytes += elementSizeInBytes;
	}

	if( m_elements.size() == 0 )
	{
		throw unify::Exception( "No vertex elements found for vertex declaration!" );
	}

#if defined( DIRECTX9 )
	VertexElement end = D3DDECL_END();
	m_elements.push_back( end );
#elif defined( DIRECTX11 )
	// Size of array is passed in now.
#endif 
}

VertexDeclaration::~VertexDeclaration()
{
	Destroy();
}

void VertexDeclaration::Build( const VertexShader & vs )
{
#if defined( DIRECTX9 )
	HRESULT hr = win::DX::GetDxDevice()->CreateVertexDeclaration( &m_elements[0], &m_layout );
	if( FAILED( hr ) )
	{
		throw unify::Exception( "Failed to create vertex declaration!" );
	}
#elif defined( DIRECTX11 )
	HRESULT hr = win::DX::GetDxDevice()->CreateInputLayout( &m_elements[0], m_elements.size(), vs.GetBytecode(), vs.GetBytecodeLength(), &m_layout );
	if( FAILED( hr ) )
	{
		throw unify::Exception( "Failed to create vertex declaration!" );
	}
#endif
}

bool VertexDeclaration::operator==( const VertexDeclaration & b ) const
{
	if( GetSize() != b.GetSize() || m_elements.size() != b.m_elements.size() )
	{
		return false;
	}

	for( size_t i = 0; i < m_elements.size(); ++i )
	{
		if( memcmp( &m_elements[i], &b.m_elements[i], sizeof( VertexElement ) ) != 0 )
		{
			return false;
		}
	}

	return true;
}

bool VertexDeclaration::operator!=( const VertexDeclaration & b ) const
{
	return !(operator==( b ));
}

void VertexDeclaration::Destroy()
{
	m_elements.clear();
	m_elementMap.clear();
	m_layout = nullptr;
}

size_t VertexDeclaration::GetNumberOfElements() const
{
	return m_elementMap.size(); // Note that the size of m_elements is either 0 or the number of elements + 1.
}

bool VertexDeclaration::ElementExists( const std::string & name ) const
{
	ElementMap::const_iterator itr = m_elementMap.find( name );
	return !(itr == m_elementMap.end());
}

bool VertexDeclaration::GetElement( const std::string name, VertexElement & element ) const
{
	ElementMap::const_iterator itr = m_elementMap.find( name );
	return GetElement( itr->second, element );
}

bool VertexDeclaration::GetElement( size_t index, VertexElement & element ) const
{
	if( index < m_elements.size() )
	{
		element = m_elements[index];
		return true;
	}
	else
	{
		return false;
	}
}

bool VertexDeclaration::GetElement( VertexElement toFind, VertexElement & element ) const
{
	auto elementItr = std::find_if( 
		m_elements.begin(), m_elements.end(), 
		[ & ]( auto & e ) 
	{ 
#if defined( DIRECTX9 )
		return e.Usage == toFind.Usage && e.UsageIndex == toFind.UsageIndex; 
#elif defined( DIRECTX11 )
		return e.SemanticName == toFind.SemanticName && e.SemanticIndex == toFind.SemanticIndex;
#endif
	} );
	if( elementItr == m_elements.end() )
	{
		return false;
	}
	element = *elementItr;
	return true;
}


size_t VertexDeclaration::GetElementOffset( const std::string & name ) const
{
	ElementMap::const_iterator itr = m_elementMap.find( name );
	if( itr == m_elementMap.end() )
	{
		throw unify::Exception( "VertexDeclaration: Element not found!" );
	}

#if defined( DIRECTX9 )
	return m_elements[itr->second].Offset;
#elif defined( DIRECTX11 )
	return m_elements[itr->second].AlignedByteOffset;
#endif
}

size_t VertexDeclaration::GetElementSize( const std::string & name ) const
{
	ElementMap::const_iterator itr = m_elementMap.find( name );
	if( itr == m_elementMap.end() )
	{
		throw unify::Exception( "VertexDeclaration: Element not found!" );
	}

#if defined( DIRECTX9 )
	return SizeOf( static_cast< Format >(m_elements[itr->second].Type) );
#elif defined( DIRECTX11 )
	return SizeOf( static_cast< Format >(m_elements[itr->second].Format) );
#endif
}

size_t VertexDeclaration::GetSize() const
{
	return m_totalSizeInBytes;
}

void VertexDeclaration::Use()
{
#if defined( DIRECTX9 )
	HRESULT result = win::DX::GetDxDevice()->SetVertexDeclaration( m_layout );
	if( FAILED( result ) )
	{
		throw unify::Exception( "Failed to set vertex declaration!" );
	}
#elif defined( DIRECTX11 )
	win::DX::GetDxContext()->IASetInputLayout( m_layout );
#endif
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V2< float > & v ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = Offset( elementFound );

	switch( WriteMethod::FromElement( elementFound ) )
	{
	case WriteMethod::Float2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	case WriteMethod::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	case WriteMethod::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = v.x;
		destItem->y = v.y;
		destItem->z = 0;
		destItem->w = 0;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V3< float > & v ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = Offset( elementFound );

	switch( WriteMethod::FromElement( elementFound ) )
	{
	case WriteMethod::Float2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	case WriteMethod::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	case WriteMethod::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V4< float > & v ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = Offset( elementFound );

	switch( WriteMethod::FromElement( elementFound ) )
	{
	case WriteMethod::Float2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = v.x;
		destItem->y = v.y;
		return true;
	}
	case WriteMethod::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = v.x;
		destItem->y = v.y;
		destItem->z = v.z;
		return true;
	}
	case WriteMethod::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = v.x;
		destItem->y = v.y;
		destItem->z = v.z;
		destItem->w = v.w;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::TexCoords & tc ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = Offset( elementFound );

	switch( WriteMethod::FromElement( elementFound ) )
	{
	case WriteMethod::Float2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = tc.u;
		destItem->y = tc.v;
		return true;
	}
	case WriteMethod::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = tc.u;
		destItem->y = tc.v;
		destItem->z = 0;
		return true;
	}
	case WriteMethod::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = tc.u;
		destItem->y = tc.v;
		destItem->z = 0;
		destItem->w = 0;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::Color & c ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	unify::ColorUnit cu( c );

	size_t offset = Offset( elementFound );

	switch( WriteMethod::FromElement( elementFound ) )
	{
	case WriteMethod::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		return true;
	}
	case WriteMethod::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		destItem->w = cu.a;
		return true;
	}
	case WriteMethod::Color:
	{
		unify::Color * destItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = c;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::ColorUnit & cu ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	unify::Color c( cu );

	size_t offset = Offset( elementFound );

	switch( WriteMethod::FromElement( elementFound ) )
	{
	case WriteMethod::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		return true;
	}
	case WriteMethod::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		destItem->w = cu.a;
		return true;
	}
	case WriteMethod::Color:
	{
		unify::Color * destItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = c;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, size_t vertexIndex, const VertexDeclaration & inVD, const void * vertex ) const
{
	size_t result = 0;
	for( auto e : inVD.m_elements )
	{
		size_t offset = Offset( e );
		switch( WriteMethod::FromElement( e ) )
		{
		case WriteMethod::Float2:
		{
			unify::V2< float > * d = reinterpret_cast<unify::V2< float > *>((unsigned char *)vertex + offset);
			result += WriteVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case WriteMethod::Float3:
		{
			unify::V3< float > * d = reinterpret_cast<unify::V3< float > *>((unsigned char *)vertex + offset);
			result += WriteVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
		}
		break;
		case WriteMethod::Float4:
		{
			unify::V4< float > * d = reinterpret_cast<unify::V4< float > *>((unsigned char *)vertex + offset);
			result += WriteVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case WriteMethod::Color:
		{
			unify::Color * d = reinterpret_cast<unify::Color *>((unsigned char *)vertex + offset);
			result += WriteVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		default:
			break;
		}
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V2< float > & v )  const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, element, v ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V3< float > & v ) const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, element, v ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V4< float > & v ) const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, element, v ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::TexCoords & tc ) const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, element, tc ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::Color & c ) const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, element, c ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::ColorUnit & cu ) const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, element, cu ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::WriteVertex( unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, const VertexDeclaration & inVD, const void * vertex ) const
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( lock, i, inVD, vertex ) ? 1 : 0;
	}
	return result != 0;
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V2< float > & v )	const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = Offset( elementFound );

	switch( WriteMethod::FromElement( elementFound ) )
	{
	case WriteMethod::Float2:
	{
		unify::V2< float > * sourceItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v = *sourceItem;
		return true;
	}
	case WriteMethod::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v = *sourceItem;
		return true;
	}
	case WriteMethod::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V3< float > & v )	const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = Offset( elementFound );

	switch( WriteMethod::FromElement( elementFound ) )
	{
	case WriteMethod::Float2:
	{
		const unify::V2< float > * sourceItem = reinterpret_cast<const unify::V2< float > *>((const unsigned char *)lock.GetItemReadOnly( vertexIndex ) + offset);
		v = *sourceItem;
		return true;
	}
	case WriteMethod::Float3:
	{
		const unify::V3< float > * sourceItem = reinterpret_cast<const unify::V3< float > *>((const unsigned char *)lock.GetItemReadOnly( vertexIndex ) + offset);
		v = *sourceItem;
		return true;
	}
	case WriteMethod::Float4:
	{
		const unify::V4< float > * sourceItem = reinterpret_cast<const unify::V4< float > *>((const unsigned char *)lock.GetItemReadOnly( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		v.z = sourceItem->z;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V4< float > & v ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = Offset( elementFound );

	switch( WriteMethod::FromElement( elementFound ) )
	{
	case WriteMethod::Float2:
	{
		unify::V2< float > * sourceItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		return true;
	}
	case WriteMethod::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		v.z = sourceItem->z;
		return true;
	}
	case WriteMethod::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		v.z = sourceItem->z;
		v.w = sourceItem->w;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::TexCoords & tc ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = Offset( elementFound );

	switch( WriteMethod::FromElement( elementFound ) )
	{
	case WriteMethod::Float2:
	{
		unify::V2< float > * sourceItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		tc.u = sourceItem->x;
		tc.v = sourceItem->y;
		return true;
	}
	case WriteMethod::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		tc.u = sourceItem->x;
		tc.v = sourceItem->y;
		return true;
	}
	case WriteMethod::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		tc.u = sourceItem->x;
		tc.v = sourceItem->y;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::Color & c ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	unify::ColorUnit cu( c );

	size_t offset = Offset( elementFound );

	switch( WriteMethod::FromElement( elementFound ) )
	{
	case WriteMethod::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		return true;
	}
	case WriteMethod::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		cu.a = sourceItem->w;
		return true;
	}
	case WriteMethod::Color:
	{
		unify::Color * sourceItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		c = *sourceItem;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::ColorUnit & cu ) const
{
	VertexElement elementFound = {};
	if( !GetElement( element, elementFound ) )
	{
		return false;
	}

	unify::Color c( cu );

	size_t offset = Offset( elementFound );

	switch( WriteMethod::FromElement( elementFound ) )
	{
	case WriteMethod::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		return true;
	}
	case WriteMethod::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		cu.a = sourceItem->w;
		return true;
	}
	case WriteMethod::Color:
	{
		unify::Color * sourceItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		c = *sourceItem;
		return true;
	}
	default:
		return false;
	}
}

bool VertexDeclaration::ReadVertex( unify::DataLock & lock, size_t vertexIndex, const VertexDeclaration & inVD, void * vertex )	const
{
	size_t result = 0;
	for( auto e : inVD.m_elements )
	{
		size_t offset = Offset( e );
		switch( WriteMethod::FromElement( e ) )
		{
		case WriteMethod::Float2:
		{
			unify::V2< float > * d = reinterpret_cast<unify::V2< float > *>((unsigned char *)vertex + offset);
			result += ReadVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case WriteMethod::Float3:
		{
			unify::V3< float > * d = reinterpret_cast<unify::V3< float > *>((unsigned char *)vertex + offset);
			result += ReadVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
		}
		break;
		case WriteMethod::Float4:
		{
			unify::V4< float > * d = reinterpret_cast<unify::V4< float > *>((unsigned char *)vertex + offset);
			result += ReadVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case WriteMethod::Color:
		{
			unify::Color * d = reinterpret_cast<unify::Color *>((unsigned char *)vertex + offset);
			result += ReadVertex( lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		default:
			break;
		}
	}
	return result != 0;
}



