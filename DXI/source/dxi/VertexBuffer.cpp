// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/VertexBuffer.h>
#include <dxi/win/DXRenderer.h>
#include <dxi/exception/FailedToCreate.h>
#include <dxi/exception/FailedToLock.h>
#include <dxi/exception/Exception.h>
#include <atlbase.h>

using namespace dxi;

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
#if defined( DIRECTX9 )
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
#elif defined( DIRECTX11 )
		auto dxDevice = m_renderer->GetDxDevice();

		// Ensure that if we are BufferUsage::Immutable, then source is not null.
		if ( BufferUsage::Immutable && source == nullptr )
		{
			throw exception::FailedToCreate( "Buffer is immutable, yet source is null!" );
		}

		D3D11_USAGE usageDX{};
		switch( m_owner.m_usage )
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
			 
		D3D11_BUFFER_DESC vertexBufferDesc {};
		vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBufferDesc.ByteWidth = vertexDeclaration->GetSize() * uNumVertices;
		vertexBufferDesc.Usage = usageDX;

		HRESULT result;
		if ( source != nullptr )
		{
			D3D11_SUBRESOURCE_DATA initialData {};
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
#endif
	}

	void Resize( unsigned int numVertices )
	{
#if defined( DIRECTX9 )
		unsigned int oldNumberOfVertices = m_owner.GetLength();
		size_t oldSize = m_owner.GetSize();
		IDirect3DVertexBuffer9 * oldVertexBuffer = m_VB;
		m_VB = 0;
		m_owner.m_length = 0;

		Create( numVertices, m_owner.GetVertexDeclaration(), nullptr,  m_owner.GetUsage() );

		HRESULT hr;
		unsigned char * oldData;
		hr = oldVertexBuffer->Lock( 0, 0, (void**)&oldData, D3DLOCK_READONLY );
		if( FAILED( hr ) )
		{
			throw exception::FailedToLock( "Failed to lock vertex buffer!" );
		}

		unsigned char * newData;
		hr = m_VB->Lock( 0, 0, (void**)&newData, D3DLOCK_DISCARD );
		if( FAILED( hr ) )
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
#elif defined( DIRECTX11 )
#endif
	}

	size_t Append( const VertexBuffer & from )
	{
		size_t offset = m_owner.GetLength();
		size_t originalSizeInBytes = m_owner.GetSize();

		if( from.GetLength() == 0 )
		{
			return offset;
		}

		if( from.GetUsage() != BufferUsage::Staging )
		{
			throw unify::Exception( "From VertexBuffer's usage is not Staging!" );
		}

		if( m_owner.GetLength() == 0 )
		{
			unsigned int length = from.GetLength();
			VertexDeclaration::ptr vd = from.GetVertexDeclaration();
			BufferUsage::TYPE usage = from.GetUsage();
			//unsigned int createFlags = from.GetCreateFlags();
			assert( 0 ); // TOOD:
			Create( length, vd, nullptr, usage /*, createFlags*/ );
		}
		else
		{
			if( m_owner.GetUsage() != BufferUsage::Staging )
			{
				throw unify::Exception( "VertexBuffer's usage is not Staging!" );
			}
			if( *m_owner.m_vertexDeclaration != *from.m_vertexDeclaration )
			{
				throw unify::Exception( "VertexBuffer's source and destination doesn't match for Append!" );
			}

			Resize( m_owner.GetLength() + from.GetLength() );
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


	void Lock( unify::DataLock & lock )
	{
#if defined( DIRECTX9 )
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
#elif defined( DIRECTX11 )
#endif
	}

	void LockReadOnly( unify::DataLock & lock ) const
	{
#if defined( DIRECTX9 )
		HRESULT hr;
		unsigned char * data;
		hr = m_VB->Lock( 0, 0, (void**)&data, D3DLOCK_READONLY );
		if( FAILED( hr ) )
		{
			lock.Invalidate();
			throw exception::FailedToLock( "Failed to lock vertex buffer!" );
		}

		lock.SetLock( data, m_owner.m_vertexDeclaration->GetSize(), m_owner.GetLength(), true );
#elif defined( DIRECTX11 )
#endif
	}

	void Unlock()
	{
#if defined( DIRECTX9 )
		if( !m_VB || !m_owner.m_locked ) return;
		m_VB->Unlock();
		m_owner.m_locked = FALSE;	// altering data in a constant function
#elif defined( DIRECTX11 )
#endif
	}

	void Unlock() const
	{
#if defined( DIRECTX9 )
		if( !m_VB || !m_owner.m_locked ) return;
		m_VB->Unlock();
		bool & locked = *const_cast< bool* >(&m_owner.m_locked);
		locked = false;
#elif defined( DIRECTX11 )
#endif
	}

	void Upload( const void * pVerticesIn, unsigned int uStartVert, unsigned int uNumVerts )
	{
#if defined( DIRECTX9 )
		if( ((uStartVert + uNumVerts) - 1) >= m_owner.GetLength() )
		{
			throw unify::Exception( "Number of vertices to upload exceeds available area to upload to!" );
		}

		unify::DataLock lock;

		Lock( lock );
		lock.CopyBytesFrom( (unsigned char*)pVerticesIn, m_owner.GetStride() * uStartVert, uNumVerts * m_owner.GetStride() );
		Unlock();
#elif defined( DIRECTX11 )
#endif
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
#if defined( DIRECTX9 )
		HRESULT hr = S_OK;

		auto dxDevice = m_renderer->GetDxDevice();

		hr = dxDevice->SetStreamSource( streamNumber, m_VB, offsetInBytes, stride );
		if( FAILED( hr ) )
		{
			throw unify::Exception( "VertexBuffer: Failed to SetStreamSource!" );
		}
#elif defined( DIRECTX11 )
		auto dxContext = m_renderer->GetDxContext();
		dxContext->IASetVertexBuffers( streamNumber, 1, &m_VB.p, &stride, &offsetInBytes );
#endif
	}

	VertexBuffer & m_owner;
	win::DXRenderer * m_renderer;

#if defined( DIRECTX9 )
	CComPtr< IDirect3DVertexBuffer9 > m_VB;
#elif defined( DIRECTX11 )
	CComPtr< ID3D11Buffer > m_VB;
#endif	
};


VertexBuffer::VertexBuffer( core::IRenderer * renderer )
	: m_pimpl( new Pimpl( *this, renderer ) )
{
}

VertexBuffer::VertexBuffer( core::IRenderer * renderer, unsigned int numVertices, VertexDeclaration::ptr vertexDeclaration, const void * source, BufferUsage::TYPE usage/*, unify::Flags flags*/ )
	: m_pimpl( new Pimpl( *this, renderer ) )
{
	Create( numVertices, vertexDeclaration, source, usage/*, flags*/ );
}

VertexBuffer::~VertexBuffer()
{
	Destroy();
}

void VertexBuffer::Create( unsigned int uNumVertices, VertexDeclaration::ptr vertexDeclaration, const void * source, BufferUsage::TYPE usage )
{
	Destroy();

	m_vertexDeclaration = vertexDeclaration;
	m_stride = m_vertexDeclaration->GetSize();
	m_length = uNumVertices;
	m_usage = usage;

	// Ensure we have some sort of idea what we need to be...
	if( GetSize() == 0 )
	{
		throw exception::FailedToCreate( "Not a valid vertex buffer size!" );
	}

	m_pimpl->Create( uNumVertices, vertexDeclaration, source, usage );
}

void VertexBuffer::Resize( unsigned int numVertices )
{
	if ( GetUsage() != BufferUsage::Staging )
	{
		throw unify::Exception( "VertexBuffer's usage is not Staging! Cannot resize without CPU read and write access." );
	}

	m_pimpl->Resize( numVertices );
}

size_t VertexBuffer::Append( const VertexBuffer & from )
{
	return m_pimpl->Append( from );
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

void VertexBuffer::Upload( const void * pVerticesIn, unsigned int uStartVert, unsigned int uNumVerts )
{
	m_pimpl->Upload( pVerticesIn, uStartVert, uNumVerts );
}

VertexDeclaration::ptr VertexBuffer::GetVertexDeclaration() const
{
	return m_vertexDeclaration;
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
