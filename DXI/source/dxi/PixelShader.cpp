// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/PixelShader.h>
#include <dxi/win/DXDevice.h>
#include <atlbase.h>
#include <dxi/exception/NotImplemented.h>

using namespace dxi;

class PixelShader::Pimpl
{
public:
	Pimpl( PixelShader & owner )
		: m_owner( owner )
	{
	}

	~Pimpl()
	{
		Destroy();
	}

	void Destroy()
	{
		m_shader = nullptr;
		m_codeBuffer = nullptr;
		m_constantTable = nullptr;
	}

	void Create()
	{
		HRESULT result = S_OK;
		CComPtr< ID3DXBuffer > errorBuffer;
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
		/*
		// Find entry handles...
		m_finalMatrix = m_constantTable->GetConstantByName( 0, "finalMatrix" );
		m_colorMultiply = m_constantTable->GetConstantByName( 0, "colorMultiply" );
		m_colorAddition = m_constantTable->GetConstantByName( 0, "colorAddition" );
		m_scalarTime = m_constantTable->GetConstantByName( 0, "scalarTime" );
		m_textureSize = m_constantTable->GetConstantByName( 0, "textureSize" );
		*/
#if defined( DIRECTX9 )
		win::DX::GetDxDevice()->CreatePixelShader( (unsigned long *)m_codeBuffer->GetBufferPointer(), &m_shader );
#elif defined( DIRECTX11 )
		throw exception::NotImplemented();
#endif	 
	}

	void Use( const RenderInfo & renderInfo )
	{
#if	defined( DIRECTX9 )
		HRESULT result = win::DX::GetDxDevice()->SetPixelShader( m_shader );
		if( FAILED( result ) )
		{
			throw unify::Exception( "Failed to set pixel shader!" );
		}
#elif defined( DIRECTX11 )
		throw exception::NotImplemented();
#endif
	}

	ID3DXConstantTable * GetConstantTable()
	{
		return m_constantTable;
	}


	PixelShader & m_owner;

#if defined( DIRECTX9 )
	CComPtr< ID3DXBuffer > m_codeBuffer;
	CComPtr< ID3DXConstantTable > m_constantTable;
	CComPtr< IDirect3DPixelShader9 > m_shader;
#elif defined( DIRECTX11 )
	CComPtr< ID3D11PixelShader > pixelShader;
	CComPtr< ID3D10Blob > pixelShaderBuffer;
#endif											  
};

PixelShader::PixelShader()
	: m_pimpl( new Pimpl( *this ) )
	, m_assembly( false )
	, m_created( false )
	, m_isTrans( false )
{
}

PixelShader::PixelShader( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile )
	: PixelShader()
{
	CreateFromFile( filePath, entryPointName, profile );
}

PixelShader::~PixelShader()
{
	Destroy();
}

void PixelShader::Destroy()
{
	m_pimpl->Destroy();
}

void PixelShader::CreateFromFile( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile )
{
	Destroy();

	m_filePath = filePath;
	m_entryPointName = entryPointName;
	m_profile = profile;

	Create();
}

void PixelShader::CreateFromCode( const std::string & code, const std::string & entryPointName, const std::string & profile )
{
	Destroy();

	m_code = code;
	m_entryPointName = entryPointName;
	m_profile = profile;

	Create();
}

void PixelShader::Create()
{
	m_pimpl->Create();
}

void PixelShader::Use( const RenderInfo & renderInfo )
{
	m_pimpl->Use( renderInfo );
}

void PixelShader::DisuseShader()
{
#if defined( DIRECTX9 )
	HRESULT result = win::DX::GetDxDevice()->SetPixelShader( 0 );
	if( FAILED( result ) )
	{
		throw unify::Exception( "Failed to set null pixel shader!" );
	}
#elif defined( DIRECTX11 )
	throw exception::NotImplemented();
#endif
}

void PixelShader::SetTrans( bool bTrans )
{
	m_isTrans = bTrans;
}

bool PixelShader::IsTrans()
{
	return(this && m_isTrans) ? true : FALSE;
}

std::string PixelShader ::GetError()
{
	return m_errorMessage;
}

ID3DXConstantTable * PixelShader::GetConstantTable()
{
	return m_pimpl->GetConstantTable();
}

