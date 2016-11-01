// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/VertexBuffer.h>
#include <dxi/win/DXRenderer.h>
#include <dxi/exception/FailedToCreate.h>
#include <dxi/exception/FailedToLock.h>
#include <dxi/exception/NotImplemented.h>
#include <dxi/exception/Exception.h>
#include <atlbase.h>

using namespace dxi;


#if defined( DIRECTX9 )


class VertexBuffer::Pimpl
{
public:
	Pimpl( VertexBuffer & owner, core::IRenderer * renderer )
		: m_owner( owner )
		, m_renderer( dynamic_cast< win::DXRenderer * >( renderer ) )
	{
	}
	
	~Pimpl()
	{
	}

	void Destroy()
	{
		m_VB = nullptr;
	}

	void Create( unsigned int uNumVertices, VertexDeclaration::ptr vertexDeclaration, const void * source, BufferUsage::TYPE usage )
	{
		auto dxDevice = m_renderer->GetDxDevice();

		unsigned int createFlags = FLAGNULL;
		/*
		// TODO: To support for DX9 and DX11...
		// Build Vertex Buffer usage...
		if( unify::CheckFlag( m_owner.m_createFlags, VertexBuffer::CreateFlags::DoNotClip ) )
		{
			createFlags |= D3DUSAGE_DONOTCLIP;
		}
		if( unify::CheckFlag( m_owner.m_createFlags, VertexBuffer::CreateFlags::NPatches ) )
		{
			createFlags |= D3DUSAGE_NPATCHES;
		}
		if( unify::CheckFlag( m_owner.m_createFlags, VertexBuffer::CreateFlags::Points ) )
		{
			createFlags |= D3DUSAGE_POINTS;
		}
		if( unify::CheckFlag( m_owner.m_createFlags, VertexBuffer::CreateFlags::RTPatches ) )
		{
			createFlags |= D3DUSAGE_RTPATCHES;
		}
		*/

		D3DPOOL pool = D3DPOOL_DEFAULT;
		switch( m_owner.m_usage )
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
		hr = dxDevice->CreateVertexBuffer( m_owner.GetSize(), createFlags, 0, pool, &m_VB, 0 );
		OnFailedThrow( hr, "Failed to create vertex buffer!" );

		if( source )
		{
			unify::DataLock lock;
			m_owner.Lock( lock );
			lock.CopyBytesFrom( source, 0, m_owner.GetSize() );
		}
	}

	void Lock( unify::DataLock & lock )
	{
		HRESULT hr;
		unsigned char * data;
		hr = m_VB->Lock( 0, 0, (void**)&data, 0 );
		if( FAILED( hr ) )
		{
			lock.Invalidate();
			throw exception::FailedToLock( "Failed to lock vertex buffer!" );
		}

		lock.SetLock( data, m_owner.m_vertexDeclaration->GetSize(), m_owner.GetLength(), false );
		m_owner.m_locked = true;
	}

	void LockReadOnly( unify::DataLock & lock ) const
	{
		HRESULT hr;
		unsigned char * data;
		hr = m_VB->Lock( 0, 0, (void**)&data, D3DLOCK_READONLY );
		if( FAILED( hr ) )
		{
			lock.Invalidate();
			throw exception::FailedToLock( "Failed to lock vertex buffer!" );
		}

		lock.SetLock( data, m_owner.m_vertexDeclaration->GetSize(), m_owner.GetLength(), true );
	}

	void Unlock()
	{
		if( !m_VB || !m_owner.m_locked ) return;
		m_VB->Unlock();
		m_owner.m_locked = FALSE;	// altering data in a constant function
	}

	void Unlock() const
	{
		if( !m_VB || !m_owner.m_locked ) return;
		m_VB->Unlock();
		bool & locked = *const_cast< bool* >(&m_owner.m_locked);
		locked = false;
	}

	bool Valid() const
	{
		return m_VB != 0;
	}

	void Use() const
	{
		unsigned int streamNumber = 0;
		unsigned int stride = m_owner.GetStride();
		unsigned int offsetInBytes = 0;
		HRESULT hr = S_OK;

		auto dxDevice = m_renderer->GetDxDevice();

		hr = dxDevice->SetStreamSource( streamNumber, m_VB, offsetInBytes, stride );
		if( FAILED( hr ) )
		{
			throw unify::Exception( "VertexBuffer: Failed to SetStreamSource!" );
		}
	}

	VertexBuffer & m_owner;
	win::DXRenderer * m_renderer;

	CComPtr< IDirect3DVertexBuffer9 > m_VB;
};


#elif defined( DIRECTX11 )


class VertexBuffer::Pimpl
{
public:
	Pimpl( VertexBuffer & owner, core::IRenderer * renderer )
		: m_owner( owner )
		, m_renderer( dynamic_cast< win::DXRenderer * >(renderer) )
	{
	}

	~Pimpl()
	{
	}

	void Destroy()
	{
		m_VB = nullptr;
	}

