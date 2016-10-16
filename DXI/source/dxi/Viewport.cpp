// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/Viewport.h>

using namespace dxi;

/* TODO: For reference.
#if defined( DIRECTX9 )
D3DVIEWPORT9 m_dxViewport;
#elif defined( DIRECTX11 )
D3D11_VIEWPORT m_dxViewport;
#endif
*/


Viewport::Viewport()
{
}

Viewport::Viewport( unify::V2< float > ul, unify::Size< float > size, float minDepth, float maxDepth )
	: m_ul( ul )
	, m_size( size )
	, m_minDepth( minDepth )
	, m_maxDepth( maxDepth )
{
}

Viewport::Viewport( float ulX, float ulY, float width, float height, float minDepth, float maxDepth )
	: Viewport( unify::V2< float >( ulX, ulY ), unify::Size< float >( width, height ), minDepth, maxDepth )
{
}

void Viewport::SetUL( unify::V2< float > ul )
{
	m_ul = ul;
}

void Viewport::SetSize( unify::Size< float > size )
{
	m_size = size;
}

void Viewport::SetMinDepth( float minDepth )
{
	m_minDepth = minDepth;
}

void Viewport::SetMaxDepth( float maxDepth )
{
	m_maxDepth = maxDepth;
}

unify::V2< float > Viewport::GetUL() const
{
	return m_ul;
}

unify::Size< float > Viewport::GetSize() const
{
	return m_size;
}

float Viewport::GetMinDepth() const
{
	return m_minDepth;
}

float Viewport::GetMaxDepth() const
{
	return m_maxDepth;
}
