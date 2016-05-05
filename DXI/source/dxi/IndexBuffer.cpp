// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/exception/FailedToCreate.h>
#include <dxi/exception/FailedToLock.h>
#include <dxi/exception/OutOfBounds.h>
#include <dxi/IndexBuffer.h>
#include <unify/Flags.h>
#include <dxi/core/Game.h>
#include <assert.h>

using namespace dxi;

IndexBuffer::IndexBuffer()
: m_IB( 0 )
{
}

IndexBuffer::~IndexBuffer()
{
	Release();
}

void IndexBuffer::Create( unsigned int uNumIndices, IndexFormat::TYPE format, BufferUsage::TYPE usage, unify::Flags flags )
{
	Release();

	unsigned int createFlags = FLAGNULL;
	// Some of these are not required.
	if( unify::CheckFlag( flags, CreateFlags::DoNotClip ) )	createFlags |= D3DUSAGE_DONOTCLIP;
	if( unify::CheckFlag( flags, CreateFlags::NPatches ) )	createFlags |= D3DUSAGE_NPATCHES;
	if( unify::CheckFlag( flags, CreateFlags::Points ) )		createFlags |= D3DUSAGE_POINTS;
	if( unify::CheckFlag( flags, CreateFlags::RTPatches ) )	createFlags |= D3DUSAGE_RTPATCHES;
	if( unify::CheckFlag( flags, CreateFlags::SoftwareProcessing ) )	createFlags |= D3DUSAGE_SOFTWAREPROCESSING;
	if( unify::CheckFlag( flags, CreateFlags::WriteOnly ) )	createFlags |= D3DUSAGE_WRITEONLY;

	D3DFORMAT d3dFormat;
	switch( format )
	{
	default:
	case IndexFormat::Index16:
		d3dFormat = D3DFMT_INDEX16;
		m_stride = sizeof( Index16 );
		break;
	case IndexFormat::Index32:
		d3dFormat = D3DFMT_INDEX32;
		m_stride = sizeof( Index32 );
		break;
	}

	m_format = format;
	m_createFlags = flags;
	m_usage = usage;

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
	
	m_length = uNumIndices;

	// Create Index Buffer...
	HRESULT hr;
	hr = win::DX::GetDxDevice()->CreateIndexBuffer(
		GetSize(),
		createFlags,
		d3dFormat,
		pool,
		&m_IB,
		NULL			
	);
	if( FAILED( hr ) )
	{
		throw exception::FailedToCreate( "Failed to create index buffer!" );
	}

	m_length = uNumIndices;
}

void IndexBuffer::Resize( unsigned int numIndices )
{
	if ( GetUsage() != BufferUsage::Staging )
	{
		throw unify::Exception( "IndexBuffer's usage is not Staging! Cannot resize without CPU read and write access." );
	}

	unsigned int oldNumberOfIndices = GetLength();
	size_t oldSize = GetSize();
	IDirect3DIndexBuffer9 * oldIndexBuffer = m_IB;
	m_IB = 0;
	m_length = 0;

	Create( numIndices, this->GetFormat(), GetUsage(), this->GetCreateFlags() );

	HRESULT hr;
	unsigned char * oldData;
	hr = oldIndexBuffer->Lock( 0, 0, (void**)&oldData, D3DLOCK_READONLY );
	if( FAILED(hr) )
	{
		throw exception::FailedToLock( "Failed to lock index buffer!" );
	}

	unsigned char * newData;
	hr = m_IB->Lock( 0, 0, (void**)&newData, D3DLOCK_DISCARD );
	if( FAILED(hr) )
	{
		oldIndexBuffer->Release();
		oldIndexBuffer = 0;
		throw exception::FailedToLock( "Failed to lock index buffer!" );
	}

	memcpy( newData, oldData, oldSize );
	m_IB->Unlock();
	oldIndexBuffer->Unlock();
	oldIndexBuffer->Release();
	oldIndexBuffer = 0;}

