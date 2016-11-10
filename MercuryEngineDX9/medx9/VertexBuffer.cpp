// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <medx9/VertexBuffer.h>
#include <me/exception/FailedToCreate.h>
#include <me/exception/FailedToLock.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/Exception.h>

using namespace medx9;
using namespace me;

VertexBuffer::VertexBuffer( me::IRenderer * renderer )
	: m_renderer( dynamic_cast< Renderer * >( renderer ) )
	, m_slot( 0 )
	, m_locked( false )
	, m_usage( BufferUsage::Default )
	, m_length( 0 )
	, m_stride( 0 )
{
}

VertexBuffer::VertexBuffer( me::IRenderer * renderer, me::VertexBufferParameters parameters )
	: m_renderer( dynamic_cast< Renderer * >( renderer ) )
	, m_slot( 0 )
	, m_locked( false )
	, m_usage( BufferUsage::Default )
	, m_length( 0 )
	, m_stride( 0 )
{
	Create( parameters );
}


VertexBuffer::~VertexBuffer()
{
	Destroy();
}

void VertexBuffer::Create( VertexBufferParameters parameters )
{
	Destroy();

	m_vertexDeclaration = parameters.vertexDeclaration;
	m_slot = parameters.slot;
	m_stride = m_vertexDeclaration->GetSize( parameters.slot );
	m_length = parameters.numVertices;
	m_usage = parameters.usage;

	// Ensure we have some sort of idea what we need to be...
	if( GetSize() == 0 )
	{
		throw exception::FailedToCreate( "Not a valid vertex buffer size!" );
	}

	auto dxDevice = m_renderer->GetDxDevice();

	unsigned int createFlags = FLAGNULL;
	/*
	// TODO: To support for DX9 and DX11...
	// Build Vertex Buffer usage...
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
	*/

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
	hr = dxDevice->CreateVertexBuffer( GetSize(), createFlags, 0, pool, &m_VB, 0 );
	OnFailedThrow( hr, "Failed to create vertex buffer!" );

	if( parameters.source )
	{
		unify::DataLock lock;
		Lock( lock );
		lock.CopyBytesFrom( parameters.source, 0, GetSize() );
	}

		
	m_bbox = parameters.bbox;
}

void VertexBuffer::Destroy()
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

	lock.SetLock( data, m_vertexDeclaration->GetSize( 0 ), GetLength(), false, 0 );
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

	lock.SetLock( data, m_vertexDeclaration->GetSize( 0 ), GetLength(), true, 0 );
}

void VertexBuffer::Unlock()
{
	if( !m_VB || !m_locked ) return;
	m_VB->Unlock();
	m_locked = FALSE;	// altering data in a constant function
}

void VertexBuffer::Unlock() const
{
	if( !m_VB || !m_locked ) return;
	m_VB->Unlock();
	bool & locked = *const_cast< bool* >(&m_locked);
	locked = false;
}


IVertexDeclaration::ptr VertexBuffer::GetVertexDeclaration() const
{
	return m_vertexDeclaration;
}

size_t VertexBuffer::GetSlot() const
{
	return m_slot;
}

bool VertexBuffer::Valid() const
{
	return m_VB != 0;
}

void VertexBuffer::Use() const
{
	unsigned int streamNumber = 0;
	unsigned int stride = GetStride();
	unsigned int offsetInBytes = 0;
	HRESULT hr = S_OK;

	auto dxDevice = m_renderer->GetDxDevice();

	hr = dxDevice->SetStreamSource( streamNumber, m_VB, offsetInBytes, stride );
	if( FAILED( hr ) )
	{
		throw unify::Exception( "VertexBuffer: Failed to SetStreamSource!" );
	}
}

unify::BBox< float > & VertexBuffer::GetBBox()
{
	return m_bbox;
}

const unify::BBox< float > & VertexBuffer::GetBBox() const
{
	return m_bbox;
}

bool VertexBuffer::Locked() const
{
	return m_locked;
}

BufferUsage::TYPE VertexBuffer::GetUsage() const
{
	return m_usage;
}

unsigned int VertexBuffer::GetStride() const
{
	return m_stride;
}

unsigned int VertexBuffer::GetLength() const
{
	return m_length;
}

unsigned int VertexBuffer::GetSize() const
{
	return m_stride * m_length;
}
