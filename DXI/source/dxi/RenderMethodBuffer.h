// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/RenderMethod.h>
#include <dxi/VertexBuffer.h>
#include <dxi/IndexBuffer.h>
#include <vector>
#include <memory>

namespace dxi
{
	class RenderMethodBuffer
	{
		std::vector< RenderMethod > m_methodList;
	public:
		typedef std::shared_ptr< RenderMethodBuffer > shared_ptr;

		RenderMethodBuffer();
		
		void Release();
		
		void AddMethod( RenderMethod & method );

		/// <summary>
		/// Append from another RenderMEthodBuffer.
		/// vertexOffset and indexOffset are added to the vertex indices and index indices. That is, the range of vertices and indices is moved by those
		/// respective amounts.
		size_t Append( const RenderMethodBuffer & from, size_t vertexOffset, size_t indexOffset );

		RenderMethod & GetMethod( unsigned int method );

		const RenderMethod & GetMethod( unsigned int method ) const;

		size_t GetSize() const;

		void Render( RenderInfo renderInfo ) const;
	};
}