// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <medx9/IndexBuffer.h>
#include <me/exception/FailedToCreate.h>
#include <me/exception/FailedToLock.h>
#include <me/exception/OutOfBounds.h>
#include <me/exception/NotImplemented.h>
#include <atlbase.h>

using namespace medx9;
using namespace me;

IndexBuffer::IndexBuffer( const me::IRenderer * renderer )
	: m_renderer( dynamic_cast< const Renderer * >(renderer) )
	, m_buffer{ 0 }
	, m_locked( false )
	, m_usage( BufferUsage::Default )
	, m_length( 0 )
	, m_stride( 0 )
{
}

IndexBuffer::IndexBuffer( const me::IRenderer * renderer, IndexBufferParameters parameters )
	: IndexBuffer( renderer )
{
	Create( parameters );
}

IndexBuffer::~IndexBuffer()
{
	Destroy();
}

void IndexBuffer::Create( IndexBufferParameters parameters )
{
	Destroy();

	HRESULT hr = S_OK;

	m_length = parameters.countAndSource[0].count;

	auto dxDevice = m_renderer->GetDxDevice();

	unsigned int createFlags = FLAGNULL;
	// Some of these are not required.
	if ( unify::CheckFlag( parameters.flags, CreateFlags::DoNotClip ) )	createFlags |= D3DUSAGE_DONOTCLIP;
	if ( unify::CheckFlag( parameters.flags, CreateFlags::NPatches ) )	createFlags |= D3DUSAGE_NPATCHES;
	if ( unify::CheckFlag( parameters.flags, CreateFlags::Points ) )		createFlags |= D3DUSAGE_POINTS;
	if ( unify::CheckFlag( parameters.flags, CreateFlags::RTPatches ) )	createFlags |= D3DUSAGE_RTPATCHES;
	if ( unify::CheckFlag( parameters.flags, CreateFlags::SoftwareProcessing ) )	createFlags |= D3DUSAGE_SOFTWAREPROCESSING;
	if ( unify::CheckFlag( parameters.flags, CreateFlags::WriteOnly ) )	createFlags |= D3DUSAGE_WRITEONLY;

	D3DFORMAT d3dFormat;
	d3dFormat = D3DFMT_INDEX32;
	m_stride = sizeof( Index32 );

	m_createFlags = createFlags;
	m_usage = parameters.usage;

	D3DPOOL pool = D3DPOOL_DEFAULT;
	switch ( m_usage )
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

	// Create Index Buffer...
	hr = dxDevice->CreateIndexBuffer(
		GetSizeInBytes(),
		createFlags,
		d3dFormat,
		pool,
		&m_buffer,
		NULL
	);
	if ( FAILED( hr ) )
	{
		throw exception::FailedToCreate( "Failed to create index buffer!" );
	}

	if ( parameters.countAndSource[0].source )
	{
		// Create the index list...
		unify::DataLock lock;
		Lock( lock );
		memcpy( lock.GetData(), parameters.countAndSource[0].source, m_length * sizeof( Index32 ) );
		Unlock( lock );
	}
}

void IndexBuffer::Destroy()
{
	if ( m_buffer )
	{
		m_buffer->Release();
	}
	m_buffer = nullptr;
	m_length = 0;
}

void IndexBuffer::Lock( unify::DataLock & lock )
{
	HRESULT hr;
	unsigned char * data;
	hr = m_buffer->Lock( 0, 0, (void**)&data, 0 );
	if ( FAILED( hr ) )
	{
		lock.Invalidate();
		throw exception::FailedToLock( "Failed to lock index buffer!" );
	}

	lock.SetLock( data, GetStride(), GetLength(), false, 0 );
	m_locked = true;
}

void IndexBuffer::LockReadOnly( unify::DataLock & lock ) const
{
	assert( m_buffer != 0 );
	if ( m_locked )
	{
		throw exception::FailedToLock( "Index buffer already locked!" );
	}

	void * data = 0;
	if ( FAILED( m_buffer->Lock( 0, 0, &data, D3DLOCK_READONLY ) ) )
	{
		throw exception::FailedToLock( "Failed to lock indices!" );
	}
	lock.SetLock( data, GetStride(), GetLength(), true, 0 );
	bool & locked = *const_cast<bool*>(&m_locked); // Break const for locking.
	locked = false;
}

void IndexBuffer::Unlock( unify::DataLock & lock )
{
	if ( m_locked == false )
	{
		return;
	}
	m_buffer->Unlock();
	m_locked = false;
}

void IndexBuffer::UnlockReadOnly( unify::DataLock & lock ) const
{
	if ( m_locked == false )
	{
		return;
	}
	m_buffer->Unlock();
	bool & locked = *const_cast<bool*>(&m_locked); // Break const for locking.
	locked = false;
}

bool IndexBuffer::Valid() const
{
	return m_buffer != nullptr;
}

void IndexBuffer::Use() const
{
	if ( !m_buffer )
	{
		return;
	}

	auto dxDevice = m_renderer->GetDxDevice();

	HRESULT hr = dxDevice->SetIndices( m_buffer );
	if ( FAILED( hr ) )
	{
		throw unify::Exception( "Failed to use index buffer!" );
	}
}

bool IndexBuffer::Locked() const
{
	return m_locked;
}

BufferUsage::TYPE IndexBuffer::GetUsage() const
{
	return m_usage;
}

unsigned int IndexBuffer::GetStride() const
{
	return m_stride;
}

unsigned int IndexBuffer::GetLength() const
{
	return m_length;
}

size_t IndexBuffer::GetSizeInBytes() const
{
	return m_stride * m_length;
}
