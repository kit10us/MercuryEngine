// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/ElementFormat.h>
#include <qxml/Element.h>
#include <qjson/QJson.h>

namespace me
{
	namespace render
	{
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
			static VertexElement Position( unsigned int slot );
			static VertexElement Normal( unsigned int slot );
			static VertexElement Diffuse( unsigned int slot );
			static VertexElement Specular( unsigned int slot );
			static VertexElement TexCoords( unsigned int slot, unsigned int semanticIndex = 0 );
			static VertexElement Generic( unsigned int slot, unsigned int semanticIndex, ElementFormat::TYPE format );
		};
	}
}