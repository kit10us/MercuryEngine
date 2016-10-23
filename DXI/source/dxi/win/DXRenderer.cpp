// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/win/DXRenderer.h>
#include <dxi/win/WindowsOS.h>
#include <dxi/core/Game.h>
#include <dxi/exception/FailedToCreate.h>
#include <dxi/exception/NotImplemented.h>
#include <atlbase.h>
#include <cassert>

using namespace dxi;
using namespace win;


#if defined(DIRECTX9)

class DXRenderer::Pimpl
{
public:
	DXRenderer & m_owner;
	win::WindowsOS * m_OS;
	core::Display m_display;
	D3DPRESENT_PARAMETERS m_pp;
	CComPtr< IDirect3DDevice9 > m_dxDevice;
	CComPtr< IDirect3DSwapChain9 > m_swapChain;
		   
	Pimpl( DXRenderer & owner, WindowsOS * os, core::Display display )
		: m_owner( owner )
		, m_OS( os )
		, m_display( display )
	{
		for ( int i = 0; i < os->RendererCount(); ++i )
		{
			auto dxRenderer = dynamic_cast<win::DXRenderer *>(os->GetRenderer( i ));
			if ( dxRenderer && dxRenderer->GetDxDevice() )
			{
				m_dxDevice = dxRenderer->GetDxDevice();
			}
		}

		m_pp = {};
		m_pp.BackBufferWidth = (UINT)display.GetSize().width;
		m_pp.BackBufferHeight = (UINT)display.GetSize().height;
		m_pp.BackBufferFormat = D3DFMT_X8R8G8B8;
		m_pp.BackBufferCount = 1;
		m_pp.MultiSampleType = D3DMULTISAMPLE_NONE;
		m_pp.MultiSampleQuality = 0;
		m_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_pp.hDeviceWindow = display.GetHandle();
		m_pp.Windowed = display.IsFullscreen() ? 0 : 1;
		m_pp.EnableAutoDepthStencil = true;
		m_pp.AutoDepthStencilFormat = D3DFMT_D16; // D3DFMT_D24S8
		m_pp.Flags = 0;

		if ( display.IsFullscreen() )
		{
			m_pp.FullScreen_RefreshRateInHz = 60;
			m_pp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		}

		IDirect3D9 * dx = Direct3DCreate9( D3D_SDK_VERSION );
		if ( !dx )
		{
			throw std::exception( "Failed to create DX!" );
		}

		bool hardwareAcceleration = true; // TODO: This should be configured elsewhere.
		D3DDEVTYPE deviceType = hardwareAcceleration ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF;
		unsigned int behaviorFlags = 0;
		behaviorFlags |= hardwareAcceleration ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		HRESULT hr = S_OK;

		if ( m_dxDevice == 0 )
		{
			hr = dx->CreateDevice( 0, deviceType, display.GetHandle(), behaviorFlags, &m_pp, &m_dxDevice );
			if ( FAILED( hr ) )
			{
				// Direct-X attempts to fix the presentation parameters, so a second attempt might resolve.
				hr = dx->CreateDevice( 0, deviceType, display.GetHandle(), behaviorFlags, &m_pp, &m_dxDevice );
				if ( FAILED( hr ) )
				{
					throw std::exception( "Failed to create Direct-X device!" );
				}
			}

			m_dxDevice->GetSwapChain( 0, &m_swapChain );
		}
		else
		{
			IDirect3DSwapChain9 * swapChain{};
			hr = m_dxDevice->CreateAdditionalSwapChain( &m_pp, &m_swapChain );
			if ( FAILED( hr ) )
			{
				throw std::exception( "Failed to create Direct-X swap chain!" );
			}
		}

		dx->Release();
		dx = 0;

		// Set our view matrix...
		D3DXMATRIX finalMatrix;

		D3DXMatrixOrthoOffCenterLH( &finalMatrix, 0, display.GetSize().width, display.GetSize().height, 0, display.GetNearZ(), display.GetFarZ() );
		m_dxDevice->SetTransform( D3DTS_PROJECTION, &finalMatrix );

		m_dxDevice->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );
		m_dxDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		m_dxDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
		m_dxDevice->SetRenderState( D3DRS_COLORVERTEX, 1 );
		m_dxDevice->SetRenderState( D3DRS_LIGHTING, 0 );
	}

	const core::Display & GetDisplay() const
	{
		return m_display;
	}

	IDirect3DDevice9 * GetDxDevice() const
	{
		return m_dxDevice;
	}

	IDirect3DSwapChain9 * GetSwapChain() const
	{
		return m_swapChain;
	}

	void BeforeRender()
	{
		HRESULT result;
		result = m_dxDevice->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB( 0, 0, 60 ), 1.0f, 0 );
		if ( FAILED( result ) )
		{
			throw unify::Exception( "Failed to clear in BeforeRender!" );
		}

		IDirect3DSurface9 * backBuffer;
		result = m_swapChain->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &backBuffer );
		result = m_dxDevice->SetRenderTarget( 0, backBuffer );

		result = m_dxDevice->BeginScene();
		if ( FAILED( result ) )
		{
			throw unify::Exception( "Failed to BeginScene in BeforeRender!" );
		}
	}

	void AfterRender()
	{
		HRESULT result;
		result = m_dxDevice->EndScene();
		if ( FAILED( result ) )
		{
			throw unify::Exception( "Failed to EndScene in AfterRender!" );
		}

		result = m_swapChain->Present( 0, 0, 0, 0, 0 );

		if ( FAILED( result ) )
		{
			throw unify::Exception( "Failed to Present in AfterRender!" );
		}
	}

	void SetCullMode( CullMode::TYPE mode )
	{
		D3DCULL dxCull{};
		switch ( mode )
		{
		case CullMode::None: dxCull = D3DCULL_NONE; break;
		case CullMode::Clockwise: dxCull = D3DCULL_CW; break;
		case CullMode::CounteClockwise: dxCull = D3DCULL_CCW; break;
		};

		m_dxDevice->SetRenderState( D3DRS_CULLMODE, dxCull );
	}

	Viewport GetViewport() const
	{
		return Viewport( 0, 0, GetDisplay().GetSize().width, GetDisplay().GetSize().height, GetDisplay().GetNearZ(), GetDisplay().GetFarZ() );
	}

	bool IsFullscreen() const
	{
		return m_display.IsFullscreen();
	}

	HWND GetHandle() const
	{
		return  m_display.GetHandle();
	}
};

