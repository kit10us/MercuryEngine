// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/PixelShader.h>
#include <dxi/win/DXRenderer.h>
#include <dxi/exception/NotImplemented.h>
#include <atlbase.h>

using namespace dxi;

#if defined( DIRECTX9 )

class PixelShader::Pimpl
{
	PixelShader & m_owner;
	win::DXRenderer * m_renderer;
	CComPtr< ID3DXBuffer > m_codeBuffer;
	CComPtr< ID3DXConstantTable > m_constantTable;
	CComPtr< IDirect3DPixelShader9 > m_shader;

public:
	Pimpl( PixelShader & owner, core::IRenderer * renderer )
		: m_owner( owner )
		, m_renderer( dynamic_cast< win::DXRenderer * >( renderer ) )
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
		bool debug =
#if defined( DEBUG ) || defined( _DEBUG )
			true;
#else
			false;
#endif

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
		}
		else
		{
			throw unify::Exception( "Attempted to create shader from unknown source!" );
		}
		CreateThisShader();
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
		m_renderer->GetDxDevice()->CreatePixelShader( (unsigned long *)m_codeBuffer->GetBufferPointer(), &m_shader );
	}

	void Use( const RenderInfo & renderInfo )
	{
		HRESULT result = m_renderer->GetDxDevice()->SetPixelShader( m_shader );
		if( FAILED( result ) )
		{
			throw unify::Exception( "Failed to set pixel shader!" );
		}
	}

	ID3DXConstantTable * GetConstantTable()
	{
		return m_constantTable;
	}
};


#elif defined( DIRECTX11 )


class PixelShader::Pimpl
{
public:
	PixelShader & m_owner;
	win::DXRenderer * m_renderer;
	CComPtr< ID3D11PixelShader > m_pixelShader;
	CComPtr< ID3D10Blob > m_pixelShaderBuffer;

	Pimpl( PixelShader & owner, core::IRenderer * renderer )
		: m_owner( owner )
		, m_renderer( dynamic_cast< win::DXRenderer * >(renderer) )
	{
	}

	~Pimpl()
	{
		Destroy();
	}

	void Destroy()
	{
		m_pixelShader = nullptr;
		m_pixelShaderBuffer = nullptr;
	}

	void Create()
	{
		bool debug =
#if defined( DEBUG ) || defined( _DEBUG )
			true;
#else
			false;
#endif

		HRESULT result;
		CComPtr< ID3D10Blob > errorBlob; // Generic buffer for error data.
		D3D_SHADER_MACRO * shaderMacros = 0;
		unsigned int flags1 = D3DCOMPILE_ENABLE_STRICTNESS;
		if ( debug )
		{
			flags1 |= D3DCOMPILE_DEBUG;
		}

		unsigned int flags2 = 0; // Only used for effect compilation.
		result = D3DCompileFromFile( unify::Cast< std::wstring >( m_owner.m_filePath.ToString() ).c_str(), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, m_owner.m_entryPointName.c_str(), m_owner.m_profile.c_str(), flags1, flags2, &m_pixelShaderBuffer, &errorBlob );
		if ( FAILED( result ) )
		{
			OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
			assert( !FAILED( result ) );
		}
		CreateThisShader();
	}

	void CreateThisShader()
	{
		HRESULT result;
		ID3D11ClassLinkage * classLinkage = nullptr;
		result = m_renderer->GetDxDevice()->CreatePixelShader( m_pixelShaderBuffer->GetBufferPointer(), m_pixelShaderBuffer->GetBufferSize(), classLinkage, &m_pixelShader );
		assert( !FAILED( result ) );
	}

	void Use( const RenderInfo & renderInfo )
	{
		win::DX::GetDxContext()->PSSetShader( m_pixelShader, nullptr, 0 );
	}
};


#endif

PixelShader::PixelShader( core::IRenderer * renderer )
	: m_pimpl( new Pimpl( *this, renderer  ) )
	, m_assembly( false )
	, m_created( false )
	, m_isTrans( false )
{
}

PixelShader::PixelShader( core::IRenderer * renderer, const unify::Path & filePath, const std::string & entryPointName, const std::string & profile )
	: PixelShader( renderer )
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

void PixelShader::SetTrans( bool bTrans )
{
	m_isTrans = bTrans;
}

bool PixelShader::IsTrans()
{
	return(this && m_isTrans) ? true : FALSE;
}

std::string PixelShader::GetError()
{
	return m_errorMessage;
}