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
	, m_slot( 0 )
	, m_locked( false )
	, m_usage( BufferUsage::Default )
	, m_length( 0 )
	, m_stride( 0 )
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

	m_vertexDeclaration = parameters.vertexDeclaration;
	m_slot = 0;
	m_stride = m_vertexDeclaration->GetSize( 0 );
	m_length = parameters.countAndSource[0].count;
	m_usage = parameters.usage;
	m_bbox = parameters.bbox;

	// Ensure we have some sort of idea what we need to be...
	if( GetSize() == 0 )
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
	vertexBufferDesc.ByteWidth = parameters.vertexDeclaration->GetSize( 0 ) * parameters.countAndSource[0].count;
	vertexBufferDesc.Usage = usageDX;

	HRESULT result;
	if ( parameters.countAndSource[0].source != nullptr )
	{
		D3D11_SUBRESOURCE_DATA initialData{};
		initialData.pSysMem = parameters.countAndSource[0].source;
		if ( parameters.usage == BufferUsage::Dynamic )
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

void VertexBuffer::Destroy()
{
	m_VB = nullptr;
	m_length = 0;
	m_stride = 0;
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
	auto dxContext = m_renderer->GetDxContext();
	dxContext->IASetVertexBuffers( streamNumber, 1, &m_VB.p, &stride, &offsetInBytes );
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
