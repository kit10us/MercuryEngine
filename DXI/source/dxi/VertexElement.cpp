// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/VertexElement.h>

using namespace dxi;

// This is the automatically assumed size of the
size_t dxi::SizeOf( ElementFormat format )
{
	switch ( format )
	{
	case ElementFormat::Float1:
		return sizeof( float );
	case ElementFormat::Float2:
		return sizeof( float ) * 2;
	case ElementFormat::Float3:
		return sizeof( float ) * 3;
	case ElementFormat::Float4:
		return sizeof( float ) * 4;
	case ElementFormat::Color:
		return sizeof( unify::Color );
		/*
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
		*/
	case ElementFormat::Unknown:
		return 0;
	default:
		throw unify::Exception( "Vertex declaration type not support!" );
	}
}

ElementFormat dxi::ElementFormatFromString( std::string type )
{
	ElementFormat format;
	if ( unify::StringIsAny( type, { "FLOAT1", "FLOAT", "R32_FLOAT" } ) )
	{
		format = ElementFormat::Float1;
	}
	else if ( unify::StringIsAny( type, { "FLOAT2", "TEXCOORD", "R32G32_FLOAT" } ) )
	{
		format = ElementFormat::Float2;
	}
	else if ( unify::StringIsAny( type, { "FLOAT3", "R32G32B32_FLOAT" } ) )
	{
		format = ElementFormat::Float3;
	}
	else if ( unify::StringIsAny( type, { "FLOAT4", "COLORUNIT", "R32G32B32A32_FLOAT" } ) )
	{
		format = ElementFormat::Float4;
	}
	else if ( unify::StringIsAny( type, { "D3DCOLOR", "COLOR", "R8G8B8A8_UNORM" } ) )
	{
		format = ElementFormat::Color;
	}
	/*
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
	*/
	else
	{
		throw unify::Exception( "Failed to convert string Vertex Declaration type!" );
	}

	return format;
}



// Common Vertex Formats:

VertexElement CommonVertexElement::Position( unsigned int slot )
{
	VertexElement positionE = {};
	positionE.InputSlot = slot;
	positionE.Format = ElementFormat::Float3;
	positionE.SemanticName = "POSITION";
	positionE.SemanticIndex = 0;
	return positionE;

}

VertexElement CommonVertexElement::Normal( unsigned int slot )
{
	VertexElement normalE = {};
	normalE.InputSlot = slot;
	normalE.Format = ElementFormat::Float3;
	normalE.SemanticName = "NORMAL";
	normalE.SemanticIndex = 0;
	return normalE;
}

VertexElement CommonVertexElement::Diffuse( unsigned int slot )
{
	VertexElement diffuseE = {};
	diffuseE.InputSlot = slot;
	diffuseE.Format = ElementFormat::Color;
	diffuseE.SemanticName = "COLOR";
	diffuseE.SemanticIndex = 0;
	return diffuseE;
}

VertexElement CommonVertexElement::Specular( unsigned int slot )
{
	VertexElement specularE = {};
	specularE.InputSlot = slot;
	specularE.Format = ElementFormat::Color;
	specularE.SemanticName = "COLOR";
	specularE.SemanticIndex = 1;
	return specularE;
}

VertexElement CommonVertexElement::TexCoords( unsigned int slot )
{
	VertexElement texE = {};
	texE.InputSlot = slot;
	texE.Format = ElementFormat::Float2;
	texE.SemanticName = "TEXCOORD";
	texE.SemanticIndex = 0;
	return texE;
}

VertexElement CommonVertexElement::TexCoords2( unsigned int slot )
{
	VertexElement texE2 = {};
	texE2.InputSlot = slot;
	texE2.Format = ElementFormat::Float2;
	texE2.SemanticName = "TEXCOORD";
	texE2.SemanticIndex = 1;
	return texE2;
}
