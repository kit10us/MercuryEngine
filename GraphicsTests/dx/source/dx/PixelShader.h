#pragma once 

#include <dx/Shader.h>
#include <string>

namespace dx
{
	class PixelShader : public Shader
	{
	CComPtr< ID3D11PixelShader > pixelShader;
	CComPtr< ID3D10Blob > pixelShaderBuffer;
	public:
		PixelShader( const std::string & path, const std::string & entryPoint, const std::string & target );
	};
}