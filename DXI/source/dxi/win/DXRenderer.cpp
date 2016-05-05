// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/win/DXRenderer.h>
#include <dxi/win/WindowsOS.h>
#include <dxi/core/Game.h>
#include <cassert>

using namespace dxi;
using namespace win;

DXRenderer::DXRenderer( WindowsOS * os )
: m_OS( os )
{
}

DXRenderer::~DXRenderer()
{
	m_OS = 0;
}

size_t DXRenderer::GetNumberOfViewports() const
{
#ifdef DX11
	unsigned int numberOfViewports = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE; // Can crash if this is above this value.
	D3D11_VIEWPORT * viewports = 0;
	win::DX::GetDxContext()->RSGetViewports( &numberOfViewports, viewports );
	return numberOfViewports;
#else
	return 1;
#endif
}

void DXRenderer::GetViewport( Viewport & viewport )
{
#ifdef DX11
	unsigned int numberOfViewports = 1;
	D3D11_VIEWPORT dxViewport[ 1 ];
	win::DX::GetDxContext()->RSGetViewports( &numberOfViewports, dxViewport );
	viewport.SetTopLeftX( dxViewport[ 0 ].TopLeftX );
	viewport.SetTopLeftY( dxViewport[ 0 ].TopLeftY );
	viewport.SetWidth( dxViewport[ 0 ].Width );
	viewport.SetHeight( dxViewport[ 0 ].Height );
	viewport.SetMinDepth( dxViewport[ 0 ].MinDepth );
	viewport.SetMaxDepth( dxViewport[ 0 ].MaxDepth );
#else
	// TODO:
#endif
}

void DXRenderer::SetViewport( const Viewport & viewport )
{
#ifdef DX11
	unsigned int numberOfViewports = 1;
	D3D11_VIEWPORT dxViewport;
	dxViewport.TopLeftX = viewport.GetTopLeftX();
	dxViewport.TopLeftY = viewport.GetTopLeftY();
	dxViewport.Width = viewport.GetWidth();
	dxViewport.Height = viewport.GetHeight();
	dxViewport.MinDepth = viewport.GetMinDepth();
	dxViewport.MaxDepth = viewport.GetMaxDepth();
	win::DX::GetDxContext()->RSSetViewports( numberOfViewports, &dxViewport );
#else
	// TODO:
#endif
}

void DXRenderer::GetViewports( size_t & numberOfViewports, Viewport * viewports )
{
#ifdef DX11
	unsigned int realNumberOfViewports = GetNumberOfViewports();
	D3D11_VIEWPORT dxViewports[ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE ];
	win::DX::GetDxContext()->RSGetViewports( &realNumberOfViewports, dxViewports );
	for ( size_t v = 0; v < numberOfViewports && v < realNumberOfViewports; ++v )
	{
		viewports[ v ].SetTopLeftX( dxViewports[ v ].TopLeftX );
		viewports[ v ].SetTopLeftY( dxViewports[ v ].TopLeftY );
		viewports[ v ].SetWidth( dxViewports[ v ].Width );
		viewports[ v ].SetHeight( dxViewports[ v ].Height );
		viewports[ v ].SetMinDepth( dxViewports[ v ].MinDepth );
		viewports[ v ].SetMaxDepth( dxViewports[ v ].MaxDepth );
	}
	numberOfViewports = realNumberOfViewports;
#else
	// TODO:
#endif
}

void DXRenderer::SetViewports( size_t & numberOfViewports, const Viewport * viewports )
{
#ifdef DX11
	D3D11_VIEWPORT dxViewports[ D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE ];
	for ( size_t v = 0; v < numberOfViewports && v < numberOfViewports; ++v )
	{
		dxViewports[ v ].TopLeftX = viewports[ 0 ].GetTopLeftX();
		dxViewports[ v ].TopLeftY = viewports[ 0 ].GetTopLeftY();
		dxViewports[ v ].Width = viewports[ 0 ].GetWidth();
		dxViewports[ v ].Height = viewports[ 0 ].GetHeight();
		dxViewports[ v ].MinDepth = viewports[ 0 ].GetMinDepth();
		dxViewports[ v ].MaxDepth = viewports[ 0 ].GetMaxDepth();
	}
	win::DX::GetDxContext()->RSSetViewports( numberOfViewports, dxViewports );
#else
	// TODO:
#endif
}
