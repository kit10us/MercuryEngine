#include <dx/VertexShader.h>

using namespace dx;

VertexShader::VertexShader( const std::string & path, const std::string & entryPoint, const std::string & target )
{
	HRESULT result = S_OK;
	CComPtr< ID3D10Blob > errorBlob; // Generic buffer for error data.
	D3D_SHADER_MACRO * shaderMacros = 0;
	unsigned int flags1 = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags1 |= D3DCOMPILE_DEBUG;
#endif
	unsigned int flags2 = 0; // Only used for effect compilation.

	std::wstring wpath;
    wpath.assign (path.begin (), path.end ());

	result = D3DCompileFromFile( wpath.c_str(), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), target.c_str(), flags1, flags2, &m_vertexShaderBuffer, &errorBlob );
	if ( FAILED( result ) )
	{
		OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
		assert( ! FAILED( result ) );
	}

	auto dxDevice = IRenderer::GetInstance()->GetDxDevice();

	ID3D11ClassLinkage * classLinkage = nullptr;
	result = dxDevice->CreateVertexShader( m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), classLinkage, &m_vertexShader );
	assert( ! FAILED( result ) );

	result = D3DReflect( m_vertexShaderBuffer->GetBufferPointer(), m_vertexShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&m_vertexShaderReflection );
	assert( ! FAILED( result ) );
}

void VertexShader::Use()
{
	/*
	auto dxContext = IRenderer::GetInstance()->GetDxContext();
	dxContext->VSSetConstantBuffers( 0, 1, &m_vertexShaderConstantBuffer.p );
	dxContext->VSSetShader( m_vertexShader, nullptr, 0 );
	*/
}