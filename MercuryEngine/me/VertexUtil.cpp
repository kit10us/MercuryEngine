// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <me/VertexUtil.h>

using namespace me;

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V2< float > & v )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = v.x;
		destItem->y = v.y;
		destItem->z = 0;
		destItem->w = 0;
		return true;
	}
	default:
		return false;
	}
}

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V3< float > & v )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = v;
		return true;
	}
	default:
		return false;
	}
}

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V4< float > & v )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = v.x;
		destItem->y = v.y;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = v.x;
		destItem->y = v.y;
		destItem->z = v.z;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = v.x;
		destItem->y = v.y;
		destItem->z = v.z;
		destItem->w = v.w;
		return true;
	}
	default:
		return false;
	}
}

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::TexCoords & tc )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * destItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = tc.u;
		destItem->y = tc.v;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = tc.u;
		destItem->y = tc.v;
		destItem->z = 0;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = tc.u;
		destItem->y = tc.v;
		destItem->z = 0;
		destItem->w = 0;
		return true;
	}
	default:
		return false;
	}
}

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::Color & c )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	unify::ColorUnit cu( c );

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		destItem->w = cu.a;
		return true;
	}
	case ElementFormat::ColorUNorm:
	{
		unify::Color * destItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = c;
		return true;
	}
	default:
		return false;
	}
}

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::ColorUnit & cu )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	unify::Color c( cu );

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float3:
	{
		unify::V3< float > * destItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * destItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		destItem->x = cu.r;
		destItem->y = cu.g;
		destItem->z = cu.b;
		destItem->w = cu.a;
		return true;
	}
	case ElementFormat::ColorUNorm:
	{
		unify::Color * destItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		*destItem = c;
		return true;
	}
	default:
		return false;
	}
}

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, const me::VertexDeclaration & inVD, const void * vertex )
{
	size_t result = 0;
	for( auto e : inVD.Elements() )
	{
		if ( lock.Slot() != e.InputSlot ) continue;

		size_t offset = e.AlignedByteOffset;
		switch( e.Format )
		{
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
		default:
			break;
		}
	}
	return result != 0;
}

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V2< float > & v )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, v ) ? 1 : 0;
	}
	return result != 0;
}

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V3< float > & v )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, v ) ? 1 : 0;
	}
	return result != 0;
}

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V4< float > & v )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, v ) ? 1 : 0;
	}
	return result != 0;
}

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::TexCoords & tc )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, tc ) ? 1 : 0;
	}
	return result != 0;
}

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::Color & c )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, c ) ? 1 : 0;
	}
	return result != 0;
}

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::ColorUnit & cu )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, element, cu ) ? 1 : 0;
	}
	return result != 0;
}

bool me::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, const VertexDeclaration & inVD, const void * vertex )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, inVD, vertex ) ? 1 : 0;
	}
	return result != 0;
}

bool me::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V2< float > & v )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * sourceItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v = *sourceItem;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v = *sourceItem;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		return true;
	}
	default:
		return false;
	}
}

bool me::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V3< float > & v )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;
	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		const unify::V2< float > * sourceItem = reinterpret_cast<const unify::V2< float > *>((const unsigned char *)lock.GetItemReadOnly( vertexIndex ) + offset);
		v = *sourceItem;
		return true;
	}
	case ElementFormat::Float3:
	{
		const unify::V3< float > * sourceItem = reinterpret_cast<const unify::V3< float > *>((const unsigned char *)lock.GetItemReadOnly( vertexIndex ) + offset);
		v = *sourceItem;
		return true;
	}
	case ElementFormat::Float4:
	{
		const unify::V4< float > * sourceItem = reinterpret_cast<const unify::V4< float > *>((const unsigned char *)lock.GetItemReadOnly( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		v.z = sourceItem->z;
		return true;
	}
	default:
		return false;
	}
}

bool me::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V4< float > & v )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * sourceItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		v.z = sourceItem->z;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		v.x = sourceItem->x;
		v.y = sourceItem->y;
		v.z = sourceItem->z;
		v.w = sourceItem->w;
		return true;
	}
	default:
		return false;
	}
}

