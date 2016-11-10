// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <medx11/PixelShader.h>
#include <medx11/Renderer.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/FailedToCreate.h>
#include <atlbase.h>

using namespace medx11;
using namespace me;

class PixelShader::Pimpl
{
public:
	PixelShader & m_owner;
	Renderer * m_renderer;
	CComPtr< ID3D11PixelShader > m_pixelShader;
	CComPtr< ID3D10Blob > m_pixelShaderBuffer;

	Pimpl( PixelShader & owner, me::IRenderer * renderer )
		: m_owner( owner )
		, m_renderer( dynamic_cast< Renderer * >(renderer) )
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

		if ( ! m_owner.m_parameters.code.empty() )
		{
			result = D3DCompile( m_owner.m_parameters.code.c_str(), m_owner.m_parameters.code.length(), nullptr, nullptr, nullptr, m_owner.m_parameters.entryPointName.c_str(), m_owner.m_parameters.profile.c_str(), flags1, flags2, &m_pixelShaderBuffer, &errorBlob );
		}
		else if ( ! m_owner.m_parameters.path.Empty() )
		{
			result = D3DCompileFromFile( unify::Cast< std::wstring >( m_owner.m_parameters.path.ToString() ).c_str(), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, m_owner.m_parameters.entryPointName.c_str(), m_owner.m_parameters.profile.c_str(), flags1, flags2, &m_pixelShaderBuffer, &errorBlob );
		}
		else
		{
			throw exception::FailedToCreate( "Failed to create shader, neither code nor file path specified!" );
		}

		if ( FAILED( result ) )
		{
			OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
			throw exception::FailedToCreate( std::string( "Failed to create shader \"" ) + m_owner.m_parameters.path.ToString() + "\": " +  std::string( (char*)errorBlob->GetBufferPointer() ) );
		}
		CreateThisShader();
	}

	void CreateThisShader()
	{
		HRESULT result;
		ID3D11ClassLinkage * classLinkage = nullptr;
		result = m_renderer->GetDxDevice()->CreatePixelShader( m_pixelShaderBuffer->GetBufferPointer(), m_pixelShaderBuffer->GetBufferSize(), classLinkage, &m_pixelShader );
		if ( FAILED( result ) )
		{
			throw exception::FailedToCreate( "Failed to create shader!" );
		}
	}

	void Use( const RenderInfo & renderInfo ) const
	{
		m_renderer->GetDxContext()->PSSetShader( m_pixelShader, nullptr, 0 );
	}
};


PixelShader::PixelShader( me::IRenderer * renderer )
	: m_pimpl( new Pimpl( *this, renderer  ) )
	, m_assembly( false )
	, m_created( false )
	, m_isTrans( false )
{
}

PixelShader::PixelShader( me::IRenderer * renderer, PixelShaderParameters parameters )
	: PixelShader( renderer )
{
	Create( parameters );
}

PixelShader::~PixelShader()
{
	Destroy();
}

void PixelShader::Destroy()
{
	m_pimpl->Destroy();
}

void PixelShader::Create( PixelShaderParameters parameters )
{
	Destroy();

	m_parameters = parameters;
	m_pimpl->Create();
}

void PixelShader::Use( const RenderInfo & renderInfo ) const
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