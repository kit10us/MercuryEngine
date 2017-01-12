// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <medx11/VertexBuffer.h>
#include <me/exception/FailedToCreate.h>
#include <me/exception/FailedToLock.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/Exception.h>

using namespace medx11;
using namespace me;

VertexBuffer::VertexBuffer( const me::IRenderer * renderer )
	: m_renderer( dynamic_cast< const Renderer * >(renderer) )
	, m_usage( BufferUsage::Default )
{
}

VertexBuffer::VertexBuffer( const me::IRenderer * renderer, me::VertexBufferParameters parameters )
	: VertexBuffer( renderer )
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

	m_usage = parameters.usage;
	m_bbox = parameters.bbox;

	size_t bufferIndex = 0;
	for ( auto && countAndSource : parameters.countAndSource )
	{
		m_vertexDeclaration = parameters.vertexDeclaration;
		m_strides.push_back( m_vertexDeclaration->GetSizeInBytes( bufferIndex ) );
		m_lengths.push_back( countAndSource.count );

		// Ensure we have some sort of idea what we need to be...
		if ( GetSizeInBytes() == 0 )
		{
			throw exception::FailedToCreate( "Not a valid vertex buffer size!" );
		}

		auto dxDevice = m_renderer->GetDxDevice();

		// Ensure that if we are BufferUsage::Immutable, then source is not null.
		if ( BufferUsage::Immutable && parameters.countAndSource[0].source == nullptr )
		{
			throw exception::FailedToCreate( "Buffer is immutable, yet source is null!" );
		}

		D3D11_USAGE usageDX{};
		switch ( m_usage )
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
		vertexBufferDesc.ByteWidth = parameters.vertexDeclaration->GetSizeInBytes( 0 ) * parameters.countAndSource[0].count;
		vertexBufferDesc.Usage = usageDX;

		HRESULT result;
		ID3D11Buffer * buffer;
		if ( parameters.countAndSource[0].source != nullptr )
		{
			D3D11_SUBRESOURCE_DATA initialData{};
			initialData.pSysMem = parameters.countAndSource[0].source;
			if ( parameters.usage == BufferUsage::Dynamic )
			{
				vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // TODO: make not hard coded.
			}
			result = dxDevice->CreateBuffer( &vertexBufferDesc, &initialData, &buffer );
		}
		else
		{
			result = dxDevice->CreateBuffer( &vertexBufferDesc, nullptr, &buffer );
		}
		OnFailedThrow( result, "Failed to create vertex buffer!" );
		m_buffers.push_back( buffer );
		bufferIndex++;
	}
}

void VertexBuffer::Destroy()
{
	for ( auto && buffer : m_buffers )
	{
		buffer->Release();
	}
	m_buffers.clear();
	m_lengths.clear();
	m_strides.clear();
}

void VertexBuffer::Lock( unify::DataLock & lock )
{
	throw exception::NotImplemented();
}

void VertexBuffer::LockReadOnly( unify::DataLock & lock ) const
{
	throw exception::NotImplemented();
}

void VertexBuffer::Unlock()
{
	throw exception::NotImplemented();
}

void VertexBuffer::Unlock() const
{
	throw exception::NotImplemented();
}

VertexDeclaration::ptr VertexBuffer::GetVertexDeclaration() const
{
	return m_vertexDeclaration;
}

bool VertexBuffer::Valid() const
{
	return m_buffers.size() == m_strides.size();
}

void VertexBuffer::Use() const
{	
	auto dxContext = m_renderer->GetDxContext();
	std::vector< unsigned int > offsetInBytes( m_strides.size(), 0 );
	dxContext->IASetVertexBuffers( 0, m_buffers.size(), &m_buffers[0], &m_strides[0], &offsetInBytes[0] );
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
	size_t bufferIndex = 0;
	return m_locked[ bufferIndex ];
}

BufferUsage::TYPE VertexBuffer::GetUsage() const
{
	return m_usage;
}

unsigned int VertexBuffer::GetStride() const
{
	size_t bufferIndex = 0;
	return m_strides[ bufferIndex ];
}

unsigned int VertexBuffer::GetLength() const
{
	size_t bufferIndex = 0;
	return m_lengths[ bufferIndex ];
}

size_t VertexBuffer::GetSizeInBytes() const
{
	size_t bufferIndex = 0;
	return m_strides[ bufferIndex ] * m_lengths[ bufferIndex ];
}
