// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/VertexElement.h>

using namespace dxi;

using namespace dxi;

size_t ElementFormat::SizeOf( ElementFormat::TYPE format )
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
	case ElementFormat::ColorUNorm:
		return sizeof( unify::Color );
	case ElementFormat::Unknown:
		return 0;
	default:
		throw unify::Exception( "Vertex declaration type not support!" );
	}
}


template< typename T >
T Convert( std::string in, std::initializer_list< std::pair< std::string, T > > pairs, bool( *compare )(const std::string &, const std::string &) = unify::StringIs )
{
	for ( auto && pair : pairs )
	{
		if ( compare( in, pair.first ) ) return pair.second;
	}
	throw unify::Exception( "Conversion of " + in + " not found!" );
}

ElementFormat::TYPE ElementFormat::FromString( std::string format )
{
	return Convert< ElementFormat::TYPE>( format,
	{
		{ "Float1", ElementFormat::Float1 },
		{ "Float2", ElementFormat::Float2 },
		{ "TexCoord", ElementFormat::Float2 },
		{ "Float3", ElementFormat::Float3 },
		{ "ColorUNorm", ElementFormat::ColorUNorm },
		{ "Color", ElementFormat::ColorUNorm }
	} );
}

std::string ElementFormat::ToString( ElementFormat::TYPE format )
{
	switch ( format )
	{
	case ElementFormat::Float1:
		return "Float1";
	case ElementFormat::Float2:
		return "Float2";
	case ElementFormat::Float3:
		return "Float3";
	case ElementFormat::Float4:
		return "Float4";
	case ElementFormat::ColorUNorm:
		return "ColorUNorm";
	case ElementFormat::Unknown:
		return 0;
	default:
		throw unify::Exception( "Vertex declaration type not support!" );
	}
}





VertexElement::VertexElement()
	: SemanticIndex( 0 )
	, Format( ElementFormat::Unknown )
	, InputSlot( 0 )
	, AlignedByteOffset( 0 )
{
}

VertexElement::VertexElement( const qxml::Element & element, unsigned int slot )
{
	InputSlot = 0;
	if ( element.HasAttributes( "stream" ) )
	{
		InputSlot = element.GetAttributeElse< unsigned short >( "stream", 0 );
	}
	else if ( element.HasAttributes( "slot" ) )
	{
		InputSlot = element.GetAttributeElse< unsigned short >( "slot", 0 );
	}


	std::string name = element.GetAttribute( "name" )->GetString();
	if ( unify::StringIs( name, "POSITION" ) )
	{
		SemanticName = "POSITION";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "BLENDWEIGHT" ) )
	{
		SemanticName = "BLENDWEIGHT";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "BLENDINDICES" ) )
	{
		SemanticName = "BLENDINDICES";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "NORMAL" ) )
	{
		SemanticName = "NORMAL";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "PSIZE" ) )
	{
		SemanticName = "PSIZE";
		SemanticIndex = 0;
	}
	else if ( unify::BeginsWith( name, "TEXCOORD" ) )
	{
		SemanticName = "TEXCOORD";
		std::string n = name.substr( strlen( "TEXCOORD" ) );
		SemanticIndex = unify::Cast< unsigned char >( n );
	}
	else if ( unify::BeginsWith( name, "TEX" ) )
	{
		SemanticName = "TEXCOORD";
		std::string n = name.substr( strlen( "TEX" ) );
		SemanticIndex = unify::Cast< unsigned char >( n );
	}
	else if ( unify::StringIs( name, "TANGENT" ) )
	{
		SemanticName = "TANGENT";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "BINORMAL" ) )
	{
		SemanticName = "BINORMAL";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "TESSFACTOR" ) )
	{
		SemanticName = "TESSFACTOR";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "POSITIONT" ) )
	{
		SemanticName = "POSITIONT";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "DIFFUSE" ) || unify::StringIs( name, "COLOR" ) )
	{
		SemanticName = "COLOR";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "SPECULAR" ) )
	{
		SemanticName = "COLOR";
		SemanticIndex = 1;
	}
	else if ( unify::StringIs( name, "FOG" ) )
	{
		SemanticName = "FOG";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "DEPTH" ) )
	{
		SemanticName = "DEPTH";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "SAMPLE" ) )
	{
		SemanticName = "SAMPLE";
		SemanticIndex = 0;
	}
	else
	{
		throw unify::Exception( "Failed to convert string Vertex Declaration usage/semantic!" );
	}

	Format = ElementFormat::FromString( element.GetAttribute( "type" )->GetString() );

	AlignedByteOffset = 0; // Because we don't know here.
}

VertexElement::VertexElement( const qjson::Pair & pair, unsigned int slot )
{
	std::string name = pair.GetName();
	if ( unify::StringIs( name, "POSITION" ) )
	{
		SemanticName = "POSITION";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "BLENDWEIGHT" ) )
	{
		SemanticName = "BLENDWEIGHT";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "BLENDINDICES" ) )
	{
		SemanticName = "BLENDINDICES";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "NORMAL" ) )
	{
		SemanticName = "NORMAL";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "PSIZE" ) )
	{
		SemanticName = "PSIZE";
		SemanticIndex = 0;
	}
	else if ( unify::BeginsWith( name, "TEXCOORD" ) )
	{
		SemanticName = "TEXCOORD";
		std::string n = name.substr( strlen( "TEXCOORD" ) );
		SemanticIndex = unify::Cast< unsigned char >( n );
	}
	else if ( unify::BeginsWith( name, "TEX" ) )
	{
		SemanticName = "TEXCOORD";
		std::string n = name.substr( strlen( "TEX" ) );
		SemanticIndex = unify::Cast< unsigned char >( n );
	}
	else if ( unify::StringIs( name, "TANGENT" ) )
	{
		SemanticName = "TANGENT";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "BINORMAL" ) )
	{
		SemanticName = "BINORMAL";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "TESSFACTOR" ) )
	{
		SemanticName = "TESSFACTOR";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "POSITIONT" ) )
	{
		SemanticName = "POSITIONT";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "DIFFUSE" ) || unify::StringIs( name, "COLOR" ) )
	{
		SemanticName = "COLOR";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "SPECULAR" ) )
	{
		SemanticName = "COLOR";
		SemanticIndex = 1;
	}
	else if ( unify::StringIs( name, "FOG" ) )
	{
		SemanticName = "FOG";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "DEPTH" ) )
	{
		SemanticName = "DEPTH";
		SemanticIndex = 0;
	}
	else if ( unify::StringIs( name, "SAMPLE" ) )
	{
		SemanticName = "SAMPLE";
		SemanticIndex = 0;
	}
	else
	{
		throw unify::Exception( "Failed to convert string Vertex Declaration usage/semantic!" );
	}

	Format = ElementFormat::FromString( pair.GetValue()->ToString() );
	InputSlot = slot;
}

size_t VertexElement::SizeOf() const
{
	return ElementFormat::SizeOf( Format );
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
	diffuseE.Format = ElementFormat::ColorUNorm;
	diffuseE.SemanticName = "COLOR";
	diffuseE.SemanticIndex = 0;
	return diffuseE;
}

VertexElement CommonVertexElement::Specular( unsigned int slot )
{
	VertexElement specularE = {};
	specularE.InputSlot = slot;
	specularE.Format = ElementFormat::ColorUNorm;
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
