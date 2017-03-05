// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <me/scene2d/CanvasComponent.h>

using namespace me;
using namespace scene2d;
using namespace scene;

CanvasComponent::CanvasComponent( IGame * game )
	: m_game( game )
	, m_position( 0, 0 )
	, m_size( 0, 0 )
	, m_enabled( true )
{
}

const char * CanvasComponent::GetName() const
{
	return "CanvasComponent";
}

bool CanvasComponent::IsEnabled() const
{
	return m_enabled;
}

void CanvasComponent::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

void CanvasComponent::OnAttach( Scene * scene )
{
	m_layer.reset( new Layer( m_game, {0,0}, {100,100}, Anchor::StretchFull ) );
}

void CanvasComponent::OnDetach( Scene * scene )
{
}

void CanvasComponent::OnInit( Scene * scene )
{
}

void CanvasComponent::OnStart( Scene * scene )
{
}
			
void CanvasComponent::OnUpdate( Scene * scene, UpdateParams params )
{
	RenderInfo myRenderInfo( params.renderInfo );

	m_size = params.renderer->GetDisplay().GetSize();

	m_projection = unify::MatrixOrthoOffCenterLH( m_position.x, m_position.x + m_size.width, m_position.y + m_size.height, m_position.y, 0.0f, 100.0f );
	myRenderInfo.SetViewMatrix( unify::MatrixIdentity() );
	myRenderInfo.SetProjectionMatrix( m_projection );

	m_layer->Update( UpdateParams{ params.renderer, myRenderInfo } );
	m_layer->UpdateLayout( UpdateParams{ params.renderer, myRenderInfo }, m_size );
}

void CanvasComponent::OnRender( Scene * scene, RenderParams params )
{
	RenderInfo myRenderInfo( params.renderInfo );

	m_size = params.renderer->GetDisplay().GetSize();

	m_projection = unify::MatrixOrthoOffCenterLH( m_position.x, m_position.x + m_size.width, m_position.y + m_size.height, m_position.y, 0.0f, 100.0f );
	myRenderInfo.SetViewMatrix( unify::MatrixIdentity() );
	myRenderInfo.SetProjectionMatrix( m_projection );

	m_layer->Render( RenderParams{ params.renderer, myRenderInfo } );
}

void CanvasComponent::OnSuspend()
{
	m_layer->OnSuspend();
}

void CanvasComponent::OnResume()
{
	m_layer->OnResume();
}

Layer * CanvasComponent::GetLayer()
{
	return m_layer.get();
}