bool me::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::TexCoords & tc )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float2:
	{
		unify::V2< float > * sourceItem = reinterpret_cast<unify::V2< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		tc.u = sourceItem->x;
		tc.v = sourceItem->y;
		return true;
	}
	case ElementFormat::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		tc.u = sourceItem->x;
		tc.v = sourceItem->y;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		tc.u = sourceItem->x;
		tc.v = sourceItem->y;
		return true;
	}
	default:
		return false;
	}
}

bool me::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::Color & c )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	unify::ColorUnit cu( c );

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		cu.a = sourceItem->w;
		return true;
	}
	case ElementFormat::ColorUNorm:
	{
		unify::Color * sourceItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		c = *sourceItem;
		return true;
	}
	default:
		return false;
	}
}

bool me::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::ColorUnit & cu )
{
	VertexElement elementFound = {};
	if( ! vd.GetElement( element, elementFound ) )
	{
		return false;
	}

	unify::Color c( cu );

	size_t offset = elementFound.AlignedByteOffset;

	switch( elementFound.Format )
	{
	case ElementFormat::Float3:
	{
		unify::V3< float > * sourceItem = reinterpret_cast<unify::V3< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		return true;
	}
	case ElementFormat::Float4:
	{
		unify::V4< float > * sourceItem = reinterpret_cast<unify::V4< float > *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		cu.r = sourceItem->x;
		cu.g = sourceItem->y;
		cu.b = sourceItem->z;
		cu.a = sourceItem->w;
		return true;
	}
	case ElementFormat::ColorUNorm:
	{
		unify::Color * sourceItem = reinterpret_cast<unify::Color *>((unsigned char *)lock.GetItem( vertexIndex ) + offset);
		c = *sourceItem;
		return true;
	}
	default:
		return false;
	}
}

bool me::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, const VertexDeclaration & inVD, void * vertex )
{
	size_t result = 0;
	for( auto e : inVD.Elements() )
	{
		size_t offset = e.AlignedByteOffset;
		switch( e.Format )
		{
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
		default:
			break;
		}
	}
	return result != 0;
}








/*


bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V2< float > & v )
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndex, element, v );
}

bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V3< float > & v )
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndex, element, v );
}

bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::V4< float > & v )
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndex, element, v );
}

bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::TexCoords & tc )
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndex, element, tc );
}

bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::Color & c )
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndex, element, c );
}

bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, const unify::ColorUnit & cu )
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndex, element, cu );
}

bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, const VertexDeclaration & inVD, const void * vertex )
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndex, inVD, vertex );
}

bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V2< float > & v )  const
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndices, element, v );
}

bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V3< float > & v )
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndices, element, v );
}

bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::V4< float > & v )
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndices, element, v );
}

bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::TexCoords & tc )
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndices, element, tc );
}

bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::Color & c )
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndices, element, c );
}

bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, VertexElement element, const unify::ColorUnit & cu )
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndices, element, cu );
}

bool VertexDeclaration::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, const VertexDeclaration & inVD, const void * vertex )
{
	return m_pimpl->WriteVertex( vd, lock, vertexIndices, inVD, vertex );
}

bool VertexDeclaration::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V2< float > & v )	const
{
	return m_pimpl->ReadVertex( vd, lock, vertexIndex, element, v );
}

bool VertexDeclaration::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V3< float > & v )	const
{
	return m_pimpl->ReadVertex( vd, lock, vertexIndex, element, v );
}

bool VertexDeclaration::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::V4< float > & v )
{
	return m_pimpl->ReadVertex( vd, lock, vertexIndex, element, v );
}

bool VertexDeclaration::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::TexCoords & tc )
{
	return m_pimpl->ReadVertex( vd, lock, vertexIndex, element, tc );
}

bool VertexDeclaration::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::Color & c )
{
	return m_pimpl->ReadVertex( vd, lock, vertexIndex, element, c );
}

bool VertexDeclaration::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, VertexElement element, unify::ColorUnit & cu )
{
	return m_pimpl->ReadVertex( vd, lock, vertexIndex, element, cu );
}

bool VertexDeclaration::ReadVertex( const VertexDeclaration & vd, unify::DataLock & lock, size_t vertexIndex, const VertexDeclaration & inVD, void * vertex )	const
{
	return m_pimpl->ReadVertex( vd, lock, vertexIndex, inVD, vertex );
}
*/