// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/objectcomponent/SphereCollider.h>

using namespace me;
using namespace scene;
using namespace mephysx;
using namespace physx;
using namespace objectcomponent;

SphereCollider::SphereCollider( me::IOS * os, GameComponent * gameComponent, float radius )
: ColliderBase( os, gameComponent )
{
	m_shape.reset( gameComponent->GetPhysics()->createShape( PxSphereGeometry( radius ), *m_material ), Releaser< PxShape > );
}

SphereCollider::~SphereCollider()
{
}

std::string SphereCollider::GetType() const
{
	return "Sphere Collider";
}

std::string SphereCollider::GetWhat() const
{
	return std::string();
}

IObjectComponent * SphereCollider::Duplicate()
{
	auto duplicate = new SphereCollider( *this );
	return duplicate;
}