	void Create( unsigned int uNumVertices, VertexDeclaration::ptr vertexDeclaration, const void * source, BufferUsage::TYPE usage )
	{
		auto dxDevice = m_renderer->GetDxDevice();

		// Ensure that if we are BufferUsage::Immutable, then source is not null.
		if ( BufferUsage::Immutable && source == nullptr )
		{
			throw exception::FailedToCreate( "Buffer is immutable, yet source is null!" );
		}

		D3D11_USAGE usageDX{};
		switch ( m_owner.m_usage )
		{
		case BufferUsage::Default:
			usageDX = D3D11_USAGE_DEFAULT;
			break;
		case BufferUsage::Immutable:
			usageDX = D3D11_USAGE_IMMUTABLE;
			break;
		case BufferUsage::Dynamic:
			usageDX = D3D11_USAGE_DYNAMIC;
			break;
		case BufferUsage::Staging:
			usageDX = D3D11_USAGE_STAGING;
			break;
		}

		D3D11_BUFFER_DESC vertexBufferDesc{};
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = vertexDeclaration->GetSize() * uNumVertices;
		vertexBufferDesc.Usage = usageDX;

		HRESULT result;
		if ( source != nullptr )
		{
			D3D11_SUBRESOURCE_DATA initialData{};
			initialData.pSysMem = source;
			if ( usage == BufferUsage::Dynamic )
			{
				vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // TODO: make not hard coded.
			}
			result = dxDevice->CreateBuffer( &vertexBufferDesc, &initialData, &m_VB );
		}
		else
		{
			result = dxDevice->CreateBuffer( &vertexBufferDesc, nullptr, &m_VB );
		}
		OnFailedThrow( result, "Failed to create vertex buffer!" );
	}

	void Lock( unify::DataLock & lock )
	{
		throw exception::NotImplemented();
	}

	void LockReadOnly( unify::DataLock & lock ) const
	{
		throw exception::NotImplemented();
	}

	void Unlock()
	{
		throw exception::NotImplemented();
	}

	void Unlock() const
	{
		throw exception::NotImplemented();
	}

	bool Valid() const
	{
		return m_VB != 0;
	}

	void Use() const
	{
		unsigned int streamNumber = 0;
		unsigned int stride = m_owner.GetStride();
		unsigned int offsetInBytes = 0;
		auto dxContext = m_renderer->GetDxContext();
		dxContext->IASetVertexBuffers( streamNumber, 1, &m_VB.p, &stride, &offsetInBytes );
	}

	VertexBuffer & m_owner;
	win::DXRenderer * m_renderer;

	CComPtr< ID3D11Buffer > m_VB;
};


#endif


VertexBuffer::VertexBuffer( core::IRenderer * renderer )
	: m_pimpl( new Pimpl( *this, renderer ) )
	, m_slot( 0 )
	, m_locked( false )
	, m_usage( BufferUsage::Default )
	, m_length( 0 )
	, m_stride( 0 )
{
}

VertexBuffer::VertexBuffer( core::IRenderer * renderer, unsigned int numVertices, VertexDeclaration::ptr vertexDeclaration, size_t slot, const void * source, BufferUsage::TYPE usage )
	: m_pimpl( new Pimpl( *this, renderer ) )
	, m_slot( slot )
	, m_locked( false )
	, m_usage( BufferUsage::Default )
	, m_length( 0 )
	, m_stride( 0 )
{
	Create( numVertices, vertexDeclaration, slot, source, usage );
}

VertexBuffer::~VertexBuffer()
{
	Destroy();
}

void VertexBuffer::Create( unsigned int uNumVertices, VertexDeclaration::ptr vertexDeclaration, size_t slot, const void * source, BufferUsage::TYPE usage )
{
	Destroy();

	m_vertexDeclaration = vertexDeclaration;
	m_slot = slot;
	m_stride = m_vertexDeclaration->GetSize( slot );
	m_length = uNumVertices;
	m_usage = usage;

	// Ensure we have some sort of idea what we need to be...
	if( GetSize() == 0 )
	{
		throw exception::FailedToCreate( "Not a valid vertex buffer size!" );
	}

	m_pimpl->Create( uNumVertices, vertexDeclaration, source, usage );
}

void VertexBuffer::Destroy()
{
	m_pimpl->Destroy();
	m_length = 0;
	m_stride = 0;
}

void VertexBuffer::Lock( unify::DataLock & lock )
{
	m_pimpl->Lock( lock );
}

void VertexBuffer::LockReadOnly( unify::DataLock & lock ) const
{
	m_pimpl->LockReadOnly( lock );
}

void VertexBuffer::Unlock()
{
	m_pimpl->Unlock();
}

void VertexBuffer::Unlock() const
{
	m_pimpl->Unlock();
}

VertexDeclaration::ptr VertexBuffer::GetVertexDeclaration() const
{
	return m_vertexDeclaration;
}

size_t VertexBuffer::GetSlot() const
{
	return m_slot;
}

bool VertexBuffer::Valid() const
{
	return m_pimpl->Valid();
}

void VertexBuffer::Use() const
{
	m_pimpl->Use();
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
