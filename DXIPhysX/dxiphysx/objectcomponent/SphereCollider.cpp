// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/objectcomponent/SphereCollider.h>

using namespace dxi;
using namespace dxiphysx;
using namespace physx;
using namespace objectcomponent;

SphereCollider::SphereCollider( core::IOS * os, GameComponent * gameComponent, float radius )
: ColliderBase( os, gameComponent )
{
	m_shape.reset( gameComponent->GetPhysics()->createShape( PxSphereGeometry( radius ), *m_material ), Releaser< PxShape > );
}

SphereCollider::~SphereCollider()
{
}

std::string SphereCollider::GetName() const
{
	return "Sphere Collider";
}

dxi::scene::IObjectComponent * SphereCollider::Duplicate()
{
	auto duplicate = new SphereCollider( *this );
	return duplicate;
}

