// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/render/VertexUtil.h>

using namespace me;
using namespace render;

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

bool me::render::WriteVertex( const VertexDeclaration & vd, unify::DataLock & lock, std::initializer_list< size_t > vertexIndices, const VertexDeclaration & inVD, const void * vertex )
{
	size_t result = 0;
	for( auto i : vertexIndices )
	{
		result += WriteVertex( vd, lock, i, inVD, vertex ) ? 1 : 0;
	}
	return result != 0;
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
