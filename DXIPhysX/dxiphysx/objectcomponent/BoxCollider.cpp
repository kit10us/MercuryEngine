// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/objectcomponent/BoxCollider.h>

using namespace dxi;
using namespace dxiphysx;
using namespace physx;
using namespace objectcomponent;

BoxCollider::BoxCollider( core::IOS * os, physx::PxPhysics * physics, unify::V3< float > halfExt )
: ColliderBase( os, physics )
{
	m_shape.reset( m_physics->createShape( PxBoxGeometry( PxVec3( halfExt.x, halfExt.y, halfExt.z ) ), *m_material ), Releaser< PxShape > );

}

BoxCollider::~BoxCollider()
{
}

std::string BoxCollider::GetName() const
{
	return "Box Collider";
}
