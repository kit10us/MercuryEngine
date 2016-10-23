// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/IndexBuffer.h>
#include <dxi/win/DXRenderer.h>
#include <dxi/exception/FailedToCreate.h>
#include <dxi/exception/FailedToLock.h>
#include <dxi/exception/OutOfBounds.h>
#include <dxi/exception/NotImplemented.h>
#include <atlbase.h>

using namespace dxi;

#if defined(DIRECTX9)

class IndexBuffer::Pimpl
{
public:
	IndexBuffer & m_owner;
	win::DXRenderer * m_renderer;
	unsigned int m_createFlags;

	CComPtr< IDirect3DIndexBuffer9 > m_IB;

	Pimpl( IndexBuffer & owner, core::IRenderer * renderer )
		: m_owner( owner )
		, m_renderer( dynamic_cast< win::DXRenderer * >(renderer) )
	{
	}
	
	Pimpl::~Pimpl()
	{
		Destroy();
	}

	void Create( unsigned int uNumIndices, Index32 * source, BufferUsage::TYPE usage, unify::Flags flags )
	{
		Destroy();

		HRESULT hr = S_OK;

		m_owner.m_length = uNumIndices;

		auto dxDevice = m_renderer->GetDxDevice();

		unsigned int createFlags = FLAGNULL;
		// Some of these are not required.
		if ( unify::CheckFlag( flags, CreateFlags::DoNotClip ) )	createFlags |= D3DUSAGE_DONOTCLIP;
		if ( unify::CheckFlag( flags, CreateFlags::NPatches ) )	createFlags |= D3DUSAGE_NPATCHES;
		if ( unify::CheckFlag( flags, CreateFlags::Points ) )		createFlags |= D3DUSAGE_POINTS;
		if ( unify::CheckFlag( flags, CreateFlags::RTPatches ) )	createFlags |= D3DUSAGE_RTPATCHES;
		if ( unify::CheckFlag( flags, CreateFlags::SoftwareProcessing ) )	createFlags |= D3DUSAGE_SOFTWAREPROCESSING;
		if ( unify::CheckFlag( flags, CreateFlags::WriteOnly ) )	createFlags |= D3DUSAGE_WRITEONLY;

		D3DFORMAT d3dFormat;
		d3dFormat = D3DFMT_INDEX32;
		m_owner.m_stride = sizeof( Index32 );

		m_createFlags = flags;
		m_owner.m_usage = usage;

		D3DPOOL pool = D3DPOOL_DEFAULT;
		switch ( m_owner.m_usage )
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
			m_owner.GetSize(),
			createFlags,
			d3dFormat,
			pool,
			&m_IB,
			NULL
		);
		if ( FAILED( hr ) )
		{
			throw exception::FailedToCreate( "Failed to create index buffer!" );
		}

