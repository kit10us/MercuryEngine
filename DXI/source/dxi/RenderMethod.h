// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/win/DirectX.h>
#include <dxi/Effect.h>
#include <unify/unify.h>
#include <string>

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

namespace dxi
{
	// Primitive Type (PrimitiveType)...
	struct PrimitiveType
	{
		enum TYPE
		{
			PointList = D3DPT_POINTLIST,
			LineList = D3DPT_LINELIST,
			LineStrip = D3DPT_LINESTRIP,
			TriangleList = D3DPT_TRIANGLELIST,
			TriangleStrip = D3DPT_TRIANGLESTRIP,
			TriangleFan	= D3DPT_TRIANGLEFAN
		};

        static TYPE StringToPrimitiveType( const std::string & type );
    };

    const unsigned int NoEffect = std::numeric_limits< unsigned int >::max();

	class RenderMethod
	{
	public:
		RenderMethod();
		RenderMethod( PrimitiveType::TYPE primitiveType, unsigned int startVertex, unsigned int vertexCount, unsigned int primitiveCount, Effect::shared_ptr, bool useIB = false, size_t vertexBufferIndex = 0, size_t indexBufferIndex = 0 );
		RenderMethod( PrimitiveType::TYPE primitiveType, int baseVertexIndex, unsigned int minIndex, unsigned int vertexCount, unsigned int startIndex, unsigned int primitiveCount, Effect::shared_ptr effect, bool useIB = true, size_t vertexBufferIndex = 0, size_t indexBufferIndex = 0 );

		// Named constructors
		// TODO: Add more low level named constructors.
		// TODO: Replace 'Create' prefix with 'RenderMethod'.
		// TODO: Support VB and IB indices.

		// These assume non-index buffered...
		static RenderMethod CreateFan( unsigned int startVertex, unsigned int segmentCount, Effect::shared_ptr effect );
		static RenderMethod CreateTriangleStrip( unsigned int startVertex, unsigned int segmentCount, Effect::shared_ptr effect );
		static RenderMethod CreatePointList( unsigned int startVertex, unsigned int pointCount, Effect::shared_ptr effect );

		static unsigned int VertexCountInAFan( unsigned int segmentCount );
		static unsigned int VertexCountInATriangleStrip( unsigned int triangleCount );
		static unsigned int VertexCountInATriangleList( unsigned int triangleCount );

		size_t GetVertexBufferIndex() const;
		size_t GetIndexBufferIndex() const;

		void Render( const RenderInfo & renderInfo ) const;

	public:
		PrimitiveType::TYPE primitiveType;
		unsigned int startVertex;
		int baseVertexIndex;
		unsigned int minIndex;
		unsigned int vertexCount;
		unsigned int startIndex;
		unsigned int primitiveCount;
		Effect::shared_ptr effect;
		bool useIB;
		size_t vertexBufferIndex;
		size_t indexBufferIndex;
	};
}