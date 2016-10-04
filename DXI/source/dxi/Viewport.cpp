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
#if defined( DIRECTX9 )
	m_dxViewport.X = static_cast< DWORD >(topLeftX);
#elif defined( DIRECTX11 )
	m_dxViewport.TopLeftX = topLeftX;
#endif
}

void Viewport::SetTopLeftY( float topLeftY )
{
#if defined( DIRECTX9 )
	m_dxViewport.Y = static_cast< DWORD >(topLeftY);
#elif defined( DIRECTX11 )
	m_dxViewport.TopLeftY = topLeftY;
#endif
}

void Viewport::SetWidth( float width )
{
#if defined( DIRECTX9 )
	m_dxViewport.Width = static_cast< DWORD >(width);
#elif defined( DIRECTX11 )
	m_dxViewport.Width = width;
#endif
}

void Viewport::SetHeight( float height )
{
#if defined( DIRECTX9 )
	m_dxViewport.Height = static_cast< DWORD >(height);
#elif defined( DIRECTX11 )
	m_dxViewport.Height = height;
#endif
}

void Viewport::SetMinDepth( float minDepth )
{
#if defined( DIRECTX9 )
	m_dxViewport.MinZ = minDepth;
#elif defined( DIRECTX11 )
	m_dxViewport.MinDepth = minDepth;
#endif
}

void Viewport::SetMaxDepth( float maxDepth )
{
#if defined( DIRECTX9 )
	m_dxViewport.MaxZ = maxDepth;
#elif defined( DIRECTX11 )
	m_dxViewport.MaxDepth = maxDepth;
#endif
}

float Viewport::GetTopLeftX() const
{
#if defined( DIRECTX9 )
	return static_cast< float >(m_dxViewport.X);
#elif defined( DIRECTX11 )
	return m_dxViewport.TopLeftX;
#endif
}

float Viewport::GetTopLeftY() const
{
#if defined( DIRECTX9 )
	return static_cast< float >(m_dxViewport.Y);
#elif defined( DIRECTX11 )
	return m_dxViewport.TopLeftY;
#endif
}

float Viewport::GetWidth() const
{
	return static_cast< float >(m_dxViewport.Width);
}

float Viewport::GetHeight() const
{
	return static_cast< float >(m_dxViewport.Height);
}

float Viewport::GetMinDepth() const
{
#if defined( DIRECTX9 )
	return m_dxViewport.MinZ;
#elif defined( DIRECTX11 )
	return m_dxViewport.MinDepth;
#endif
}

float Viewport::GetMaxDepth() const
{
#if defined( DIRECTX9 )
	return m_dxViewport.MaxZ;
#elif defined( DIRECTX11 )
	return m_dxViewport.MaxDepth;
#endif
}
