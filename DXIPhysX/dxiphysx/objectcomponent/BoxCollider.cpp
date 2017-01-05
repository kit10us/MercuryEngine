// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/objectcomponent/BoxCollider.h>

using namespace me;
using namespace scene;
using namespace dxiphysx;
using namespace physx;
using namespace objectcomponent;

BoxCollider::BoxCollider( BoxCollider & collider )
	: ColliderBase( collider )
{
}


BoxCollider::BoxCollider( me::IOS * os, GameComponent * gameComponent, unify::V3< float > halfExt )
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

IObjectComponent * BoxCollider::Duplicate()
{
	auto duplicate = new BoxCollider( *this );
	return duplicate;
}
