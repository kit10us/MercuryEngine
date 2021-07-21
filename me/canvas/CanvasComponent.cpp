// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#include <me/canvas/CanvasComponent.h>

using namespace me::canvas;
using namespace me::scene;
using namespace me::render;

const char* CanvasComponent::Name()
{
	return "CanvasComponent";
}

CanvasComponent::CanvasComponent( game::IGame* gameInstance )
	: SceneComponent( gameInstance->GetOS(), Name() )
	, m_game( gameInstance )
	, m_position{ 0, 0 }
	, m_size{ 0, 0 }
	, m_projection{}
	, m_layer{}
{
}

void CanvasComponent::OnAttach( IScene * scene )
{
	SceneComponent::OnAttach( scene );
	m_layer.reset( new Layer( m_game, {0,0}, {100,100}, Anchor::StretchFull ) );
}

void CanvasComponent::OnDetach( IScene * scene )
{
	SceneComponent::OnDetach( scene );
}
		
void CanvasComponent::OnUpdate( const UpdateParams & params )
{
	RenderInfo myRenderInfo( params.renderInfo );

	m_size = params.renderer->GetDisplay().GetSize();

	m_projection = unify::MatrixOrthoOffCenterLH( m_position.x, m_position.x + m_size.width, m_position.y + m_size.height, m_position.y, 0.0f, 100.0f );
	myRenderInfo.SetViewMatrix( unify::MatrixIdentity() );
	myRenderInfo.SetProjectionMatrix( m_projection );

	m_layer->Update( UpdateParams{ params.renderer, myRenderInfo } );
	m_layer->UpdateLayout( UpdateParams{ params.renderer, myRenderInfo }, { { 0, 0 }, m_size } );
}

void CanvasComponent::OnRender( RenderGirl & renderGirl )
{
	const render::Params & params = *renderGirl.GetParams();

	RenderInfo myRenderInfo( params.renderInfo );

	m_size = params.renderer->GetDisplay().GetSize();

	m_projection = unify::MatrixOrthoOffCenterLH( m_position.x, m_position.x + m_size.width, m_position.y + m_size.height, m_position.y, 0.0f, 100.0f );
	myRenderInfo.SetViewMatrix( unify::MatrixIdentity() );
	myRenderInfo.SetProjectionMatrix( m_projection );

	m_layer->Render( { params.renderer, myRenderInfo } );
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

std::string CanvasComponent::GetWhat() const
{
	return std::string();
}
