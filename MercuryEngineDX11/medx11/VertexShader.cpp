// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <medx11/VertexShader.h>
#include <me/exception/NotImplemented.h>

using namespace medx11;
using namespace me;

VertexShader::VertexShader( const me::IRenderer * renderer )
	: m_renderer( dynamic_cast< const Renderer * >(renderer) )
	, m_assembly( false )
	, m_created( false )
{
}

VertexShader::VertexShader( const me::IRenderer * renderer, VertexShaderParameters parameters )
	: m_renderer( dynamic_cast< const Renderer * >(renderer) )
	, m_assembly( false )
	, m_created( false )
	, m_vertexDeclaration( parameters.vertexDeclaration )
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

	m_vertexShaderConstants.worldMatrix = unify::MatrixIdentity();

	unify::V3< float > eye( 0.0f, 1.0f, -50.0f );
	unify::V3< float > at( 0.0f, 0.0f, 0.0f );
	unify::V3< float > up( 0.0f, 1.0f, 0.0f );
	m_vertexShaderConstants.viewMatrix = unify::MatrixLookAtLH( eye, at, up );

	m_vertexShaderConstants.projectionMatrix = unify::MatrixPerspectiveFovLH( 4.141592653589f * 0.25f, 800 / 640, 0.01f, 100.0f );


	D3D11_BUFFER_DESC constantBufferDesc = D3D11_BUFFER_DESC();
	constantBufferDesc.ByteWidth = sizeof( m_vertexShaderConstants );
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	D3D11_SUBRESOURCE_DATA constantBufferSourceData = D3D11_SUBRESOURCE_DATA();
	constantBufferSourceData.pSysMem = &m_vertexShaderConstants;
	result = dxDevice->CreateBuffer( &constantBufferDesc, &constantBufferSourceData, &m_vertexShaderConstantBuffer );
	assert( !FAILED( result ) );
		
	m_vertexDeclaration->Build( m_renderer, *this );
	m_created = true;
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

void VertexShader::Use( const RenderInfo & renderInfo, const unify::Matrix & world )
{
	m_vertexDeclaration->Use();

	auto dxDevice = m_renderer->GetDxDevice();
	auto dxContext = m_renderer->GetDxContext();

	using namespace DirectX;
	D3D11_MAPPED_SUBRESOURCE subResource = D3D11_MAPPED_SUBRESOURCE();
	HRESULT result = dxContext->Map( m_vertexShaderConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subResource );
	if ( FAILED( result ) )
	{
		throw unify::Exception( "Failed to set vertex shader!" );
	}

	m_vertexShaderConstants.worldMatrix = world;
	m_vertexShaderConstants.viewMatrix = renderInfo.GetViewMatrix();
	m_vertexShaderConstants.projectionMatrix = renderInfo.GetProjectionMatrix();

	ConstantBuffer * target = (ConstantBuffer*)subResource.pData;
	*target = m_vertexShaderConstants;

	dxContext->Unmap( m_vertexShaderConstantBuffer, 0 );

	dxContext->VSSetConstantBuffers( 0, 1, &m_vertexShaderConstantBuffer.p );

	dxContext->VSSetShader( m_vertexShader, nullptr, 0 );
}

std::string VertexShader::GetError()
{
	return m_errorMessage;
}