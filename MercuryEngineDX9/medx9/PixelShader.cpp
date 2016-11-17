// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <medx9/PixelShader.h>
#include <medx9/Renderer.h>
#include <me/exception/NotImplemented.h>
#include <me/exception/FailedToCreate.h>
#include <atlbase.h>

using namespace medx9;
using namespace me;

class PixelShader::Pimpl
{
	PixelShader & m_owner;
	const Renderer * m_renderer;
	CComPtr< ID3DXBuffer > m_codeBuffer;
	CComPtr< ID3DXConstantTable > m_constantTable;
	CComPtr< IDirect3DPixelShader9 > m_shader;

public:
	Pimpl( PixelShader & owner, const me::IRenderer * renderer )
		: m_owner( owner )
		, m_renderer( dynamic_cast< const Renderer * >( renderer ) )
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
		if ( ! m_owner.m_parameters.code.empty() )
		{
		}
		else if( ! m_owner.m_parameters.path.Empty() )
		{
			if( m_owner.m_assembly )
			{
				DWORD flags = D3DXSHADER_DEBUG;
				result = D3DXAssembleShaderFromFileA( m_owner.m_parameters.path.ToString().c_str(), 0, 0, D3DXSHADER_DEBUG, &m_codeBuffer, &errorBuffer );
			}
			else
			{
				DWORD flags = 0;
				result = D3DXCompileShaderFromFileA( m_owner.m_parameters.path.ToString().c_str(), 0, 0, m_owner.m_parameters.entryPointName.c_str(), m_owner.m_parameters.profile.c_str(), flags, &m_codeBuffer, &errorBuffer, &m_constantTable );
			}
			if( m_codeBuffer == 0 || FAILED( result ) )
			{
				m_owner.m_errorMessage = static_cast< char * >(errorBuffer->GetBufferPointer());
				throw unify::Exception( "Failed to create shader \"" + m_owner.m_parameters.path.ToString() + "\": " + m_owner.m_errorMessage + "\n" );
			}
		}
		else if( m_owner.m_parameters.code.empty() )
		{
			if( m_owner.m_assembly )
			{
				DWORD flags = 0;
				D3DXAssembleShader( m_owner.m_parameters.code.c_str(), static_cast< unsigned int >(m_owner.m_parameters.code.length() - 1), 0, NULL, flags, &m_codeBuffer, &errorBuffer );
			}
			else
			{
				DWORD flags = 0;
				D3DXCompileShader( m_owner.m_parameters.code.c_str(), static_cast< unsigned int >(m_owner.m_parameters.code.length() - 1), 0, 0, m_owner.m_parameters.entryPointName.c_str(), m_owner.m_parameters.profile.c_str(), flags, &m_codeBuffer, &errorBuffer, &m_constantTable );
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

	void Use( const RenderInfo & renderInfo ) const
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


PixelShader::PixelShader( const me::IRenderer * renderer )
	: m_pimpl( new Pimpl( *this, renderer  ) )
	, m_assembly( false )
	, m_created( false )
	, m_isTrans( false )
{
}

PixelShader::PixelShader( const me::IRenderer * renderer, PixelShaderParameters parameters )
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