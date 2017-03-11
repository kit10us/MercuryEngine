// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/objectcomponent/SphereCollider.h>

using namespace me;
using namespace mephysx;
using namespace physx;
using namespace objectcomponent;

SphereCollider::SphereCollider( mephysx::GameComponent * gameComponent, float radius )
: ColliderBase( "SphereCollider", gameComponent )
{
	m_shape.reset( gameComponent->GetPhysics()->createShape( PxSphereGeometry( radius ), *m_material ), Releaser< PxShape > );
}

SphereCollider::~SphereCollider()
{
}

std::string SphereCollider::GetWhat() const
{
	return std::string();
}

scene::IObjectComponent * SphereCollider::Duplicate()
{
	auto duplicate = new SphereCollider( *this );
	return duplicate;
}