size_t IndexBuffer::Append( const IndexBuffer & from, size_t vertexOffset  )
{
	size_t offset = GetLength();

	if ( from.GetLength() == 0 )
	{
		return offset;
	}

	if ( from.GetUsage() != BufferUsage::Staging )
	{
		throw unify::Exception( "From IndexBuffer's usage is not Staging!" );
	}

	if ( GetLength() == 0 )
	{
		Create( from.GetLength(), from.GetFormat(), from.GetUsage(), from.GetCreateFlags() );
	}
	else
	{
		if ( GetUsage() != BufferUsage::Staging )
		{
			throw unify::Exception( "IndexBuffer's usage is not Staging!" );
		}
		Resize( GetLength() + from.GetLength() );
	}

	// Copy vertices...
	IndexLock locksrc, lockdest;
	Lock( lockdest );

	from.LockReadOnly( locksrc );

	for( size_t i = 0; i < locksrc.Count(); ++i )
	{
		size_t to = offset + i;
		const void * rawData = locksrc.GetItemReadOnly( i );
		if ( locksrc.Stride() == 2 )
		{
			unsigned short fromIndex = *static_cast< const short int * >( rawData );
			lockdest.SetIndex( to, fromIndex + vertexOffset );
		}
		else
		{
			unsigned int fromIndex = *static_cast< const unsigned int * >( rawData );
			lockdest.SetIndex( to, fromIndex + vertexOffset );
		}
	}
	
	from.UnlockReadOnly();
	Unlock();

	return offset;
}

void IndexBuffer::Release()
{
	if( m_IB )
	{
		m_IB->Release();
		m_IB = 0;
	}
	m_length = 0;
}

void IndexBuffer::Lock( IndexLock & lock )
{
	HRESULT hr;
	unsigned char * data;
	hr = m_IB->Lock( 0, 0, (void**)&data, 0 );
	if( FAILED(hr) )
	{
		lock.Invalidate();
		throw exception::FailedToLock( "Failed to lock index buffer!" );
	}

	lock.SetLock( data, GetStride(), GetLength(), false );
	m_locked = true;
}

void IndexBuffer::LockReadOnly( IndexLock & lock ) const
{
	assert( m_IB != 0 );
	if( m_locked )
	{
		throw exception::FailedToLock( "Index buffer already locked!" );
	}

	void * data = 0;
	if( FAILED( m_IB->Lock( 0, 0, &data, D3DLOCK_READONLY ) ) )
	{
		throw exception::FailedToLock( "Failed to lock indices!" );
	}
	lock.SetLock( data, GetStride(), GetLength(), true );
	bool & locked = *const_cast<bool*>( &m_locked ); // Break const for locking.
	locked = false;
}

void IndexBuffer::Unlock()
{
	if( m_locked == false )
	{
		return;
	}
	m_IB->Unlock();
	m_locked = false;
}

void IndexBuffer::UnlockReadOnly() const
{
	if( m_locked == false )
	{
		return;
	}
	m_IB->Unlock();
	bool & locked = *const_cast<bool*>( &m_locked ); // Break const for locking.
	locked = false;
}

unsigned int IndexBuffer::GetCreateFlags() const
{
	return m_createFlags;
}

bool IndexBuffer::Valid() const
{
	return m_IB != 0;
}

void IndexBuffer::Use() const
{
	if( ! m_IB )
	{
		return;
	}	

	HRESULT hr = win::DX::GetDxDevice()->SetIndices( m_IB );
	if ( FAILED( hr ) )
	{
		throw unify::Exception( "Failed to use index buffer!" );
	}
}

void IndexBuffer::Disuse() const
{
	if( GetLength() )
	{
		win::DX::GetDxDevice()->SetIndices( 0 );
	}
}

IndexFormat::TYPE IndexBuffer::GetFormat() const
{
	return m_format;
}