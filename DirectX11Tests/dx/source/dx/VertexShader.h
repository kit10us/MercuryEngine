#pragma once 

#include <dx/IRenderer.h>
#include <dx/Shader.h>
#include <string>

namespace dx
{
	class VertexShader : public Shader
	{
		CComPtr< ID3D11VertexShader > m_vertexShader;
		CComPtr< ID3D10Blob > m_vertexShaderBuffer;
		CComPtr< ID3D11ShaderReflection > m_vertexShaderReflection;
	public:
		VertexShader( const std::string & path, const std::string & entryPoint, const std::string & target );
		void Use();
	};
}