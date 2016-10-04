// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/VertexShader.h>
#include <dxi/win/DXDevice.h>
#include <dxi/exception/NotImplemented.h>

using namespace dxi;

class VertexShader::Pimpl
{
	VertexShader & m_owner;

#if defined( DIRECTX9 )
#elif defined( DIRECTX11 )
	struct ConstantBuffer
	{
		DirectX::XMMATRIX worldMatrix;
		DirectX::XMMATRIX viewMatrix;
		DirectX::XMMATRIX projectionMatrix;
	} m_vertexShaderConstants;
#endif

#if defined( DIRECTX9 )
	CComPtr< ID3DXBuffer > m_codeBuffer;
	CComPtr< ID3DXConstantTable > m_constantTable;
	CComPtr< IDirect3DVertexShader9 > m_shader;
	
	D3DXHANDLE m_worldMatrixHandle;
	D3DXHANDLE m_viewMatrixHandle;
	D3DXHANDLE m_projectionMatrixHandle;
	D3DXHANDLE m_finalMatrixHandle;
#elif defined( DIRECTX11 )
	CComPtr< ID3D11VertexShader > m_vertexShader;
	CComPtr< ID3D10Blob > m_vertexShaderBuffer;
	//CComPtr< ID3D11ShaderReflection > m_vertexShaderReflection;

	// TODO: Need a standard interface for updating constant data that is cross renderer support.
	CComPtr< ID3D11Buffer > m_vertexShaderConstantBuffer;
#endif

public:
	Pimpl( VertexShader & owner )
		: m_owner( owner )
#if defined( DIRECTX9 )
		, m_finalMatrixHandle( 0 )
		, m_worldMatrixHandle( 0 )
	    , m_viewMatrixHandle( 0 )
	    , m_projectionMatrixHandle( 0 )
#elif defined( DIRECTX11 )
#endif
	{
	}

	~Pimpl()
	{
		Destroy();
	}

	void Destroy()
	{
#if defined( DIRECTX9 )
		m_codeBuffer = nullptr;
		m_constantTable = nullptr;
		m_shader = nullptr;
#elif defined( DIRECTX11 )
		m_vertexShader = nullptr;
		m_vertexShaderBuffer = nullptr;
		//		m_vertexShaderReflection = nullptr;
#endif
	}

	void Create()
	{
		bool debug = false;
#if defined( DEBUG ) || defined( _DEBUG )
		debug = true;
#endif

#if defined( DIRECTX9 )
		HRESULT result = S_OK;
		ID3DXBuffer * errorBuffer = 0;
		if( !m_owner.m_filePath.Empty() )
		{
			if( m_owner.m_assembly )
			{
				DWORD flags = D3DXSHADER_DEBUG;
				result = D3DXAssembleShaderFromFileA( m_owner.m_filePath.ToString().c_str(), 0, 0, D3DXSHADER_DEBUG, &m_codeBuffer, &errorBuffer );
			}
			else
			{
				DWORD flags = 0;
				result = D3DXCompileShaderFromFileA( m_owner.m_filePath.ToString().c_str(), 0, 0, m_owner.m_entryPointName.c_str(), m_owner.m_profile.c_str(), flags, &m_codeBuffer, &errorBuffer, &m_constantTable );
			}
			if( m_codeBuffer == 0 || FAILED( result ) )
			{
				m_owner.m_errorMessage = static_cast< char * >(errorBuffer->GetBufferPointer());
				errorBuffer->Release();
				errorBuffer = 0;
				throw unify::Exception( "Failed to create shader \"" + m_owner.m_filePath.ToString() + "\": " + m_owner.m_errorMessage + "\n" );
			}
		}
		else if( m_owner.m_code.empty() )
		{
			if( m_owner.m_assembly )
			{
				DWORD flags = 0;
				D3DXAssembleShader( m_owner.m_code.c_str(), static_cast< unsigned int >(m_owner.m_code.length() - 1), 0, NULL, flags, &m_codeBuffer, &errorBuffer );
			}
			else
			{
				DWORD flags = 0;
				D3DXCompileShader( m_owner.m_code.c_str(), static_cast< unsigned int >(m_owner.m_code.length() - 1), 0, 0, m_owner.m_entryPointName.c_str(), m_owner.m_profile.c_str(), flags, &m_codeBuffer, &errorBuffer, &m_constantTable );
			}

			if( m_codeBuffer == 0 || FAILED( result ) )
			{
				m_owner.m_errorMessage = static_cast< char * >(errorBuffer->GetBufferPointer());
				errorBuffer->Release();
				errorBuffer = 0;
				throw unify::Exception( "Failed to create shader from code: " + m_owner.m_errorMessage + "\n" );
			}
		}
		else
		{
			throw unify::Exception( "Attempted to create shader from unknown source!" );
		}
#elif defined( DIRECTX11 )
		HRESULT result = S_OK;
		CComPtr< ID3D10Blob > errorBlob; // Generic buffer for error data.
		D3D_SHADER_MACRO * shaderMacros = 0;
		unsigned int flags1 = D3DCOMPILE_ENABLE_STRICTNESS;
		if( debug )
		{
			flags1 |= D3DCOMPILE_DEBUG;
		}
		unsigned int flags2 = 0; // Only used for effect compilation.
		result = D3DCompileFromFile( unify::Cast< std::wstring >( m_owner.m_filePath.ToString() ).c_str(), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, m_owner.m_entryPointName.c_str(), m_owner.m_profile.c_str(), flags1, flags2, &m_vertexShaderBuffer, &errorBlob );
		if( FAILED( result ) )
		{
			OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
			assert( !FAILED( result ) );
		}
#endif
		CreateThisShader();
	}

