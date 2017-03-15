// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/objectcomponent/CapsuleCollider.h>

using namespace me;
using namespace object;
using namespace mephysx;
using namespace physx;
using namespace objectcomponent;

CapsuleCollider::CapsuleCollider( CapsuleCollider & collider )
	: ColliderBase( collider )
{
}

CapsuleCollider::CapsuleCollider( mephysx::GameComponent * gameComponent, float radius, float halfHeight )
: ColliderBase( "CapsuleCollider", gameComponent )
{
	m_shape.reset( gameComponent->GetPhysics()->createShape( PxCapsuleGeometry( radius, halfHeight ), *m_material ), Releaser< PxShape > );
}

CapsuleCollider::~CapsuleCollider()
{
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
