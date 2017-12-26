// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <mephy/collider/BoxCollider.h>

using namespace mephy;
using namespace collider;
using namespace me;
using namespace collider;

char* BoxCollider::Name()
{
	return "MEPhyBoxCollider";
}

BoxCollider::BoxCollider( BoxCollider & component )
	: ColliderBase( component )
	, m_halfExt{ component.m_halfExt }
	, m_bbox{ component.m_bbox }
	, m_once{ false }
{
}

BoxCollider::BoxCollider( unify::V3< float > halfExt, bool moveable, float mass )
	: ColliderBase( Name(), moveable, mass )
	, m_halfExt{ halfExt }
	, m_bbox{ halfExt, halfExt * -1.0f }
	, m_once{ false }
{
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::TestCollision( ColliderBase * entity, const me::UpdateParams & params )
{
	// Get entity posiitons...
	auto earlyPos = entity->GetEarly().GetPosition();
	auto currentPos = entity->GetObject()->GetFrame().GetPosition();
	
	// Transform the entity positions into our space...
	GetObject()->GetFrame().GetMatrix().Inverse().TransformCoord( earlyPos );
	GetObject()->GetFrame().GetMatrix().Inverse().TransformCoord( currentPos );

	bool wasIn;
	bool isIn;

	// If we are new, then nothing could have been in us before...
	if( m_once )
	{
		wasIn = false;
	}
	else
	{
		wasIn = m_bbox.ContainsPoint( earlyPos );
	}

	isIn = m_bbox.ContainsPoint( currentPos );

	if( ! wasIn && isIn )
	{
		/*
		unify::Ray ray( earlyPos, currentPos );

		unify::V3< float > hitPoint;
		bool hit = m_bbox.RayTest( ray, hitPoint );
		*/
		
		this->PerformOnEnter( entity, params );
	}

	else if ( wasIn && ! isIn )
	{
		this->PerformOnExit( entity, params );
	}

	m_once = false;
}

me::object::component::IObjectComponent::ptr BoxCollider::Duplicate()
{
	auto duplicate = new BoxCollider( *this );
	return me::object::component::IObjectComponent::ptr( duplicate );
}