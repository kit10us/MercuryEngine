// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/scene2d/Canvas.h>

using namespace me;
using namespace scene2d;
using namespace scene;

Canvas::Canvas( IGame * game )
	: m_game( game )
	, m_position( 0, 0 )
	, m_size( 0, 0 )
	, m_enabled( true )
{
}

const char * Canvas::GetName() const
{
	return "Canvas";
}

bool Canvas::IsEnabled() const
{
	return m_enabled;
}

void Canvas::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

void Canvas::OnAttach( Scene * scene )
{
}

void Canvas::OnDetach( Scene * scene )
{
}

void Canvas::Canvas::OnInit( Scene * scene )
{
}

void Canvas::OnStart( Scene * scene )
{
}
			
void Canvas::OnUpdate( Scene * scene, IRenderer * renderer, const RenderInfo & renderInfo )
{
	RenderInfo myRenderInfo( renderInfo );

	m_size = renderer->GetDisplay().GetSize();

	m_projection = unify::MatrixOrthoOffCenterLH( m_position.x, m_position.x + m_size.width, m_position.y + m_size.height, m_position.y, 0.0f, 100.0f );
	myRenderInfo.SetViewMatrix( unify::MatrixIdentity() );
	myRenderInfo.SetProjectionMatrix( m_projection );

	for( auto e : m_elements )
	{
		e->Update( renderer, myRenderInfo );
	}
}

void Canvas::OnRender( Scene * scene, IRenderer * renderer, const RenderInfo & renderInfo )
{
	RenderInfo myRenderInfo( renderInfo );

	m_size = renderer->GetDisplay().GetSize();

	m_projection = unify::MatrixOrthoOffCenterLH( m_position.x, m_position.x + m_size.width, m_position.y + m_size.height, m_position.y, 0.0f, 100.0f );
	myRenderInfo.SetViewMatrix( unify::MatrixIdentity() );
	myRenderInfo.SetProjectionMatrix( m_projection );

	for( auto e : m_elements )
	{
		e->Render( renderer, myRenderInfo );
	}
}

void Canvas::OnSuspend()
{
}

void Canvas::OnResume()
{
}

void Canvas::AddElement( IElement::ptr element )
{
	m_elements.push_back( element );
}