	void CreateThisShader()
	{
#if defined( DIRECTX9 )
		win::DX::GetDxDevice()->CreateVertexShader( (unsigned long *)m_codeBuffer->GetBufferPointer(), &m_shader );
		m_finalMatrixHandle = m_constantTable->GetConstantByName( 0, "finalMatrix" );
		m_worldMatrixHandle = m_constantTable->GetConstantByName( 0, "worldMatrix" );
		m_viewMatrixHandle = m_constantTable->GetConstantByName( 0, "viewMatrix" );
		m_projectionMatrixHandle = m_constantTable->GetConstantByName( 0, "projectionMatrix" );
#elif defined( DIRECTX11 )
		HRESULT result = S_OK;
		auto dxDevice = win::DX::GetDxDevice();
		ID3D11ClassLinkage * classLinkage = nullptr;
		result = dxDevice->CreateVertexShader( m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), classLinkage, &m_vertexShader );
		assert( !FAILED( result ) );

		/*
		result = D3DReflect( m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&m_vertexShaderReflection );
		assert( !FAILED( result ) );
		*/

		using namespace DirectX;

		m_vertexShaderConstants.worldMatrix = XMMatrixIdentity();

		XMVECTOR eye = XMVectorSet( 0.0f, 1.0f, -50.0f, 0.0f );
		XMVECTOR at = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
		XMVECTOR up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
		m_vertexShaderConstants.viewMatrix = XMMatrixLookAtLH( eye, at, up );

		m_vertexShaderConstants.projectionMatrix = XMMatrixPerspectiveFovLH( DirectX::XM_PIDIV4, 800 / 640, 0.01f, 100.0f );


