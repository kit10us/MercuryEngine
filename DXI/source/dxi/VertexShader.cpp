// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/RenderInfo.h>
#include <dxi/VertexShader.h>
#include <dxi/win/DXDevice.h>

using namespace dxi;

void VertexShader::DisuseShader()
{
	win::DX::GetDxDevice()->SetVertexShader( 0 );
}

VertexShader::VertexShader()
: m_shader( 0 )
, m_finalMatrixHandle( 0 )
{
}

VertexShader::VertexShader( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile, VertexDeclaration vertexDeclaration )
: m_vertexDeclaration( vertexDeclaration )
, m_shader( 0 )
, m_finalMatrixHandle( 0 )
{
	CreateFromFile( filePath, entryPointName, profile );
}

void VertexShader::SetVertexDeclaration( VertexDeclaration vertexDeclaration )
{
	m_vertexDeclaration = vertexDeclaration;
}

VertexDeclaration VertexShader::GetVertexDeclaration() const
{
	return m_vertexDeclaration;
}

void VertexShader::Destroy()
{
	if( m_shader )
	{
		m_shader->Release();
	}
	ShaderBase::Destroy();	
}

void VertexShader::Use( const RenderInfo * renderInfo )
{
	HRESULT result = S_OK;
	m_vertexDeclaration.Use();

	if( renderInfo != 0 )
	{
		if ( m_finalMatrixHandle != 0 )
		{
			unify::Matrix final = renderInfo->GetFinalMatrix();
			result = GetConstantTable()->SetMatrix( win::DX::GetDxDevice(), m_finalMatrixHandle, final.GetD3DXMatrix() );
		}
	}

	result = win::DX::GetDxDevice()->SetVertexShader( m_shader );
	if( FAILED( result ) )
	{
		throw unify::Exception( "Failed to set vertex shader!" );
	}
}
void VertexShader::CreateThisShader()
{
	win::DX::GetDxDevice()->CreateVertexShader( (unsigned long *)m_codeBuffer->GetBufferPointer(), &m_shader );
	m_finalMatrixHandle = GetConstantTable()->GetConstantByName( 0, "finalMatrix" );
}
