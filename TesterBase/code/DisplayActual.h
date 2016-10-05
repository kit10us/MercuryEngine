#pragma once
#include <dx/DX.h>
#include <media/ImageInstanceData.h>

class DisplayActual
{
public:
	DisplayActual();
	~DisplayActual();

	void CreateDefaultShaders();
	void DestroyDefaultShaders();

	void SetHandle( HWND handle );
	HWND GetHandle() const;

	void SetPP( D3DPRESENT_PARAMETERS pp );
	const D3DPRESENT_PARAMETERS & GetPP() const;

	void SetDxDevice( IDirect3DDevice9 * dxDevice );
	IDirect3DDevice9 * GetDxDevice();

	void SetSwapChain( IDirect3DSwapChain9 * swapChain );
	IDirect3DSwapChain9 * GetSwapChain();

	void BeforeRender();
	void AfterRender();

	//unsigned int GetShaderFVF() const;
	void BeginShaderDraw( bool textured, const media::ImageInstanceData & imageInstanceData );
	void EndShaderDraw();

private:
	HWND m_handle;
	D3DPRESENT_PARAMETERS m_pp;
	CComPtr< IDirect3DDevice9 > m_dxDevice;
	CComPtr< IDirect3DSwapChain9 > m_swapChain;

	// For default shaders...
	CComPtr< IDirect3DPixelShader9 > m_pixelShaderNonTextured;
	CComPtr< ID3DXConstantTable > m_constantTablePSNonTextured;

	CComPtr< IDirect3DPixelShader9 > m_pixelShaderTextured;
	CComPtr< ID3DXConstantTable > m_constantTablePSTextured;

	CComPtr< IDirect3DVertexShader9 > m_vertexShader;
	CComPtr< ID3DXConstantTable > m_constantTableVS;

	CComPtr< IDirect3DVertexDeclaration9 > m_vertexDeclaration;

	float m_nearZ;
	float m_farZ;
};
