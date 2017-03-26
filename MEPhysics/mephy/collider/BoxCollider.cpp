// Copyright (c) 2002 - 2011, Quentin S. Smith
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
{
}

BoxCollider::BoxCollider( unify::V3< float > halfExt )
	: ColliderBase( Name() )
	, m_halfExt{ halfExt }
	, m_bbox{ halfExt, halfExt * -1.0f }
{
}

BoxCollider::~BoxCollider()
{
}

void BoxCollider::TestCollision( Entity* entity, const me::UpdateParams & params )
{
	// Get entity posiiton...
	auto originalPos = entity->GetObject()->GetFrame().GetPosition();
	auto pos = entity->GetObject()->GetFrame().GetPosition();
	
	// Transform the entity position into our space...
	GetObject()->GetFrame().GetMatrix().Inverse().TransformCoord( pos );

	auto myPos = GetObject()->GetFrame().GetPosition();

	if( m_bbox.ContainsPoint( pos ) )
	{
		this->PerformOnEnter( entity, params );
	}
}

me::object::IObjectComponent::ptr BoxCollider::Duplicate()
{
	auto duplicate = new BoxCollider( *this );
	return me::object::IObjectComponent::ptr( duplicate );
}