#include <dx/DXHelpers.h>

void CreateVertexShader( CComPtr< ID3D11Device > dxDevice, std::string file, std::string entryPoint, std::string target, CComPtr< ID3D11VertexShader > & vertexShader, CComPtr< ID3D10Blob > & vertexShaderBuffer, CComPtr< ID3D11ShaderReflection > & vertexShaderReflection )
{
	HRESULT result = {};
	CComPtr< ID3D10Blob > errorBlob; // Generic buffer for error data.
	D3D_SHADER_MACRO * shaderMacros = 0;
	unsigned int flags1 = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags1 |= D3DCOMPILE_DEBUG;
#endif
	unsigned int flags2 = 0; // Only used for effect compilation.

	std::wstring wFile( file.begin(), file.end() );

	result = D3DCompileFromFile( wFile.c_str(), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), target.c_str(), flags1, flags2, &vertexShaderBuffer, &errorBlob );
	if( FAILED( result ) )
	{
		OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
		assert( !FAILED( result ) );
	}

	ID3D11ClassLinkage * classLinkage = nullptr;
	result = dxDevice->CreateVertexShader( vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), classLinkage, &vertexShader );
	assert( !FAILED( result ) );

	result = D3DReflect( vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&vertexShaderReflection );
	assert( !FAILED( result ) );
}

void CreatePixelShader( CComPtr< ID3D11Device > dxDevice, std::string file, std::string entryPoint, std::string target, CComPtr< ID3D11PixelShader > & pixelShader, CComPtr< ID3D10Blob > & pixelShaderBuffer )
{
	HRESULT result = {};
	CComPtr< ID3D10Blob > errorBlob; // Generic buffer for error data.
	D3D_SHADER_MACRO * shaderMacros = 0;
	unsigned int flags1 = D3DCOMPILE_ENABLE_STRICTNESS;

	unsigned int flags2 = 0; // Only used for effect compilation.

	std::wstring wFile( file.begin(), file.end() );

	result = D3DCompileFromFile( wFile.c_str(), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), target.c_str(), flags1, flags2, &pixelShaderBuffer, &errorBlob );
	if( FAILED( result ) )
	{
		OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
		assert( !FAILED( result ) );
	}

	ID3D11ClassLinkage * classLinkage = nullptr;
	result = dxDevice->CreatePixelShader( pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), classLinkage, &pixelShader );
	assert( !FAILED( result ) );
}

void CreateComputeShader( CComPtr< ID3D11Device > dxDevice, std::string file, std::string entryPoint, std::string target, CComPtr< ID3D11ComputeShader > & computeShader, CComPtr< ID3D10Blob > & computeShaderBuffer, CComPtr< ID3D11ShaderReflection > & computeShaderReflection )
{
	HRESULT result = {};
	CComPtr< ID3D10Blob > errorBlob; // Generic buffer for error data.
	D3D_SHADER_MACRO * shaderMacros = 0;
	unsigned int flags1 = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags1 |= D3DCOMPILE_DEBUG;
#endif
	unsigned int flags2 = 0; // Only used for effect compilation.

	std::wstring wFile( file.begin(), file.end() );

	result = D3DCompileFromFile( wFile.c_str(), shaderMacros, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryPoint.c_str(), target.c_str(), flags1, flags2, &computeShaderBuffer, &errorBlob );
	if( FAILED( result ) )
	{
		OutputDebugStringA( (char*)errorBlob->GetBufferPointer() );
		assert( !FAILED( result ) );
	}

	ID3D11ClassLinkage * classLinkage = nullptr;
	result = dxDevice->CreateComputeShader( computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), classLinkage, &computeShader );
	assert( !FAILED( result ) );

	result = D3DReflect( computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&computeShaderReflection );
	assert( !FAILED( result ) );
}