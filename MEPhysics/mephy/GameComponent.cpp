// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <mephy/GameComponent.h>
#include <mephy/collider/BoxCollider.h>

using namespace mephy;

GameComponent::GameComponent()
	: Master( "MEPhysics" )
{
}

GameComponent::~GameComponent()
{																	 
}

me::object::IObjectComponent::ptr GameComponent::CreateBoxCollider( unify::BBox< float > bbox )
{									   
	collider::ColliderBase * collider = new collider::BoxCollider( bbox );
	return me::object::IObjectComponent::ptr( collider );
}

void GameComponent::OnAttach( me::IGame * game )
{
	me::GameComponent::OnAttach( game );

}
		 
void GameComponent::OnBeforeStartup()
{
}

void GameComponent::OnAfterStartup()
{
}

void GameComponent::OnBeforeUpdate()
{
}

void GameComponent::OnUpdate( me::UpdateParams params )
{
}

void GameComponent::OnAfterUpdate()
{
}

void GameComponent::OnRender( me::RenderParams params )
{
}

void GameComponent::OnDetach( me::IGame * game )
{
}

std::string GameComponent::GetWhat() const
{
	return std::string();
}
