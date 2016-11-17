// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <medx9/VertexShader.h>
#include <medx9/Renderer.h>
#include <me/exception/NotImplemented.h>
#include <atlbase.h>

using namespace medx9;
using namespace me;

class VertexShader::Pimpl
{
	VertexShader & m_owner;
	const Renderer * m_renderer;

	CComPtr< ID3DXBuffer > m_codeBuffer;
	CComPtr< ID3DXConstantTable > m_constantTable;
	CComPtr< IDirect3DVertexShader9 > m_shader;
	
	D3DXHANDLE m_worldMatrixHandle;
	D3DXHANDLE m_viewMatrixHandle;
	D3DXHANDLE m_projectionMatrixHandle;
	D3DXHANDLE m_finalMatrixHandle;

public:
	Pimpl( VertexShader & owner, const me::IRenderer * renderer )
		: m_owner( owner )
		, m_renderer( dynamic_cast< const Renderer * >( renderer ) )
		, m_finalMatrixHandle( 0 )
		, m_worldMatrixHandle( 0 )
	    , m_viewMatrixHandle( 0 )
	    , m_projectionMatrixHandle( 0 )
	{
	}

	~Pimpl()
	{
		Destroy();
	}

	void Destroy()
	{
		m_codeBuffer = nullptr;
		m_constantTable = nullptr;
		m_shader = nullptr;
	}

	void Create()
	{
		bool debug = false;
#if defined( DEBUG ) || defined( _DEBUG )
		debug = true;
#endif

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
		CreateThisShader();
	}

	void CreateThisShader()
	{
		m_renderer->GetDxDevice()->CreateVertexShader( (unsigned long *)m_codeBuffer->GetBufferPointer(), &m_shader );
		m_finalMatrixHandle = m_constantTable->GetConstantByName( 0, "finalMatrix" );
		m_worldMatrixHandle = m_constantTable->GetConstantByName( 0, "worldMatrix" );
		m_viewMatrixHandle = m_constantTable->GetConstantByName( 0, "viewMatrix" );
		m_projectionMatrixHandle = m_constantTable->GetConstantByName( 0, "projectionMatrix" );
		m_owner.m_vertexDeclaration->Build( m_renderer, m_owner );
		m_owner.m_created = true;
	}

	void Use( const RenderInfo & renderInfo, RenderInstance instance )
	{
		HRESULT result = S_OK;

		unify::Matrix world = instance.m;
		unify::Matrix view = renderInfo.GetViewMatrix();
		unify::Matrix projection = renderInfo.GetProjectionMatrix();

		if( m_worldMatrixHandle != 0 )
		{
			result = GetConstantTable()->SetMatrix( m_renderer->GetDxDevice(), m_worldMatrixHandle, (D3DXMATRIX*)&world.m );
		}
		if( m_viewMatrixHandle != 0 )
		{
			result = GetConstantTable()->SetMatrix( m_renderer->GetDxDevice(), m_viewMatrixHandle, (D3DXMATRIX*)&view.m );
		}
		if( m_projectionMatrixHandle != 0 )
		{
			result = GetConstantTable()->SetMatrix( m_renderer->GetDxDevice(), m_projectionMatrixHandle, (D3DXMATRIX*)&projection.m );
		}
		if( m_finalMatrixHandle != 0 )
		{
			unify::Matrix final = world * view * projection;
			result = GetConstantTable()->SetMatrix( m_renderer->GetDxDevice(), m_finalMatrixHandle, (D3DXMATRIX*)&final.m );
		}

		result = m_renderer->GetDxDevice()->SetVertexShader( m_shader );
		if( FAILED( result ) )
		{
			throw unify::Exception( "Failed to set vertex shader!" );
		}
	}

	ID3DXConstantTable * GetConstantTable()
	{
		return m_constantTable;
	}

	const void * GetBytecode() const
	{
		return nullptr;
	}

	size_t GetBytecodeLength() const
	{
		return 0;
	}
};


VertexShader::VertexShader( const me::IRenderer * renderer )
	: m_pimpl( new Pimpl( *this, renderer ) )
	, m_assembly( false )
	, m_created( false )
{
}

VertexShader::VertexShader( const me::IRenderer * renderer, VertexShaderParameters parameters )
	: m_pimpl( new Pimpl( *this, renderer ) )
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
	m_pimpl->Destroy();
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

	m_pimpl->Create();
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

void VertexShader::Use( const RenderInfo & renderInfo, const RenderInstance & instance )
{
	m_vertexDeclaration->Use();
	m_pimpl->Use( renderInfo, instance );
}

std::string VertexShader::GetError()
{
	return m_errorMessage;
}