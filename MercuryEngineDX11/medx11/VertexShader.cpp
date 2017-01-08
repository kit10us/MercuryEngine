// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <medx11/VertexShader.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/FailedToLock.h>
#include <me/exception/Exception.h>

using namespace medx11;
using namespace me;
using namespace shader;

VertexShader::VertexShader( const me::IRenderer * renderer )
	: m_renderer( dynamic_cast< const Renderer * >(renderer) )
	, m_assembly( false )
	, m_created( false )
	, m_lockCount{ 0 }
{
}

VertexShader::VertexShader( const me::IRenderer * renderer, VertexShaderParameters parameters )
	: VertexShader( renderer )
{
	Create( parameters );
}

VertexShader::~VertexShader()
{
	Destroy();
}

void VertexShader::Destroy()
{
	m_vertexShader = nullptr;
	m_vertexShaderBuffer = nullptr;
}

void VertexShader::Create( VertexShaderParameters parameters )
{
	Destroy();

	m_constants = parameters.constants;
	m_vertexDeclaration = parameters.vertexDeclaration;

	if( parameters.path.Empty() == false )
	{
		m_filePath = parameters.path;
	}
	else
	{
		m_code = parameters.code;
	}

	m_entryPointName = parameters.entryPointName;
	m_profile = parameters.profile;

	bool debug = false;
#if defined( DEBUG ) || defined( _DEBUG )
	debug = true;
#endif

	HRESULT result = S_OK;
	CComPtr< ID3D10Blob > errorBlob; // Generic buffer for error data.
	D3D_SHADER_MACRO * shaderMacros = 0;
	unsigned int flags1 = D3DCOMPILE_ENABLE_STRICTNESS;
	if ( debug )
	{
		flags1 |= D3DCOMPILE_DEBUG;
	}
	unsigned int flags2 = 0; // Only used for effect compilation.
	result = D3DCompileFromFile( unify::Cast< std::wstring >( m_filePath.ToString() ).c_str(), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, m_entryPointName.c_str(), m_profile.c_str(), flags1, flags2, &m_vertexShaderBuffer, &errorBlob );
	if ( FAILED( result ) )
	{
		OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
		assert( !FAILED( result ) );
	}

	auto dxDevice = m_renderer->GetDxDevice();
	ID3D11ClassLinkage * classLinkage = nullptr;
	result = dxDevice->CreateVertexShader( m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), classLinkage, &m_vertexShader );
	assert( !FAILED( result ) );

	/*
	result = D3DReflect( m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&m_vertexShaderReflection );
	assert( !FAILED( result ) );
	*/

	using namespace DirectX;

	size_t index = 0;
	for ( auto && buffer : m_constants->GetBuffers() )
	{
		D3D11_BUFFER_DESC constantBufferDesc{};
		constantBufferDesc.ByteWidth = buffer->GetSizeInBytes();
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		CComPtr< ID3D11Buffer > createdBuffer;
		result = dxDevice->CreateBuffer( &constantBufferDesc, nullptr, &createdBuffer );
		m_vertexShaderConstantBuffers.push_back( createdBuffer );
		m_locked.push_back( false );
		assert( !FAILED( result ) );
	}
		
	m_vertexDeclaration->Build( m_renderer, *this );
	m_created = true;
}

const ShaderConstants * VertexShader::GetConstants() const
{
	return m_constants.get();
}

void VertexShader::LockConstants( size_t buffer, unify::DataLock & lock )
{
	if ( m_locked[buffer] ) throw exception::FailedToLock( "Failed to lock vertex shader constant buffer!" );

	auto dxDevice = m_renderer->GetDxDevice();
	auto dxContext = m_renderer->GetDxContext();

	D3D11_MAPPED_SUBRESOURCE subresource{};
	HRESULT result = dxContext->Map( m_vertexShaderConstantBuffers[ buffer ], 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subresource );
	if ( FAILED( result ) )
	{
		throw unify::Exception( "Failed to set vertex shader!" );
	}		

	lock.SetLock( subresource.pData, m_constants->GetBuffers()[ buffer ]->GetSizeInBytes(), false, 0 );
	m_locked[ buffer ] = true;
	m_lockCount++;
}

void VertexShader::UnlockConstants( size_t buffer, unify::DataLock & lock )
{
	if ( ! m_locked[buffer] ) throw exception::FailedToLock( "Failed to unlock vertex shader constant buffer (buffer not locked)!" );

	auto dxDevice = m_renderer->GetDxDevice();
	auto dxContext = m_renderer->GetDxContext();

	dxContext->Unmap( m_vertexShaderConstantBuffers[ buffer ].p, buffer );
	dxContext->VSSetConstantBuffers( buffer, 1, &m_vertexShaderConstantBuffers[ buffer ].p );
	m_locked[buffer] = false;
	m_lockCount--;
}

void VertexShader::SetVertexDeclaration( VertexDeclaration::ptr vertexDeclaration )
{
	m_vertexDeclaration = vertexDeclaration;
}

VertexDeclaration::ptr VertexShader::GetVertexDeclaration() const
{
	return m_vertexDeclaration;
}

const void * VertexShader::GetBytecode() const
{
	return m_vertexShaderBuffer->GetBufferPointer();
}

size_t VertexShader::GetBytecodeLength() const
{
	return m_vertexShaderBuffer->GetBufferSize();
}

void VertexShader::Use()
{
	if ( m_lockCount != 0 ) throw unify::Exception( "Vertex shader is still locked, while attempting to use it!" );
	
	m_vertexDeclaration->Use();

	auto dxDevice = m_renderer->GetDxDevice();
	auto dxContext = m_renderer->GetDxContext();

	dxContext->VSSetShader( m_vertexShader, nullptr, 0 );
}

std::string VertexShader::GetError()
{
	return m_errorMessage;
}