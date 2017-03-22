// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <mephy/collider/ColliderBase.h>

using namespace mephy;
using namespace collider;
using namespace me;
using namespace object;

ColliderBase::ColliderBase( ColliderBase & component )
	: ObjectComponent( component )
{
}

ColliderBase::ColliderBase( std::string name )
	: ObjectComponent( name, true, false )
{
}

ColliderBase::~ColliderBase()
{
}

void ColliderBase::AddOnEnterAction( me::action::IAction::ptr action )
{
	m_onEnter.AddAction( action );
}

void ColliderBase::AddOnExitAction( me::action::IAction::ptr action )
{
	m_onExit.AddAction( action );
}

void ColliderBase::OnStart()
{
	ObjectComponent::OnStart();
}

void ColliderBase::OnUpdate( me::UpdateParams params )
{
	ObjectComponent::OnUpdate( params );
}

void ColliderBase::CollectGeometry( me::GeometryCache & cache, const unify::FrameLite * frame )
{																										
	ObjectComponent::CollectGeometry( cache, frame );
}

void ColliderBase::OnSuspend()
{
	ObjectComponent::OnSuspend();
}

void ColliderBase::OnResume()
{
	ObjectComponent::OnResume();
}

std::string ColliderBase::GetWhat() const
{
	return std::string();
}
