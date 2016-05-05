// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/ShaderBase.h>
#include <unify/String.h>
#include <unify/Exception.h>

using namespace dxi;

ShaderBase::ShaderBase()
: m_assembly( false )
, m_constantTable( 0 )
, m_codeBuffer( 0 )
, m_created( false )
{
}

ShaderBase::ShaderBase( const std::string & entryPointName, const std::string & profile )
: m_assembly( false )
, m_constantTable( 0 )
, m_codeBuffer( 0 )
, m_created( false )
, m_entryPointName( entryPointName )
, m_profile( profile )
{
}


ShaderBase::~ShaderBase()
{
	Destroy();
}

void ShaderBase::Create()
{
	HRESULT result = S_OK;
	ID3DXBuffer * errorBuffer = 0;
	if ( ! m_filePath.Empty() )
	{
		if ( m_assembly )
		{
			DWORD flags = D3DXSHADER_DEBUG;
			result = D3DXAssembleShaderFromFileA( m_filePath.ToString().c_str(), 0, 0, D3DXSHADER_DEBUG, &m_codeBuffer, &errorBuffer );
		}
		else
		{
			DWORD flags = 0;
			result = D3DXCompileShaderFromFileA( m_filePath.ToString().c_str(), 0, 0, m_entryPointName.c_str(), m_profile.c_str(), flags, &m_codeBuffer, &errorBuffer, &m_constantTable );
		}
		if( m_codeBuffer == 0 || FAILED( result ) )
		{
			m_errorMessage = static_cast< char * >( errorBuffer->GetBufferPointer() );
			errorBuffer->Release();
			errorBuffer = 0;
			throw unify::Exception( "Failed to create shader \"" + m_filePath.ToString() + "\": " + m_errorMessage + "\n" );
		}
		
		CreateThisShader();
		m_created = true;
	}
	else if ( m_code.empty() )
	{
		if ( m_assembly )
		{
			DWORD flags = 0;
			D3DXAssembleShader(	m_code.c_str(), static_cast< unsigned int >( m_code.length() - 1 ), 0, NULL, flags, &m_codeBuffer, &errorBuffer );
		}
		else
		{
			DWORD flags = 0;
			D3DXCompileShader( m_code.c_str(), static_cast< unsigned int >( m_code.length() - 1 ), 0, 0, m_entryPointName.c_str(), m_profile.c_str(), flags, &m_codeBuffer, &errorBuffer, &m_constantTable ); 
		}

		if( m_codeBuffer == 0 || FAILED( result ) ) 
		{
			m_errorMessage = static_cast< char * >( errorBuffer->GetBufferPointer() );
			errorBuffer->Release();
			errorBuffer = 0;
			throw unify::Exception( "Failed to create shader from code: " + m_errorMessage + "\n" );
		}
		
		CreateThisShader();
		m_created = true;
	}
	else
	{
		throw unify::Exception( "Attempted to create shader from unknown source!" );
	}
}

void ShaderBase::Destroy()
{
	if( m_codeBuffer )
	{
		m_codeBuffer->Release();
		m_codeBuffer = 0;
	}
	if( m_constantTable )
	{
		m_constantTable->Release();
		m_constantTable = 0;
	}
}

void ShaderBase::Validate()
{
}

void ShaderBase::Invalidate()
{
}

void ShaderBase::CreateFromFile( const unify::Path & filePath, const std::string & entryPointName, const std::string & profile )
{
	Destroy();

	m_filePath = filePath;
	m_entryPointName = entryPointName;
	m_profile = profile;

	Create();
}

void ShaderBase::CreateFromCode( const std::string & code, const std::string & entryPointName, const std::string & profile )
{
	Destroy();

	m_code = code;
	m_entryPointName = entryPointName;
	m_profile = profile;

	Create();
}

std::string ShaderBase::GetError()
{
	return m_errorMessage;
}

ID3DXConstantTable * ShaderBase::GetConstantTable()
{
	return m_constantTable;
}