		if ( source )
		{
			// Create the index list...
			IndexLock indexLock;
			Lock( indexLock );
			memcpy( indexLock.GetData(), source, m_owner.m_length * sizeof( Index32 ) );
			Unlock();
		}
	}

	void Resize( unsigned int numIndices )
	{
		if ( m_owner.GetUsage() != BufferUsage::Staging )
		{
			throw unify::Exception( "IndexBuffer's usage is not Staging! Cannot resize without CPU read and write access." );
		}

		/*
		TODO:

		unsigned int oldNumberOfIndices = GetLength();
		size_t oldSize = GetSize();
		ID3D11Buffer * oldIndexBuffer = m_IB;
		m_IB = 0;
		m_length = 0;

		Create( numIndices, this->GetFormat(), GetUsage(), m_createFlags );

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
		oldIndexBuffer = 0;
		*/
	}

	size_t Append( const IndexBuffer & from, size_t vertexOffset )
	{
		size_t offset = m_owner.GetLength();

		if ( from.GetLength() == 0 )
		{
			return offset;
		}

		if ( from.GetUsage() != BufferUsage::Staging )
		{
			throw unify::Exception( "From IndexBuffer's usage is not Staging!" );
		}

		if ( m_owner.GetLength() == 0 )
		{
			assert( 0 ); // TODO:
			Create( from.GetLength(), nullptr, from.GetUsage(), from.m_pimpl->m_createFlags );
		}
		else
		{
			if ( m_owner.GetUsage() != BufferUsage::Staging )
			{
				throw unify::Exception( "IndexBuffer's usage is not Staging!" );
			}
			Resize( m_owner.GetLength() + from.GetLength() );
		}

		// Copy vertices...
		IndexLock locksrc, lockdest;
		Lock( lockdest );

		from.LockReadOnly( locksrc );

		for ( size_t i = 0; i < locksrc.Count(); ++i )
		{
			size_t to = offset + i;
			const void * rawData = locksrc.GetItemReadOnly( i );
			if ( locksrc.Stride() == 2 )
			{
				unsigned short fromIndex = *static_cast< const short int * >(rawData);
				lockdest.SetIndex( to, fromIndex + vertexOffset );
			}
			else
			{
				unsigned int fromIndex = *static_cast< const unsigned int * >(rawData);
				lockdest.SetIndex( to, fromIndex + vertexOffset );
			}
		}

		from.UnlockReadOnly();
		Unlock();

		return offset;
	}

	void Destroy()
	{
		m_IB = nullptr;
		m_owner.m_length = 0;
	}

	void Lock( IndexLock & lock )
	{
		HRESULT hr;
		unsigned char * data;
		hr = m_IB->Lock( 0, 0, (void**)&data, 0 );
		if ( FAILED( hr ) )
		{
			lock.Invalidate();
			throw exception::FailedToLock( "Failed to lock index buffer!" );
		}

		lock.SetLock( data, m_owner.GetStride(), m_owner.GetLength(), false );
		m_owner.m_locked = true;
	}

	void LockReadOnly( IndexLock & lock ) const
	{
		assert( m_IB != 0 );
		if ( m_owner.m_locked )
		{
			throw exception::FailedToLock( "Index buffer already locked!" );
		}

		void * data = 0;
		if ( FAILED( m_IB->Lock( 0, 0, &data, D3DLOCK_READONLY ) ) )
		{
			throw exception::FailedToLock( "Failed to lock indices!" );
		}
		lock.SetLock( data, m_owner.GetStride(), m_owner.GetLength(), true );
		bool & locked = *const_cast<bool*>(&m_owner.m_locked); // Break const for locking.
		locked = false;
	}

	void Unlock()
	{
		if ( m_owner.m_locked == false )
		{
			return;
		}
		m_IB->Unlock();
		m_owner.m_locked = false;
	}

	void UnlockReadOnly() const
	{
		if ( m_owner.m_locked == false )
		{
			return;
		}
		m_IB->Unlock();
		bool & locked = *const_cast<bool*>(&m_owner.m_locked); // Break const for locking.
		locked = false;
	}

	bool Valid() const
	{
		return m_IB != 0;
	}

	void Use() const
	{
		if ( !m_IB )
		{
			return;
		}

		auto dxDevice = m_renderer->GetDxDevice();

		HRESULT hr = dxDevice->SetIndices( m_IB );
		if ( FAILED( hr ) )
		{
			throw unify::Exception( "Failed to use index buffer!" );
		}
	}
};























#elif defined(DIRECTX11)

class IndexBuffer::Pimpl
{
public:
	IndexBuffer & m_owner;
	win::DXRenderer * m_renderer;
	unsigned int m_createFlags;

	CComPtr< ID3D11Buffer > m_IB;

	Pimpl( IndexBuffer & owner, core::IRenderer * renderer )
		: m_owner( owner )
		, m_renderer( dynamic_cast< win::DXRenderer * >(renderer) )
	{
	}

	Pimpl::~Pimpl()
	{
		Destroy();
	}

	void Create( unsigned int uNumIndices, Index32 * source, BufferUsage::TYPE usage, unify::Flags flags )
	{
		Destroy();

		HRESULT hr = S_OK;

		m_owner.m_length = uNumIndices;

		// Fill in a buffer description.
		D3D11_BUFFER_DESC bufferDesc;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof( unsigned int ) * uNumIndices;
		bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bufferDesc.CPUAccessFlags = 0;
		bufferDesc.MiscFlags = 0;
		D3D11_SUBRESOURCE_DATA initialData = D3D11_SUBRESOURCE_DATA();
		initialData.pSysMem = source;

		// Create the buffer with the device.
		hr = dxDevice->CreateBuffer( &bufferDesc, &initialData, &m_IB );
		if ( FAILED( hr ) )
		{
			throw exception::FailedToCreate( "Failed to create index buffer!" );
		}
	}

