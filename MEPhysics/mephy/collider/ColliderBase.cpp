// Copyright (c) 2002 - 2018, Quentin S. Smith
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
	, m_isSolid{ true }
{
}

ColliderBase::~ColliderBase()
{
}

void ColliderBase::SetOnEnterAction( me::object::action::IObjectAction::ptr action )
{
	m_onEnter = action;
	m_isSolid = false;
}

void ColliderBase::SetOnExitAction( me::object::action::IObjectAction::ptr action )
{
	m_onExit = action;
	m_isSolid = false;
}

void ColliderBase::PerformOnEnter( Entity * entity, const me::UpdateParams & params )
{
	if( m_isSolid )
	{
		auto earlyPos = entity->GetEarly();
		entity->GetObject()->GetFrame().SetPosition( earlyPos.GetPosition() );

	}

	if( m_onEnter )
	{
		m_onEnter->Perform( entity->GetObject(), params.GetDelta() );
	}
}

void ColliderBase::PerformOnExit( Entity * entity, const me::UpdateParams & params )
{
	if( m_onExit )
	{
		m_onExit->Perform( entity->GetObject(), params.GetDelta() );
	}
}
