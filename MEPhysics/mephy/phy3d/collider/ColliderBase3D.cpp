// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mephy/phy3d/collider/ColliderBase3D.h>

using namespace mephy;
using namespace phy3d;
using namespace collider;
using namespace me;
using namespace object;

ColliderBase::ColliderBase( ColliderBase & component )
	: ObjectComponent( component )
{
}

ColliderBase::ColliderBase( std::string name, bool moveable, float mass )
	: ObjectComponent( name, true, false )
	, m_isSolid{ true }
	, m_moveable{ moveable }
	, m_mass{ mass }
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

void ColliderBase::SetMoveable( bool moveable )
{
	m_moveable = moveable;
}

bool ColliderBase::GetMoveable() const
{
	return m_moveable;
}

void ColliderBase::SetMass( float mass )
{
	m_mass = mass;
}

float ColliderBase::GetMass() const
{
	return m_mass;
}

void ColliderBase::CaptureEarly()
{
	m_early = GetObject()->GetFrame().GetMatrix();
}

const unify::Matrix & ColliderBase::GetEarly() const
{
	return m_early;
}

void ColliderBase::PerformOnEnter( ColliderBase * collider, const me::UpdateParams & params )
{
	if( m_isSolid )
	{
		auto earlyPos = collider->GetEarly();
		collider->GetObject()->GetFrame().SetPosition( earlyPos.GetPosition() );

	}

	if( m_onEnter )
	{
		m_onEnter->Perform( collider->GetObject(), params.GetDelta() );
	}
}

void ColliderBase::PerformOnExit( ColliderBase * collider, const me::UpdateParams & params )
{
	if( m_onExit )
	{
		m_onExit->Perform( collider->GetObject(), params.GetDelta() );
	}
}


void ColliderBase::OnAttach( me::object::Object * object )
{
	ObjectComponent::OnAttach( object );
}

void ColliderBase::OnDetach( me::object::Object * object )
{
	int x( 0 ); x;
	ObjectComponent::OnDetach( object );
}