	void Resize( unsigned int numIndices )
	{
		if ( m_owner.GetUsage() != BufferUsage::Staging )
		{
			throw unify::Exception( "IndexBuffer's usage is not Staging! Cannot resize without CPU read and write access." );
		}

		/*
		TODO:

		unsigned int oldNumberOfIndices = GetLength();
		size_t oldSize = GetSize();
		ID3D11Buffer * oldIndexBuffer = m_IB;
		m_IB = 0;
		m_length = 0;

		Create( numIndices, this->GetFormat(), GetUsage(), m_createFlags );

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
		oldIndexBuffer = 0;
		*/
	}

	size_t Append( const IndexBuffer & from, size_t vertexOffset )
	{
		size_t offset = m_owner.GetLength();

		if ( from.GetLength() == 0 )
		{
			return offset;
		}

		if ( from.GetUsage() != BufferUsage::Staging )
		{
			throw unify::Exception( "From IndexBuffer's usage is not Staging!" );
		}

		if ( m_owner.GetLength() == 0 )
		{
			assert( 0 ); // TODO:
			Create( from.GetLength(), nullptr, from.GetUsage(), from.m_pimpl->m_createFlags );
		}
		else
		{
			if ( m_owner.GetUsage() != BufferUsage::Staging )
			{
				throw unify::Exception( "IndexBuffer's usage is not Staging!" );
			}
			Resize( m_owner.GetLength() + from.GetLength() );
		}

		// Copy vertices...
		IndexLock locksrc, lockdest;
		Lock( lockdest );

		from.LockReadOnly( locksrc );

		for ( size_t i = 0; i < locksrc.Count(); ++i )
		{
			size_t to = offset + i;
			const void * rawData = locksrc.GetItemReadOnly( i );
			if ( locksrc.Stride() == 2 )
			{
				unsigned short fromIndex = *static_cast< const short int * >(rawData);
				lockdest.SetIndex( to, fromIndex + vertexOffset );
			}
			else
			{
				unsigned int fromIndex = *static_cast< const unsigned int * >(rawData);
				lockdest.SetIndex( to, fromIndex + vertexOffset );
			}
		}

		from.UnlockReadOnly();
		Unlock();

		return offset;
	}

	void Destroy()
	{
		m_IB = nullptr;
		m_owner.m_length = 0;
	}

	void Lock( IndexLock & lock )
	{
		throw exception::NotImplemented( "Locking not supported in DX11!" );
	}

	void LockReadOnly( IndexLock & lock ) const
	{
		throw exception::NotImplemented( "Locking not supported in DX11!" );
	}

	void Unlock()
	{
		throw exception::NotImplemented( "Locking not supported in DX11!" );
	}

	void UnlockReadOnly() const
	{
		throw exception::NotImplemented( "Locking not supported in DX11!" );
	}

	bool Valid() const
	{
		return m_IB != 0;
	}

	void Use() const
	{
		if ( !m_IB )
		{
			return;
		}

		// Set the buffer.
		m_dxContext->IASetIndexBuffer( m_IB, DXGI_FORMAT_R32_UINT, 0 );
	}
};

#endif



IndexBuffer::IndexBuffer( core::IRenderer * renderer )
	: m_pimpl( new Pimpl( *this, renderer ) )
{
}

IndexBuffer::~IndexBuffer()
{
	m_pimpl->Destroy();
}

void IndexBuffer::Create( unsigned int uNumIndices, Index32 * source, BufferUsage::TYPE usage, unify::Flags flags )
{
	m_pimpl->Create( uNumIndices, source, usage, flags );
}

void IndexBuffer::Resize( unsigned int numIndices )
{
	m_pimpl->Resize( numIndices );
}

size_t IndexBuffer::Append( const IndexBuffer & from, size_t vertexOffset  )
{
	return m_pimpl->Append( from, vertexOffset );
}

void IndexBuffer::Destroy()
{
	m_pimpl->Destroy();
}

void IndexBuffer::Lock( IndexLock & lock )
{
	m_pimpl->Lock( lock );
}

void IndexBuffer::LockReadOnly( IndexLock & lock ) const
{
	m_pimpl->LockReadOnly( lock );
}

void IndexBuffer::Unlock()
{
	m_pimpl->Unlock();
}

void IndexBuffer::UnlockReadOnly() const
{
	m_pimpl->UnlockReadOnly();
}

bool IndexBuffer::Valid() const
{
	return m_pimpl->Valid();
}

void IndexBuffer::Use() const
{
	m_pimpl->Use();
}