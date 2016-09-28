// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/VertexShader.h>
#include <dxi/win/DXDevice.h>
#include <dxi/exception/NotImplemented.h>

using namespace dxi;

class VertexShader::Pimpl
{
public:
	Pimpl( VertexShader & owner)
		: m_owner( owner )
		, m_finalMatrixHandle( 0 )
	{
	}						   

	~Pimpl()
	{
		m_codeBuffer = nullptr;
		m_constantTable = nullptr;
		m_shader = nullptr;
	}

	void Destroy()
	{
		m_codeBuffer = nullptr;
		m_constantTable = nullptr;
		m_shader = nullptr;
	}

	void Create()
	{
		HRESULT result = S_OK;
		ID3DXBuffer * errorBuffer = 0;
		if( ! m_owner.m_filePath.Empty() )
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

			CreateThisShader();
			m_owner.m_created = true;
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

			CreateThisShader();
			m_owner.m_created = true;
		}
		else
		{
			throw unify::Exception( "Attempted to create shader from unknown source!" );
		}
	}

	void CreateThisShader()
	{
		win::DX::GetDxDevice()->CreateVertexShader( (unsigned long *)m_codeBuffer->GetBufferPointer(), &m_shader );
		m_finalMatrixHandle = m_constantTable->GetConstantByName( 0, "finalMatrix" );
	}

	void Use( const RenderInfo & renderInfo )
	{
		HRESULT result = S_OK;

		if( m_finalMatrixHandle != 0 )
		{
			unify::Matrix final = renderInfo.GetFinalMatrix();
			result = GetConstantTable()->SetMatrix( win::DX::GetDxDevice(), m_finalMatrixHandle, final.GetD3DXMatrix() );
		}

		result = win::DX::GetDxDevice()->SetVertexShader( m_shader );
		if( FAILED( result ) )
		{
			throw unify::Exception( "Failed to set vertex shader!" );
		}
	}

	ID3DXConstantTable * GetConstantTable()
	{
		return m_constantTable;
	}


	VertexShader & m_owner;

	CComPtr< ID3DXBuffer > m_codeBuffer;
	CComPtr< ID3DXConstantTable > m_constantTable;

	CComPtr< IDirect3DVertexShader9 > m_shader;
	D3DXHANDLE m_finalMatrixHandle; // Combination Model * View * Projection matrix location in shader.				
};		

void VertexShader::DisuseShader()
{
	win::DX::GetDxDevice()->SetVertexShader( 0 );
}

VertexShader::VertexShader()
: m_pimpl( new Pimpl( *this ) )
, m_assembly( false )
, m_created( false )
{
}

VertexShader::VertexShader( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile, VertexDeclaration vertexDeclaration )
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

void VertexShader::SetVertexDeclaration( VertexDeclaration vertexDeclaration )
{
	m_vertexDeclaration = vertexDeclaration;
}

VertexDeclaration VertexShader::GetVertexDeclaration() const
{
	return m_vertexDeclaration;
}

void VertexShader::Use( const RenderInfo & renderInfo )
{
	m_vertexDeclaration.Use();
	m_pimpl->Use( renderInfo );
}
 
std::string VertexShader::GetError()
{
	return m_errorMessage;
}

ID3DXConstantTable * VertexShader::GetConstantTable()
{
	return m_pimpl->GetConstantTable();
}
