// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <medx9/IndexBuffer.h>
#include <medx9/Renderer.h>
#include <me/exception/FailedToCreate.h>
#include <me/exception/FailedToLock.h>
#include <me/exception/OutOfBounds.h>
#include <me/exception/NotImplemented.h>
#include <atlbase.h>

using namespace medx9;
using namespace me;

class IndexBuffer::Pimpl
{
public:
	IndexBuffer & m_owner;
	Renderer * m_renderer;
	unsigned int m_createFlags;

	CComPtr< IDirect3DIndexBuffer9 > m_IB;

	Pimpl( IndexBuffer & owner, me::IRenderer * renderer )
		: m_owner( owner )
		, m_renderer( dynamic_cast< Renderer * >(renderer) )
	{
	}
	
	Pimpl::~Pimpl()
	{
		Destroy();
	}

	void Create( IndexBufferParameters parameters )
	{
		Destroy();

		HRESULT hr = S_OK;

		m_owner.m_length = parameters.numIndices;

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
		m_owner.m_stride = sizeof( Index32 );

		m_createFlags = createFlags;
		m_owner.m_usage = parameters.usage;

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

		if ( parameters.source )
		{
			// Create the index list...
			IndexLock indexLock;
			Lock( indexLock );
			memcpy( indexLock.GetData(), parameters.source, m_owner.m_length * sizeof( Index32 ) );
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
			Create( IndexBufferParameters{ from.GetLength(), nullptr, from.GetUsage(), from.m_pimpl->m_createFlags } );
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

		lock.SetLock( data, m_owner.GetStride(), m_owner.GetLength(), false, 0 );
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
		lock.SetLock( data, m_owner.GetStride(), m_owner.GetLength(), true, 0 );
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


IndexBuffer::IndexBuffer( me::IRenderer * renderer )
	: m_pimpl( new Pimpl( *this, renderer ) )
	, m_locked( false )
	, m_usage( BufferUsage::Default )
	, m_length( 0 )
	, m_stride( 0 ){
}

IndexBuffer::IndexBuffer( me::IRenderer * renderer, IndexBufferParameters parameters )
	: m_pimpl( new Pimpl( *this, renderer ) )
	, m_locked( false )
	, m_usage( BufferUsage::Default )
	, m_length( 0 )
	, m_stride( 0 )
{
	Create( parameters );
}


IndexBuffer::~IndexBuffer()
{
	m_pimpl->Destroy();
}

void IndexBuffer::Create( IndexBufferParameters parameters )
{
	m_pimpl->Create( parameters );
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

unsigned int IndexBuffer::GetSize() const
{
	return m_stride * m_length;
}
