// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/render/VertexElement.h>
#include <unify/String.h>
#include <unify/Exception.h>

using namespace me;
using namespace render;

SlotClass::TYPE SlotClass::FromString( std::string value )
{
	if ( unify::StringIs( value, "vertex" ) )
	{
		return SlotClass::Vertex;
	}
	else if ( unify::StringIs( value, "object" ) )
	{
		return SlotClass::Object;
	}

	throw unify::Exception( "Unknown Vertex Element slot class string! (" + value + ")" );
}

std::string SlotClass::ToString( SlotClass::TYPE value )
{
	switch( value )
	{
	case SlotClass::Vertex:
		return "Vertex";
	case SlotClass::Object:
		return "Object";
	default:
		throw unify::Exception( "Unknown Vertex Element slot class value!" );
	}
}
						   


VertexElement::VertexElement()
	: SemanticIndex( 0 )
	, Format( ElementFormat::Unknown )
	, InputSlot( 0 )
	, AlignedByteOffset( 0 )
	, SlotClass( SlotClass::Vertex )
	, InstanceDataStepRate( 0 )
{
}

VertexElement::VertexElement( const qxml::Element & element )
	: VertexElement()
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

	if ( element.HasAttributes( "name" ) )
	{
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
	}
	else if ( element.HasAttributes( "SemanticName" ) )
	{
		SemanticName = element.GetAttribute< std::string >( "semanticname" );
		SemanticIndex = element.GetAttributeElse( "semanticindex", 0 );
	}

	InstanceDataStepRate = element.GetAttributeElse( "instancedatasteprate", 0 );

	Format = ElementFormat::FromString( element.GetAttribute( "type" )->GetString() );

	if ( element.HasAttributes( "SlotClass" ) )
	{
		SlotClass = SlotClass::FromString( element.GetAttribute( "SlotClass" )->GetString() );
	}

	AlignedByteOffset = 0; // Because we don't know here.
}

VertexElement::VertexElement( const qjson::Pair & pair )
{
	InputSlot = 0;

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
