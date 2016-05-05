// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/Viewport.h>

using namespace dxi;

Viewport::Viewport()
{
}

Viewport::Viewport( float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth )
{
	SetTopLeftX( topLeftX );
	SetTopLeftY( topLeftY );
	SetWidth( width );
	SetHeight( height );
	SetMinDepth( minDepth );
	SetMaxDepth( maxDepth );
}

void Viewport::SetTopLeftX( float topLeftX )
{
#ifdef DX11
	m_dxViewport.TopLeftX = topLeftX;
#else
	m_dxViewport.X = static_cast< DWORD >( topLeftX );
#endif
}

void Viewport::SetTopLeftY( float topLeftY )
{
#ifdef DX11
	m_dxViewport.TopLeftY = topLeftY;
#else
	m_dxViewport.Y = static_cast< DWORD >( topLeftY );
#endif
}

void Viewport::SetWidth( float width )
{
	m_dxViewport.Width = static_cast< DWORD >( width );
}

void Viewport::SetHeight( float height )
{
	m_dxViewport.Height = static_cast< DWORD >( height );
}

void Viewport::SetMinDepth( float minDepth )
{
#ifdef DX11
	m_dxViewport.MinDepth = minDepth;
#else
	m_dxViewport.MinZ = minDepth;
#endif
}

void Viewport::SetMaxDepth( float maxDepth )
{
#ifdef DX11
	m_dxViewport.MaxDepth = maxDepth;
#else
	m_dxViewport.MaxZ = maxDepth;
#endif
}

float Viewport::GetTopLeftX() const
{
#ifdef DX11
	return m_dxViewport.TopLeftX;
#else
	return static_cast< float >( m_dxViewport.X );
#endif
}

float Viewport::GetTopLeftY() const
{
#ifdef DX11
	return m_dxViewport.TopLeftY;
#else
	return static_cast< float >( m_dxViewport.Y );
#endif
}

float Viewport::GetWidth() const
{
	return static_cast< float >( m_dxViewport.Width );
}

float Viewport::GetHeight() const
{
	return static_cast< float >( m_dxViewport.Height );
}

float Viewport::GetMinDepth() const
{
#ifdef DX11
	return m_dxViewport.MinDepth;
#else
	return m_dxViewport.MinZ;
#endif
}

float Viewport::GetMaxDepth() const
{
#ifdef DX11
	return m_dxViewport.MaxDepth;
#else
	return m_dxViewport.MaxZ;
#endif
}
