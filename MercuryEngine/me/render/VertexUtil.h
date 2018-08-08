// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/Mercury.h>
#include <me/render/VertexElement.h>
#include <me/render/VertexDeclaration.h>
#include <unify/DataLock.h>
#include <unify/V2.h>
#include <unify/V3.h>
#include <unify/V4.h>
#include <unify/TexCoords.h>
#include <unify/Color.h>
#include <unify/ColorUnit.h>
#include <unify/Matrix.h>
#include <memory>

namespace me
{
	namespace render
	{
		template< typename T >
		bool WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const T & v )
		{
			VertexElement elementFound = {};
			if( !vd.GetElement( element, elementFound ) )
			{
				return false;
			}

			return ElementFormat::Convert( elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset, ElementFormat::FromType< T >(), &v );
		}

		bool WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, const VertexDeclaration & inVD, const void * vertex );


		template< typename T >
		bool WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const T & v )
		{
			size_t result = 0;
			for( auto i : vertexIndices )
			{
				result += WriteVertex( vd, lock, i, element, v ) ? 1 : 0;
			}
			return result != 0;
		}

		bool WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, const VertexDeclaration & inVD, const void * vertex );

		template< typename T >
		bool ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, T & v )
		{
			VertexElement elementFound = {};
			if( !vd.GetElement( element, elementFound ) )
			{
				return false;
			}

			return ElementFormat::Convert( ElementFormat::FromType< T >(), &v, elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset );
		}

		bool ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, const VertexDeclaration & inVD, void * vertex );
	}
}
