#pragma once

#include <dx/Common.h>

/// <description>
/// Once a feature has been tested in the earlier tests, we can create simply helper functions here to simplify the later tests.
/// However, all functions within here must be simple and standalone so that the logic is followable; though using these functions
/// instead of duplicating the code in later tests is expected to make more difficult or complicated tests easier to follow.
/// </description>

void CreateVertexShader( CComPtr< ID3D11Device > dxDevice, std::string file, std::string entryPoint, std::string target, CComPtr< ID3D11VertexShader > & vertexShader, CComPtr< ID3D10Blob > & vertexShaderBuffer, CComPtr< ID3D11ShaderReflection > & vertexShaderReflection );

void CreatePixelShader( CComPtr< ID3D11Device > dxDevice, std::string file, std::string entryPoint, std::string target, CComPtr< ID3D11PixelShader > & pixelShader, CComPtr< ID3D10Blob > & pixelShaderBuffer );

void CreateComputeShader( CComPtr< ID3D11Device > dxDevice, std::string file, std::string entryPoint, std::string target, CComPtr< ID3D11ComputeShader > & computeShader, CComPtr< ID3D10Blob > & computeShaderBuffer, CComPtr< ID3D11ShaderReflection > & computeShaderReflection );
