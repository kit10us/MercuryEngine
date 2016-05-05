// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/PixelShader.h>
#include <dxi/core/Game.h>

using namespace dxi;

PixelShader::PixelShader()
: m_shader( 0 )
, m_isTrans( false )
{
}

PixelShader::PixelShader( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile )
: m_isTrans( false )
, m_shader( 0 )
{
	CreateFromFile( filePath, entryPointName, profile );
}

void PixelShader::Destroy()
{
	if( m_shader )
	{
		m_shader->Release();
		m_shader = 0;
	}
	ShaderBase::Destroy();	
}

void PixelShader::DisuseShader()
{
	HRESULT result = win::DX::GetDxDevice()->SetPixelShader( 0 );
	if( FAILED( result ) )
	{
		throw unify::Exception( "Failed to set null pixel shader!" );
	}
}

void PixelShader::Use( const RenderInfo * renderInfo )
{
	HRESULT result = win::DX::GetDxDevice()->SetPixelShader( m_shader );
	if( FAILED( result ) )
	{
		throw unify::Exception( "Failed to set pixel shader!" );
	}
}

void PixelShader::SetTrans( bool bTrans )
{
	m_isTrans = bTrans;
}

bool PixelShader::IsTrans()
{
	return( this && m_isTrans ) ? true : FALSE;
}

void PixelShader::CreateThisShader()
{
	/*
    // Find entry handles...
    m_finalMatrix = m_constantTable->GetConstantByName( 0, "finalMatrix" );
    m_colorMultiply = m_constantTable->GetConstantByName( 0, "colorMultiply" );
    m_colorAddition = m_constantTable->GetConstantByName( 0, "colorAddition" );
    m_scalarTime = m_constantTable->GetConstantByName( 0, "scalarTime" );
    m_textureSize = m_constantTable->GetConstantByName( 0, "textureSize" );
	*/

	win::DX::GetDxDevice()->CreatePixelShader( (unsigned long *)m_codeBuffer->GetBufferPointer(), &m_shader );
}