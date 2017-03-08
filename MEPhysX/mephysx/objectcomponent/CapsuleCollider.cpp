// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/objectcomponent/CapsuleCollider.h>

using namespace me;
using namespace scene;
using namespace mephysx;
using namespace physx;
using namespace objectcomponent;

CapsuleCollider::CapsuleCollider( CapsuleCollider & collider )
	: ColliderBase( collider )
{
}

CapsuleCollider::CapsuleCollider( me::IOS * os, GameComponent * gameComponent, float radius, float halfHeight )
: ColliderBase( os, gameComponent )
{
	m_shape.reset( gameComponent->GetPhysics()->createShape( PxCapsuleGeometry( radius, halfHeight ), *m_material ), Releaser< PxShape > );
}

CapsuleCollider::~CapsuleCollider()
{
}

std::string CapsuleCollider::GetType() const
{
	return "Capsule Collider";
}

std::string CapsuleCollider::GetWhat() const
{
	return std::string();
}

IObjectComponent * CapsuleCollider::Duplicate()
{
	auto duplicate = new CapsuleCollider( *this );
	return duplicate;
}
