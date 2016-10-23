// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/objectcomponent/BoxCollider.h>

using namespace dxi;
using namespace dxiphysx;
using namespace physx;
using namespace objectcomponent;

BoxCollider::BoxCollider( core::IOS * os, GameComponent * gameComponent, unify::V3< float > halfExt )
: ColliderBase( os, gameComponent )
{
	m_shape.reset( gameComponent->GetPhysics()->createShape( PxBoxGeometry( PxVec3( halfExt.x, halfExt.y, halfExt.z ) ), *m_material ), Releaser< PxShape > );

}

BoxCollider::~BoxCollider()
{
}

std::string BoxCollider::GetName() const
{
	return "Box Collider";
}