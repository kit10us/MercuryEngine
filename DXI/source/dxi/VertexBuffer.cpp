// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/exception/FailedToCreate.h>
#include <dxi/exception/FailedToLock.h>
#include <dxi/exception/Exception.h>
#include <assert.h>
#include <dxi/VertexBuffer.h>
#include <unify/TexCoords.h>
#include <unify/V3.h>
#include <unify/Color.h>
#include <unify/V4.h>
#include <dxi/core/Game.h>

using namespace dxi;

VertexBuffer::VertexBuffer()
{
}

VertexBuffer::VertexBuffer( unsigned int numVertices, VertexDeclaration vertexDeclaration, BufferUsage::TYPE usage, const void * source, unify::Flags flags )
{
	Create( numVertices, vertexDeclaration, usage, source, flags );
}

VertexBuffer::~VertexBuffer()
{
	Release();
}

void VertexBuffer::Create( unsigned int uNumVertices, VertexDeclaration vertexDeclaration, BufferUsage::TYPE usage, const void * source, unify::Flags flags )
{
	Release();

	m_vertexDeclaration = vertexDeclaration;
	m_createFlags = flags;
	m_stride = m_vertexDeclaration.GetSize();
	m_length = uNumVertices;
	m_usage = usage;

	// Ensure we have some sort of idea what we need to be...
	if( GetSize() == 0 )
	{
		throw exception::FailedToCreate( "Not a valid vertex buffer size!" );
	}

	// Build Vertex Buffer usage...
	unsigned int createFlags = FLAGNULL;
	if( unify::CheckFlag( m_createFlags, VertexBuffer::CreateFlags::DoNotClip ) )
	{
		createFlags |= D3DUSAGE_DONOTCLIP;
	}
	if( unify::CheckFlag( m_createFlags, VertexBuffer::CreateFlags::NPatches ) )
	{
		createFlags |= D3DUSAGE_NPATCHES;
	}
	if( unify::CheckFlag( m_createFlags, VertexBuffer::CreateFlags::Points ) )
	{
		createFlags |= D3DUSAGE_POINTS;
	}
	if( unify::CheckFlag( m_createFlags, VertexBuffer::CreateFlags::RTPatches ) )
	{
		createFlags |= D3DUSAGE_RTPATCHES;
	}

	D3DPOOL pool = D3DPOOL_DEFAULT;
	switch( m_usage )
	{
	case BufferUsage::Default:
		pool = D3DPOOL_MANAGED;
		break;
	case BufferUsage::Immutable:
		pool = D3DPOOL_DEFAULT;
		createFlags |= D3DUSAGE_WRITEONLY;
		break;
	case BufferUsage::Dynamic:
		pool = D3DPOOL_MANAGED;
		break;
	case BufferUsage::Staging:
		//pool = D3DPOOL_SCRATCH;
		pool = D3DPOOL_SYSTEMMEM;
		break;
	}

	// Create Vertex Buffer...
	HRESULT hr;
	hr = win::DX::GetDxDevice()->CreateVertexBuffer( GetSize(), createFlags, 0, pool, &m_VB, 0 );
	OnFailedThrow( hr, "Failed to create vertex buffer!" );

	if( source )
	{
		unify::DataLock lock;
		Lock( lock );
		lock.CopyBytesFrom( source, 0, GetSize() );
	}
}

void VertexBuffer::Resize( unsigned int numVertices )
{
	if ( GetUsage() != BufferUsage::Staging )
	{
		throw unify::Exception( "VertexBuffer's usage is not Staging! Cannot resize without CPU read and write access." );
	}

	unsigned int oldNumberOfVertices = GetLength();
	size_t oldSize = GetSize();
	IDirect3DVertexBuffer9 * oldVertexBuffer = m_VB;
	m_VB = 0;
	m_length = 0;

	Create( numVertices, GetVertexDeclaration(), GetUsage(), nullptr, GetCreateFlags() );

	HRESULT hr;
	unsigned char * oldData;
	hr = oldVertexBuffer->Lock( 0, 0, (void**)&oldData, D3DLOCK_READONLY );
	if( FAILED(hr) )
	{
		throw exception::FailedToLock( "Failed to lock vertex buffer!" );
	}

	unsigned char * newData;
	hr = m_VB->Lock( 0, 0, (void**)&newData, D3DLOCK_DISCARD );
	if( FAILED(hr) )
	{
		oldVertexBuffer->Release();
		oldVertexBuffer = 0;
		throw exception::FailedToLock( "Failed to lock vertex buffer!" );
	}

	memcpy( newData, oldData, oldSize );
	m_VB->Unlock();
	oldVertexBuffer->Unlock();
	oldVertexBuffer->Release();
	oldVertexBuffer = 0;
}

