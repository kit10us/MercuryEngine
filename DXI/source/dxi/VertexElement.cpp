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
