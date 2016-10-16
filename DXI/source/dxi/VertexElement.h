// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <unify/V2.h>
#include <unify/V3.h>
#include <unify/V4.h>
#include <unify/TexCoords.h>
#include <unify/Color.h>

namespace dxi
{
	enum class ElementFormat
	{
		Float1,
		Float2,
		Float3,
		Float4,
		Color,
		Unknown
	};

	size_t SizeOf( ElementFormat format );
	ElementFormat ElementFormatFromString( std::string type );

	struct VertexElement
	{
		std::string                SemanticName;
		unsigned int               SemanticIndex;
		ElementFormat		       Format;
		unsigned int               InputSlot;
		unsigned int               AlignedByteOffset;
	};

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