#elif defined(DIRECTX11)
				
class DXRenderer::Pimpl
{
public:
	DXRenderer & m_owner;
	win::WindowsOS * m_OS;
	core::Display m_display;

	CComPtr< ID3D11DeviceContext > m_dxContext;
	CComPtr< ID3D11Device > m_dxDevice;
	DXGI_SWAP_CHAIN_DESC m_swapChainDesc;
	CComPtr< IDXGISwapChain > m_swapChain;
	CComPtr< ID3D11RenderTargetView > m_renderTargetView;
	CComPtr< ID3D11Texture2D > m_depthStencilBuffer;
	CComPtr< ID3D11DepthStencilView > m_depthStencilView;
	CComPtr< ID3D11RasterizerState > m_rasterizerState;

	Pimpl( DXRenderer & owner, WindowsOS * os, core::Display display )
		: m_owner( owner )
		, m_OS( os )
		, m_display( display )
		, m_swapChainDesc{}
	{
		for ( int i = 0; i < os->RendererCount(); ++i )
		{
			auto dxRenderer = dynamic_cast<win::DXRenderer *>(os->GetRenderer( i ));
			if ( dxRenderer && dxRenderer->GetDxDevice() )
			{
				m_dxDevice = dxRenderer->GetDxDevice();
			}
		}

		m_pp = {};
		m_pp.BackBufferWidth = (UINT)display.GetSize().width;
		m_pp.BackBufferHeight = (UINT)display.GetSize().height;
		m_pp.BackBufferFormat = D3DFMT_X8R8G8B8;
		m_pp.BackBufferCount = 1;
		m_pp.MultiSampleType = D3DMULTISAMPLE_NONE;
		m_pp.MultiSampleQuality = 0;
		m_pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		m_pp.hDeviceWindow = display.GetHandle();
		m_pp.Windowed = display.IsFullscreen() ? 0 : 1;
		m_pp.EnableAutoDepthStencil = true;
		m_pp.AutoDepthStencilFormat = D3DFMT_D16; // D3DFMT_D24S8
		m_pp.Flags = 0;

		if ( display.IsFullscreen() )
		{
			m_pp.FullScreen_RefreshRateInHz = 60;
			m_pp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		}

		IDirect3D9 * dx = Direct3DCreate9( D3D_SDK_VERSION );
		if ( !dx )
		{
			throw std::exception( "Failed to create DX!" );
		}

		bool hardwareAcceleration = true; // TODO: This should be configured elsewhere.
		D3DDEVTYPE deviceType = hardwareAcceleration ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF;
		unsigned int behaviorFlags = 0;
		behaviorFlags |= hardwareAcceleration ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		HRESULT hr = S_OK;

		if ( m_dxDevice == 0 )
		{
			hr = dx->CreateDevice( 0, deviceType, display.GetHandle(), behaviorFlags, &m_pp, &m_dxDevice );
			if ( FAILED( hr ) )
			{
				// Direct-X attempts to fix the presentation parameters, so a second attempt might resolve.
				hr = dx->CreateDevice( 0, deviceType, display.GetHandle(), behaviorFlags, &m_pp, &m_dxDevice );
				if ( FAILED( hr ) )
				{
					throw std::exception( "Failed to create Direct-X device!" );
				}
			}

			m_dxDevice->GetSwapChain( 0, &m_swapChain );
		}
		else
		{
			IDirect3DSwapChain9 * swapChain{};
			hr = m_dxDevice->CreateAdditionalSwapChain( &m_pp, &m_swapChain );
			if ( FAILED( hr ) )
			{
				throw std::exception( "Failed to create Direct-X swap chain!" );
			}
		}

		dx->Release();
		dx = 0;

		// Set our view matrix...
		D3DXMATRIX finalMatrix;

		D3DXMatrixOrthoOffCenterLH( &finalMatrix, 0, display.GetSize().width, display.GetSize().height, 0, display.GetNearZ(), display.GetFarZ() );
		m_dxDevice->SetTransform( D3DTS_PROJECTION, &finalMatrix );

		m_dxDevice->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF );
		m_dxDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
		m_dxDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
		m_dxDevice->SetRenderState( D3DRS_COLORVERTEX, 1 );
		m_dxDevice->SetRenderState( D3DRS_LIGHTING, 0 );
	}

	const core::Display & GetDisplay() const
	{
		return m_display;
	}

	IDirect3DDevice9 * GetDxDevice() const
	{
		return m_dxDevice;
	}

	IDirect3DSwapChain9 * GetSwapChain() const
	{
		return m_swapChain;
	}

	void BeforeRender()
	{
		float clearColor[] = { 0.1f, 0.0f, 0.2f, 1.0f };
		m_dxContext->ClearRenderTargetView( m_renderTargetView, clearColor );
		m_dxContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

		m_dxContext->OMSetDepthStencilState( 0, 0 );
		float blendFactors[] = { 0, 0, 0, 0 };
		m_dxContext->OMSetBlendState( 0, blendFactors, std::numeric_limits< unsigned int >::max() );
	}

	void AfterRender()
	{
		m_swapChain->Present( 0, 0 );
	}

	void SetCullMode( CullMode::TYPE mode )
	{
		// TODO: throw exception::NotImplemented( "DX11" );
	}

	Viewport GetViewport() const
	{
		return Viewport( 0, 0, GetDisplay().GetSize().width, GetDisplay().GetSize().height, GetDisplay().GetNearZ(), GetDisplay().GetFarZ() );
	}

	bool IsFullscreen() const
	{
		return m_display.IsFullscreen();
	}

	HWND GetHandle() const
	{
		return  m_display.GetHandle();
	}
};

