// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/render/VertexUtil.h>

using namespace me;
using namespace render;

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const float & f )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset, ElementFormat::Float1, &f );
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V2< float > & v )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset, ElementFormat::Float2, &v );
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V3< float > & v )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset, ElementFormat::Float3, &v );
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V4< float > & v )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset, ElementFormat::Float4, &v );
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::TexCoords & tc )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset, ElementFormat::Float2, &tc );
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::Color & c )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset, ElementFormat::ColorUNorm, &c );
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::ColorUnit & cu )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset, ElementFormat::Float4, &cu );
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::Matrix & matrix )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset, ElementFormat::Matrix4x4, &matrix );
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, const me::render::VertexDeclaration & inVD, const void * vertex )
{
	size_t result = 0;
	size_t slot = 0;
	for ( size_t slot = 0, slot_count = inVD.NumberOfSlots(); slot < slot_count; slot++ )
	{
		for ( auto e : inVD.Elements( slot ) )
		{
			if ( lock.Slot() != e.InputSlot ) continue;

			size_t offset = e.AlignedByteOffset;
			switch ( e.Format )
			{
			case ElementFormat::Float1:
			{
				float * d = reinterpret_cast< float * >((unsigned char *)vertex + offset);
				result += WriteVertex( vd, lock, vertexIndex, e, *d ) ? 1 : 0;
				break;
			}
			case ElementFormat::Float2:
			{
				unify::V2< float > * d = reinterpret_cast<unify::V2< float > *>((unsigned char *)vertex + offset);
				result += WriteVertex( vd, lock, vertexIndex, e, *d ) ? 1 : 0;
				break;
			}
			case ElementFormat::Float3:
			{
				unify::V3< float > * d = reinterpret_cast<unify::V3< float > *>((unsigned char *)vertex + offset);
				result += WriteVertex( vd, lock, vertexIndex, e, *d ) ? 1 : 0;
			}
			break;
			case ElementFormat::Float4:
			{
				unify::V4< float > * d = reinterpret_cast<unify::V4< float > *>((unsigned char *)vertex + offset);
				result += WriteVertex( vd, lock, vertexIndex, e, *d ) ? 1 : 0;
				break;
			}
			case ElementFormat::ColorUNorm:
			{
				unify::Color * d = reinterpret_cast<unify::Color *>((unsigned char *)vertex + offset);
				result += WriteVertex( vd, lock, vertexIndex, e, *d ) ? 1 : 0;
				break;
			}
			case ElementFormat::Matrix4x4:
			{
				unify::Matrix * d = reinterpret_cast<unify::Matrix *>((unsigned char *)vertex + offset);
				result += WriteVertex( vd, lock, vertexIndex, e, *d ) ? 1 : 0;
				break;
			}
			default:
				break;
			}
		}
	}
	return result != 0;
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const float & f )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, f ) ? 1 : 0;
	}
	return result != 0;
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V2< float > & v )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, v ) ? 1 : 0;
	}
	return result != 0;
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V3< float > & v )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, v ) ? 1 : 0;
	}
	return result != 0;
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V4< float > & v )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, v ) ? 1 : 0;
	}
	return result != 0;
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::TexCoords & tc )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, tc ) ? 1 : 0;
	}
	return result != 0;
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::Color & c )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, c ) ? 1 : 0;
	}
	return result != 0;
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::ColorUnit & cu )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, cu ) ? 1 : 0;
	}
	return result != 0;
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::Matrix & matrix )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, matrix ) ? 1 : 0;
	}
	return result != 0;
}

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, const VertexDeclaration & inVD, const void * vertex )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, inVD, vertex ) ? 1 : 0;
	}
	return result != 0;
}

bool me::render::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, float & f )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( ElementFormat::Float1, &f, elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset );
}

bool me::render::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V2< float > & v )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( ElementFormat::Float2, &v, elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset );
}

bool me::render::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V3< float > & v )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( ElementFormat::Float3, &v, elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset );
}

bool me::render::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V4< float > & v )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( ElementFormat::Float4, &v, elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset );
}

bool me::render::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::TexCoords & tc )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( ElementFormat::Float2, &tc, elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset );
}

bool me::render::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::Color & c )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( ElementFormat::ColorUNorm, &c, elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset );
}

bool me::render::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::ColorUnit & cu )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( ElementFormat::Float4, &cu, elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset );
}

bool me::render::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::Matrix & matrix )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	return ElementFormat::Convert( ElementFormat::Matrix4x4, &matrix, elementFound.Format, (unsigned char *)lock.GetItem( vertexIndex ) + elementFound.AlignedByteOffset );
}

bool me::render::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, const VertexDeclaration & inVD, void * vertex )
{
	size_t result = 0;
	for( auto e : inVD.Elements() )
	{
		size_t offset = e.AlignedByteOffset;
		switch( e.Format )
		{
		case ElementFormat::Float1:
		{
			float * d = reinterpret_cast< float *>((unsigned char *)vertex + offset);
			result += ReadVertex( vd, lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case ElementFormat::Float2:
		{
			unify::V2< float > * d = reinterpret_cast<unify::V2< float > *>((unsigned char *)vertex + offset);
			result += ReadVertex( vd, lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case ElementFormat::Float3:
		{
			unify::V3< float > * d = reinterpret_cast<unify::V3< float > *>((unsigned char *)vertex + offset);
			result += ReadVertex( vd, lock, vertexIndex, e, *d ) ? 1 : 0;
		}
		break;
		case ElementFormat::Float4:
		{
			unify::V4< float > * d = reinterpret_cast<unify::V4< float > *>((unsigned char *)vertex + offset);
			result += ReadVertex( vd, lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case ElementFormat::ColorUNorm:
		{
			unify::Color * d = reinterpret_cast<unify::Color *>((unsigned char *)vertex + offset);
			result += ReadVertex( vd, lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}
		case ElementFormat::Matrix4x4:
		{
			unify::Matrix * d = reinterpret_cast<unify::Matrix *>((unsigned char *)vertex + offset);
			result += ReadVertex( vd, lock, vertexIndex, e, *d ) ? 1 : 0;
			break;
		}		default:
			break;
		}
	}
	return result != 0;
}