size_t VertexBuffer::Append( const VertexBuffer & from )
{
	size_t offset = GetLength();
	size_t originalSizeInBytes = GetSize();

	if ( from.GetLength() == 0 )
	{
		return offset;
	}

	if ( from.GetUsage() != BufferUsage::Staging )
	{
		throw unify::Exception( "From VertexBuffer's usage is not Staging!" );
	}

	if ( GetLength() == 0 )
	{
		unsigned int length = from.GetLength();
		VertexDeclaration vd = from.GetVertexDeclaration();
		BufferUsage::TYPE usage = from.GetUsage();
		unsigned int createFlags = from.GetCreateFlags();
		Create( length, vd, usage, nullptr, createFlags );
	}
	else
	{
		if ( GetUsage() != BufferUsage::Staging )
		{
			throw unify::Exception( "VertexBuffer's usage is not Staging!" );
		}
		if ( m_vertexDeclaration != from.m_vertexDeclaration )
		{
			throw unify::Exception( "VertexBuffer's source and destination doesn't match for Append!" );
		}

		Resize( GetLength() + from.GetLength() );
	}

	// Copy vertices.5..
	unify::DataLock locksrc, lockdest;
	Lock( lockdest );

	from.LockReadOnly( locksrc );

	lockdest.CopyBytesFrom( locksrc.GetDataReadOnly(), originalSizeInBytes, locksrc.GetSizeInBytes() );
	Unlock();
	from.Unlock();
	return offset;
}

void VertexBuffer::Release()
{
	m_VB = nullptr;
	m_length = 0;
	m_stride = 0;
}

void VertexBuffer::Lock( unify::DataLock & lock )
{
	HRESULT hr;
	unsigned char * data;
	hr = m_VB->Lock( 0, 0, (void**)&data, 0 );
	if( FAILED( hr ) )
	{
		lock.Invalidate();
		throw exception::FailedToLock( "Failed to lock vertex buffer!" );
	}

	lock.SetLock( data, m_vertexDeclaration.GetSize(), GetLength(), false );
	m_locked = true;
}

void VertexBuffer::LockReadOnly( unify::DataLock & lock ) const
{
	HRESULT hr;
	unsigned char * data;
	hr = m_VB->Lock( 0, 0, (void**)&data, D3DLOCK_READONLY );
	if( FAILED( hr ) )
	{
		lock.Invalidate();
		throw exception::FailedToLock( "Failed to lock vertex buffer!" );
	}

	lock.SetLock( data, m_vertexDeclaration.GetSize(), GetLength(), true );
}

void VertexBuffer::Unlock()
{
	if( ! m_VB || ! m_locked ) return;
	m_VB->Unlock();
	m_locked = FALSE;	// altering data in a constant function
}

void VertexBuffer::Unlock() const
{
	if( !m_VB || !m_locked ) return;
	m_VB->Unlock();
	bool & locked = *const_cast< bool* >( &m_locked );
	locked = false;
}

void VertexBuffer::Upload( const void * pVerticesIn, unsigned int uStartVert, unsigned int uNumVerts )
{
	if( ( ( uStartVert + uNumVerts ) - 1 ) >= GetLength() )
	{
		throw unify::Exception( "Number of vertices to upload exceeds available area to upload to!" );
	}

	unify::DataLock lock;

	Lock( lock );
	lock.CopyBytesFrom( (unsigned char*)pVerticesIn, GetStride() * uStartVert, uNumVerts * GetStride() );
	Unlock();
}

VertexDeclaration VertexBuffer::GetVertexDeclaration() const
{
	return m_vertexDeclaration;
}

unify::Flags VertexBuffer::GetCreateFlags() const
{
	return m_createFlags;
}

// Checks if the vertex buffer has been created.
bool VertexBuffer::Valid() const
{
	return m_VB != 0;
}

void VertexBuffer::Use( unsigned int streamNumber, unsigned int offsetInBytes, unsigned int stride ) const
{
	HRESULT hr = S_OK;
	
	hr = win::DX::GetDxDevice()->SetStreamSource( streamNumber, m_VB, offsetInBytes, stride != 0 ? stride : GetStride() );
	if ( FAILED( hr ) )
	{
		throw unify::Exception( "VertexBuffer: Failed to SetStreamSource!" );
	}
}

void VertexBuffer::Disuse() const
{
	win::DX::GetDxDevice()->SetStreamSource( 0, 0, 0, 0 );
}