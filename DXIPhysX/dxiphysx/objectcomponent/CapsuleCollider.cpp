// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/objectcomponent/CapsuleCollider.h>

using namespace dxi;
using namespace dxiphysx;
using namespace physx;
using namespace objectcomponent;

CapsuleCollider::CapsuleCollider( core::IOS * os, physx::PxPhysics * physics, float radius, float halfHeight )
: ColliderBase( os, physics )
{
	m_shape.reset( physics->createShape( PxCapsuleGeometry( radius, halfHeight ), *m_material ), Releaser< PxShape > );
}

CapsuleCollider::~CapsuleCollider()
{
}

std::string CapsuleCollider::GetName() const
{
	return "Capsule Collider";
}