		D3D11_BUFFER_DESC constantBufferDesc = D3D11_BUFFER_DESC();
		constantBufferDesc.ByteWidth = sizeof( m_vertexShaderConstants );
		constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		D3D11_SUBRESOURCE_DATA constantBufferSourceData = D3D11_SUBRESOURCE_DATA();
		constantBufferSourceData.pSysMem = &m_vertexShaderConstants;
		result = dxDevice->CreateBuffer( &constantBufferDesc, &constantBufferSourceData, &m_vertexShaderConstantBuffer );
		assert( !FAILED( result ) );
#endif
		m_owner.m_vertexDeclaration->Build( m_owner );
		m_owner.m_created = true;
	}

	void Use( const RenderInfo & renderInfo )
	{
#if defined( DIRECTX9 )
		HRESULT result = S_OK;

		unify::Matrix world = renderInfo.GetWorldMatrix();
		unify::Matrix view = renderInfo.GetViewMatrix();
		unify::Matrix projection = renderInfo.GetProjectionMatrix();
		unify::Matrix final = world * view * projection;

		if( m_worldMatrixHandle != 0 )
		{
			result = GetConstantTable()->SetMatrix( win::DX::GetDxDevice(), m_worldMatrixHandle, (D3DXMATRIX*)&world.m );
		}
		if( m_viewMatrixHandle != 0 )
		{
			result = GetConstantTable()->SetMatrix( win::DX::GetDxDevice(), m_viewMatrixHandle, (D3DXMATRIX*)&view.m );
		}
		if( m_projectionMatrixHandle != 0 )
		{
			result = GetConstantTable()->SetMatrix( win::DX::GetDxDevice(), m_projectionMatrixHandle, (D3DXMATRIX*)&projection.m );
		}
		if( m_finalMatrixHandle != 0 )
		{
			result = GetConstantTable()->SetMatrix( win::DX::GetDxDevice(), m_finalMatrixHandle, (D3DXMATRIX*)&final.m );
		}

		result = win::DX::GetDxDevice()->SetVertexShader( m_shader );
		if( FAILED( result ) )
		{
			throw unify::Exception( "Failed to set vertex shader!" );
		}
#elif defined( DIRECTX11 )
		auto dxDevice = win::DX::GetDxDevice();
		auto dxContext = win::DX::GetDxContext();

		using namespace DirectX;
		D3D11_MAPPED_SUBRESOURCE subResource = D3D11_MAPPED_SUBRESOURCE();
		HRESULT result = win::DX::GetDxContext()->Map( m_vertexShaderConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &subResource );
		if( FAILED( result ) )
		{
			throw unify::Exception( "Failed to set vertex shader!" );
		}

		memcpy( &m_vertexShaderConstants.worldMatrix.r[0], &renderInfo.GetWorldMatrix().m, sizeof( float ) * 4 * 4 );
		memcpy( &m_vertexShaderConstants.viewMatrix.r[0], &renderInfo.GetViewMatrix().m, sizeof( float ) * 4 * 4 );
		memcpy( &m_vertexShaderConstants.projectionMatrix.r[0], &renderInfo.GetProjectionMatrix().m, sizeof( float ) * 4 * 4 );

		ConstantBuffer * target = (ConstantBuffer*)subResource.pData;
		*target = m_vertexShaderConstants;

		dxContext->Unmap( m_vertexShaderConstantBuffer, 0 );

		dxContext->VSSetConstantBuffers( 0, 1, &m_vertexShaderConstantBuffer.p );

		win::DX::GetDxContext()->VSSetShader( m_vertexShader, nullptr, 0 );

#endif
	}

#if defined( DIRECTX9 )
	ID3DXConstantTable * GetConstantTable()
	{
		return m_constantTable;
	}
#elif defined( DIRECTX11 )
#endif

	const void * GetBytecode() const
	{
#if defined( DIRECTX9 )
		return nullptr;
#elif defined( DIRECTX11 )
		return m_vertexShaderBuffer->GetBufferPointer();
#endif
	}

	size_t GetBytecodeLength() const
	{
#if defined( DIRECTX9 )
		return 0;
#elif defined( DIRECTX11 )
		return m_vertexShaderBuffer->GetBufferSize();
#endif
	}
};

void VertexShader::DisuseShader()
{
#if defined( DIRECTX9 )
	win::DX::GetDxDevice()->SetVertexShader( 0 );
#elif defined( DIRECTX11 )
#endif
}

VertexShader::VertexShader()
	: m_pimpl( new Pimpl( *this ) )
	, m_assembly( false )
	, m_created( false )
{
}

VertexShader::VertexShader( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile, VertexDeclaration::ptr vertexDeclaration )
	: m_pimpl( new Pimpl( *this ) )
	, m_assembly( false )
	, m_created( false )
	, m_vertexDeclaration( vertexDeclaration )
{
	CreateFromFile( filePath, entryPointName, profile );
}

VertexShader::~VertexShader()
{
	Destroy();
}

void VertexShader::Destroy()
{
	m_pimpl->Destroy();
}

void VertexShader::Create()
{
	m_pimpl->Create();
}

void VertexShader::CreateFromFile( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile )
{
	Destroy();

	m_filePath = filePath;
	m_entryPointName = entryPointName;
	m_profile = profile;

	Create();
}

void VertexShader::CreateFromCode( const std::string & code, const std::string & entryPointName, const std::string & profile )
{
	Destroy();

	m_code = code;
	m_entryPointName = entryPointName;
	m_profile = profile;

	Create();
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
	return m_pimpl->GetBytecode();
}

size_t VertexShader::GetBytecodeLength() const
{
	return m_pimpl->GetBytecodeLength();
}

void VertexShader::Use( const RenderInfo & renderInfo )
{
	m_vertexDeclaration->Use();
	m_pimpl->Use( renderInfo );
}

std::string VertexShader::GetError()
{
	return m_errorMessage;
}

#if defined( DIRECTX9 )
ID3DXConstantTable * VertexShader::GetConstantTable()
{
	return m_pimpl->GetConstantTable();
}
#elif defined( DIRECTX11 )
#endif