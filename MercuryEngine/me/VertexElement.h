// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <qxml/Element.h>
#include <qjson/QJson.h>
#include <unify/V2.h>
#include <unify/V3.h>
#include <unify/V4.h>
#include <unify/TexCoords.h>
#include <unify/Color.h>

namespace me
{
	struct ElementFormat
	{
		enum TYPE
		{
			Float1,
			Float2,
			Float3,
			Float4,
			ColorUNorm,
			Unknown
		};

		static size_t SizeOf( TYPE format );
		static TYPE FromString( std::string format );
		static std::string ToString( TYPE format );
	};


	struct SlotClass
	{
		enum TYPE
		{
			Vertex = 0,
			Object
		};
		static TYPE FromString( std::string value );
		static std::string ToString( TYPE value );
	};

	struct VertexElement
	{
		std::string             SemanticName;
		unsigned int            SemanticIndex;
		ElementFormat::TYPE		Format;
		unsigned int            InputSlot;
		unsigned int			AlignedByteOffset;  
		SlotClass::TYPE			SlotClass;
		unsigned int			InstanceDataStepRate;

		VertexElement();
		VertexElement( const qxml::Element & element );
		VertexElement( const qjson::Pair & pair );
		size_t SizeOf() const;
	};


	/// <summary>
	/// Used for writing to the elements.
	/// </summary>
	struct CommonVertexElement
	{
		static VertexElement Position( unsigned int slot = 0 );
		static VertexElement Normal( unsigned int slot = 0 );
		static VertexElement Diffuse( unsigned int slot = 0 );
		static VertexElement Specular( unsigned int slot = 0 );
		static VertexElement TexCoords( unsigned int slot = 0 );
		static VertexElement TexCoords2( unsigned int slot = 0 );
	};
}