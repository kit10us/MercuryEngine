// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Effect.h>
#include <unify/unify.h>
#include <string>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace me
{
	// Primitive Type (PrimitiveType)...
	struct PrimitiveType
	{
		enum TYPE
		{
			PointList,
			LineList,
			LineStrip,
			TriangleList,
			TriangleStrip
		};

		static TYPE StringToPrimitiveType( const std::string & type );
	};

	const unsigned int NoEffect = std::numeric_limits< unsigned int >::max();

	class RenderMethod
	{
	public: // TODO: Make private.
		RenderMethod( PrimitiveType::TYPE type, unsigned int startVertex, unsigned int vertexCount, unsigned int primitiveCount, Effect::ptr, bool useIB = false, size_t vertexBufferIndex = 0, size_t indexBufferIndex = 0 );
		RenderMethod( PrimitiveType::TYPE type, int baseVertexIndex, unsigned int minIndex, unsigned int vertexCount, unsigned int startIndex, unsigned int primitiveCount, Effect::ptr effect, bool useIB = true, size_t vertexBufferIndex = 0, size_t indexBufferIndex = 0 );
	public:
		RenderMethod();

		static RenderMethod CreateTriangleStrip( unsigned int startVertex, unsigned int segmentCount, Effect::ptr effect );
		static RenderMethod CreateTriangleList( unsigned int startVertex, unsigned int triangleCount, Effect::ptr effect );
		static RenderMethod CreatePointList( unsigned int startVertex, unsigned int pointCount, Effect::ptr effect );
		static RenderMethod CreateLineList( unsigned int startVertex, unsigned int lineCount, Effect::ptr effect );

		static RenderMethod CreateTriangleStripIndexed( size_t vertexCount, unsigned int indexCount, unsigned int startIndex, unsigned int baseVertexIndex, Effect::ptr effect );
		static RenderMethod CreateTriangleListIndexed( size_t vertexCount, unsigned int indexCount, unsigned int startIndex, unsigned int baseVertexIndex, Effect::ptr effect );

		static unsigned int VertexCountInATriangleStrip( unsigned int triangleCount );
		static unsigned int VertexCountInATriangleList( unsigned int triangleCount );

		void Render( const me::RenderInfo & renderInfo ) const;

	public:
		PrimitiveType::TYPE primitiveType;
		unsigned int startVertex;
		int baseVertexIndex;
		unsigned int minIndex;
		unsigned int vertexCount;
		unsigned int startIndex;
		unsigned int primitiveCount;
		unsigned int indexCount;
		Effect::ptr effect;
		bool useIB;
		size_t vertexBufferIndex;
		size_t indexBufferIndex;
	};
}