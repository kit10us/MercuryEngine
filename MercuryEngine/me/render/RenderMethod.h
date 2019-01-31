// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/render/Effect.h>
#include <unify/unify.h>
#include <string>


namespace me
{
	namespace render
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

			static TYPE StringToPrimitiveType( std::string type );
		};

		const unsigned int NoEffect = std::numeric_limits< unsigned int >::max();

		class RenderMethod
		{
		public: // TODO: Make private.
			RenderMethod( PrimitiveType::TYPE type, unsigned int startVertex, unsigned int vertexCount, unsigned int primitiveCount, bool useIB = false, size_t vertexBufferIndex = 0, size_t indexBufferIndex = 0 );
			RenderMethod( PrimitiveType::TYPE type, int baseVertexIndex, unsigned int minIndex, unsigned int vertexCount, unsigned int startIndex, unsigned int primitiveCount, bool useIB = true, size_t vertexBufferIndex = 0, size_t indexBufferIndex = 0 );

		public:
			RenderMethod();

			static RenderMethod CreateTriangleStrip( unsigned int startVertex, unsigned int segmentCount );
			static RenderMethod CreateTriangleList( unsigned int startVertex, unsigned int triangleCount );
			static RenderMethod CreatePointList( unsigned int startVertex, unsigned int pointCount );
			static RenderMethod CreateLineList( unsigned int startVertex, unsigned int lineCount );

			static RenderMethod CreateTriangleStripIndexed( size_t vertexCount, unsigned int indexCount, unsigned int startIndex, unsigned int baseVertexIndex );
			static RenderMethod CreateTriangleListIndexed( size_t vertexCount, unsigned int indexCount, unsigned int startIndex, unsigned int baseVertexIndex );

			static unsigned int VertexCountInATriangleStrip( unsigned int triangleCount );
			static unsigned int VertexCountInATriangleList( unsigned int triangleCount );

		public:
			PrimitiveType::TYPE primitiveType;
			unsigned int startVertex;
			int baseVertexIndex;
			unsigned int minIndex;
			unsigned int vertexCount;
			unsigned int startIndex;
			unsigned int primitiveCount;
			unsigned int indexCount;
			bool useIB;
			size_t vertexBufferIndex;
			size_t indexBufferIndex;
		};
	}
}