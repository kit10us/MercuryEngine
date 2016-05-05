#include "Display.h"
#include <exception>
#include <D3DX10tex.h>
#include "Error.h"

Display::Display( const RECT & area )
: m_area( area )
, m_sourcePosition()
, m_size()
, m_blendFunction()
, m_layeredWindowInfo()
, m_device( 0 )
, m_texture( 0 )
, m_target( 0 )
, m_surface()
, m_DC( 0 )
, m_hwnd( 0 )
, m_rootGraphic( new Graphic() )
, m_clearColor( D2D1::ColorF( D2D1::ColorF::Blue, 1.0f ) )
, m_opacity( 0.0f )
{
	m_size.cx = area.right - area.left;
	m_size.cy = area.bottom - area.top;

	m_blendFunction.SourceConstantAlpha = 255;
	m_blendFunction.AlphaFormat = AC_SRC_ALPHA;
}

Display::Display( const Display & display )
: m_area( display.m_area )
, m_sourcePosition( display.m_sourcePosition )
, m_size( display.m_size )
, m_blendFunction( display.m_blendFunction )
, m_layeredWindowInfo( display.m_layeredWindowInfo )
, m_device( display.m_device )
, m_texture( display.m_texture )
, m_target( display.m_target )
, m_surface( display.m_surface )
, m_DC( display.m_DC )
, m_hwnd( display.m_hwnd )
, m_rootGraphic( display.m_rootGraphic )
, m_clearColor( display.m_clearColor )
, m_opacity( display.m_opacity )
{
}

Display::~Display()
{
}

HWND Display::GetHwnd() const
{
	return m_hwnd;
}

const RECT & Display::GetRect() const
{
	return m_area;
}

int Display::GetWidth() const
{
	return m_size.cx;
}

int Display::GetHeight() const
{
	return m_size.cy;
}

D2D1_RECT_F Display::GetRectF() const
{
	return D2D1::RectF( static_cast< float >( m_area.left ), static_cast< float >( m_area.top ), static_cast< float >( m_area.right ), static_cast< float >( m_area.bottom ) );
}

ID2D1RenderTarget * Display::GetTarget() const
{
	return m_target;
}

Graphic * Display::GetRootGraphic()
{
	return m_rootGraphic.get();
}

void Display::SetClearColor( const D2D1_COLOR_F & clearColor )
{
	m_clearColor = clearColor;
}

const D2D1_COLOR_F & Display::GetClearColor() const
{
	return m_clearColor;
}

void Display::Create( HWND hwnd, ID2D1Factory * factory )
{
	m_hwnd = hwnd;

	HRESULT hr = S_OK;
	{ // Create device.
		hr = D3D10CreateDevice1( 0, D3D10_DRIVER_TYPE_HARDWARE, 0, D3D10_CREATE_DEVICE_BGRA_SUPPORT, D3D10_FEATURE_LEVEL_10_0, D3D10_1_SDK_VERSION, &m_device );
		OnFailedThrow( hr, "Failed to create DirectX Device!" );
	}

	{ // Create destination texture.
		D3D10_TEXTURE2D_DESC description = {};
		description.ArraySize = 1;
		description.BindFlags = D3D10_BIND_RENDER_TARGET;
		description.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		description.Width = GetWidth();
		description.Height = GetHeight();
		description.MipLevels = 1;
		description.SampleDesc.Count = 1;
		description.MiscFlags = D3D10_RESOURCE_MISC_GDI_COMPATIBLE;

		hr = m_device->CreateTexture2D( &description, 0, &m_texture );
		OnFailedThrow( hr, "Failed to create texture for display!" );
	}

	CreateDeviceDependantResources( factory );
}

void Display::CreateDeviceDependantResources( ID2D1Factory * factory )
{
	HRESULT hr = S_OK;

	if ( ! m_surface )
	{
		hr = m_texture->QueryInterface( &m_surface );
		OnFailedThrow( hr, "Failed to get the surface from the texture!" );
	}

	if ( ! m_target )
	{
		float dpiX;
        float dpiY;
        factory->GetDesktopDpi(&dpiX, &dpiY);
		D2D1_PIXEL_FORMAT format = D2D1::PixelFormat( DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED );
		D2D1_RENDER_TARGET_PROPERTIES properties = D2D1::RenderTargetProperties( D2D1_RENDER_TARGET_TYPE_DEFAULT, format, dpiX, dpiY, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE );
		hr = factory->CreateDxgiSurfaceRenderTarget( m_surface, &properties, &m_target );
		OnFailedThrow( hr, "Failed to create surface render target!" );
	}
}

void Display::DiscardDeviceDependentResources()
{
	m_target = 0;
}

void Display::Draw()
{
	static long lastTime = timeGetTime();

	m_target->BeginDraw();
	m_clearColor.a = m_opacity;
	m_target->Clear( m_clearColor );
	m_rootGraphic->Render( m_target, D2D1::Matrix3x2F::Identity() );

	HRESULT hr;

	ID2D1GdiInteropRenderTarget * interopTarget = 0;
	m_target->QueryInterface( &interopTarget );
	hr = interopTarget->GetDC( D2D1_DC_INITIALIZE_MODE_COPY, &m_DC );
	OnFailedThrow( hr, "Failed to get device context from render target!" );

	m_layeredWindowInfo.cbSize = sizeof( m_layeredWindowInfo );
	m_layeredWindowInfo.pptSrc = &m_sourcePosition;
	m_layeredWindowInfo.pptDst = (POINT*)&m_area;
	m_layeredWindowInfo.psize = &m_size;
	m_layeredWindowInfo.pblend = &m_blendFunction;
	m_layeredWindowInfo.dwFlags = ULW_ALPHA;
	m_layeredWindowInfo.hdcSrc = m_DC;
	bool result = UpdateLayeredWindowIndirect( m_hwnd, &m_layeredWindowInfo ) != 0;
	OnFalseGetLastErrorThrow( result, "Failed to update layered window!\n" );

	RECT rect = {};
	interopTarget->ReleaseDC( &rect );
	m_DC = 0;

	hr = m_target->EndDraw();
	if ( hr == D2DERR_RECREATE_TARGET )
	{
		DiscardDeviceDependentResources();
	}
	else
	{
		OnFailedThrow( hr, "Failed to end draw!" );
	}
}

void Display::Hide()
{
	m_opacity = 0.0f;
	ShowWindow( GetHwnd(), SW_HIDE );
}