#endif



DXRenderer::DXRenderer( WindowsOS * os, core::Display display )
	: m_pimpl( new Pimpl( *this, os, display ) )
{
}

DXRenderer::~DXRenderer()
{
}

const core::Display & DXRenderer::GetDisplay() const
{
	return m_pimpl->GetDisplay();
}

#if defined(DIRECTX9)

IDirect3DDevice9 * DXRenderer::GetDxDevice() const
{
	return m_pimpl->GetDxDevice();
}

IDirect3DSwapChain9 * DXRenderer::GetSwapChain() const
{
	return m_pimpl->GetSwapChain();
}

#elif defined(DIRECTX11)

#endif
	   
void DXRenderer::BeforeRender()
{
	m_pimpl->BeforeRender();
}

void DXRenderer::AfterRender()
{
	m_pimpl->AfterRender();
}

void DXRenderer::SetCullMode( CullMode::TYPE mode )
{
	m_pimpl->SetCullMode( mode );
}

Viewport DXRenderer::GetViewport() const
{
	return Viewport( 0, 0, GetDisplay().GetSize().width, GetDisplay().GetSize().height, GetDisplay().GetNearZ(), GetDisplay().GetFarZ() );
}

bool DXRenderer::IsFullscreen() const
{
	return m_pimpl->GetDisplay().IsFullscreen();
}

HWND DXRenderer::GetHandle() const
{
	return  m_pimpl->GetDisplay().GetHandle();
}

