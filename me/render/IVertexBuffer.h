// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/Mercury.h>
#include <me/render/VertexDeclaration.h>
#include <me/render/IBuffer.h>
#include <unify/BBox.h>
#include <memory>

namespace me 
{
	namespace render
	{
		struct VertexBufferParameters
		{
			VertexBufferParameters()
				: usage( BufferUsage::Default )
			{
			}

			VertexBufferParameters( VertexDeclaration::ptr _vd, std::vector< CountAndSource > countAndSource, BufferUsage::TYPE _usage = me::render::BufferUsage::Default, unify::BBox< float > _bbox = unify::BBox< float >() )
				: vertexDeclaration( _vd )
				, countAndSource{ countAndSource }
				, usage( _usage )
				, bbox( _bbox )
			{
			}

			VertexDeclaration::ptr vertexDeclaration;
			std::vector< CountAndSource > countAndSource;
			BufferUsage::TYPE usage;
			unify::BBox< float > bbox;
		};

		class IVertexBuffer : public IBuffer
		{
		public:
			typedef std::shared_ptr< IVertexBuffer > ptr;

			virtual void Create( VertexBufferParameters parameters ) = 0;

			virtual VertexDeclaration::ptr GetVertexDeclaration() const = 0;

			virtual unify::BBox< float > & GetBBox() = 0;
			virtual const unify::BBox< float > & GetBBox() const = 0;
		};
	}
}