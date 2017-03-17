// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <mephy/collider/BoxCollider.h>

using namespace mephy;
using namespace collider;
using namespace me;
using namespace collider;

BoxCollider::BoxCollider( BoxCollider & component )
	: ColliderBase( component )
	, m_bbox{ component.m_bbox }
{
}

BoxCollider::BoxCollider( unify::BBox< float > bbox )
	: ColliderBase( "BoxCollider" )
	, m_bbox{ bbox }
{
}

BoxCollider::~BoxCollider()
{
}

me::object::IObjectComponent::ptr BoxCollider::Duplicate()
{
	auto duplicate = new BoxCollider( *this );
	return me::object::IObjectComponent::ptr( duplicate );
}
