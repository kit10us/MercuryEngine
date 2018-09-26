// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <canvas/SceneComponent.h>

using namespace me;
using namespace canvas;
using namespace render;

char * SceneComponent::Name()
{
	return "CanvasSceneComponent";
}

SceneComponent::SceneComponent( game::IGame * gameInstance )
	: scene::SceneComponent( gameInstance->GetOS(), Name() )
	, m_game( gameInstance )
	, m_position( 0, 0 )
	, m_size( 0, 0 )
{
}

void SceneComponent::OnAttach( scene::IScene * scene )
{
	scene::SceneComponent::OnAttach( scene );
	m_layer.reset( new Layer( m_game, {0,0}, {100,100}, Anchor::StretchFull ) );
}

void SceneComponent::OnDetach( scene::IScene * scene )
{
	scene::SceneComponent::OnDetach( scene );
}
		
void SceneComponent::OnUpdate( const UpdateParams & params )
{
	RenderInfo myRenderInfo( params.renderInfo );

	m_size = params.renderer->GetDisplay().GetSize();

	m_projection = unify::MatrixOrthoOffCenterLH( m_position.x, m_position.x + m_size.width, m_position.y + m_size.height, m_position.y, 0.0f, 100.0f );
	myRenderInfo.SetViewMatrix( unify::MatrixIdentity() );
	myRenderInfo.SetProjectionMatrix( m_projection );

	m_layer->Update( UpdateParams{ params.renderer, myRenderInfo } );
	m_layer->UpdateLayout( UpdateParams{ params.renderer, myRenderInfo }, { { 0, 0 }, m_size } );
}

void SceneComponent::OnRender( scene::RenderGirl & renderGirl )
{
	const render::Params & params = *renderGirl.GetParams();

	RenderInfo myRenderInfo( params.renderInfo );

	m_size = params.renderer->GetDisplay().GetSize();

	m_projection = unify::MatrixOrthoOffCenterLH( m_position.x, m_position.x + m_size.width, m_position.y + m_size.height, m_position.y, 0.0f, 100.0f );
	myRenderInfo.SetViewMatrix( unify::MatrixIdentity() );
	myRenderInfo.SetProjectionMatrix( m_projection );

	m_layer->Render( render::Params{ params.renderer, myRenderInfo } );
}

void SceneComponent::OnSuspend()
{
	m_layer->OnSuspend();
}

void SceneComponent::OnResume()
{
	m_layer->OnResume();
}

Layer * SceneComponent::GetLayer()
{
	return m_layer.get();
}

std::string SceneComponent::GetWhat() const
{
	return std::string();
}
