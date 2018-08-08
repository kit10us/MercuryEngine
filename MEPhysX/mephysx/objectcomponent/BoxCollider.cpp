// Copyright (c) 2002 - 2013, Evil Quail LLC
// All Rights Reserved

#include <mephysx/objectcomponent/BoxCollider.h>

using namespace me;
using namespace object;
using namespace mephysx;
using namespace physx;
using namespace objectcomponent;

BoxCollider::BoxCollider( BoxCollider & collider )
	: ColliderBase( collider )
{
}
 
BoxCollider::BoxCollider( mephysx::GameComponent * gameComponent, unify::V3< float > halfExt )
: ColliderBase( "BoxCollider", gameComponent )
{
	m_shape.reset( gameComponent->GetPhysics()->createShape( PxBoxGeometry( PxVec3( halfExt.x, halfExt.y, halfExt.z ) ), *m_material ), Releaser< PxShape > );
}

BoxCollider::~BoxCollider()
{
}

std::string BoxCollider::GetWhat() const
{
	return std::string();
}

IObjectComponent::ptr BoxCollider::Duplicate()
{
	auto duplicate = new BoxCollider( *this );
	return object::IObjectComponent::ptr( duplicate );
}
