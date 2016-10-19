// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/objectcomponent/SphereCollider.h>

using namespace dxi;
using namespace dxiphysx;
using namespace physx;
using namespace objectcomponent;

SphereCollider::SphereCollider( core::IOS * os, physx::PxPhysics * physics, float radius )
: ColliderBase( os, physics )
{
	m_shape.reset( physics->createShape( PxSphereGeometry( radius ), *m_material ), Releaser< PxShape > );
}

SphereCollider::~SphereCollider()
{
}

std::string SphereCollider::GetName() const
{
	return "Sphere Collider";
}
