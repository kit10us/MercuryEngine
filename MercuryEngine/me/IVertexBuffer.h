// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/Mercury.h>
#include <me/VertexDeclaration.h>
#include <me/DataBuffer.h>
#include <unify/BBox.h>
#include <memory>

namespace me 
{
	struct VertexBufferParameters
	{
		VertexBufferParameters()
			: slot( 0 )
			, source( 0 )
			, usage( me::BufferUsage::Default )
		{
		}

		VertexBufferParameters( unsigned int _count, VertexDeclaration::ptr _vd, size_t _slot, const void * _source, me::BufferUsage::TYPE _usage = me::BufferUsage::Default, unify::BBox< float > _bbox = unify::BBox< float >() )
			: numVertices( _count )
			, vertexDeclaration( _vd )
			, slot( _slot )
			, source( _source )
			, usage( _usage )
			, bbox( _bbox )
		{
		}

		unsigned int numVertices;
		VertexDeclaration::ptr vertexDeclaration;
		size_t slot;
		const void * source;
		BufferUsage::TYPE usage;
		unify::BBox< float > bbox;
	};

	class IVertexBuffer
	{
	public:
		typedef std::shared_ptr< IVertexBuffer > ptr;

		virtual ~IVertexBuffer() {}

		virtual void Create( me::VertexBufferParameters parameters ) = 0;
		
		virtual VertexDeclaration::ptr GetVertexDeclaration() const = 0;

		virtual void Lock( unify::DataLock & lock ) = 0;
		virtual void LockReadOnly( unify::DataLock & lock ) const = 0;
		virtual void Unlock() = 0;
		virtual void Unlock() const = 0;

		virtual void Use() const = 0;

		virtual unify::BBox< float > & GetBBox() = 0;
		virtual const unify::BBox< float > & GetBBox() const = 0;
	};
}