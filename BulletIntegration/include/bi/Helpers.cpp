// Copyright (c) 2003 - 2011, Evil Quail LLC
// All Rights Reserved

#include <bi/Helpers.h>


btRigidBody * bi::GetRigidBody( dxi::scene::Object * object )
{
	if( ! object )
	{
		return 0;
	}

	bi::Instance * instance = dynamic_cast< bi::Instance * >( object->GetPhysics().get() );
	if( ! instance )
	{
		return 0;
	}

	return instance->GetRigidBody();
}

const btRigidBody * bi::GetRigidBody( const dxi::scene::Object * object )
{
	if( ! object )
	{
		return 0;
	}

	const bi::Instance * instance = dynamic_cast< const bi::Instance * >( object->GetPhysics().get() );
	if( ! instance )
	{
		return 0;
	}

	return instance->GetRigidBody();
}

btDiscreteDynamicsWorld * bi::GetWorld( dxi::scene::Scene * scene )
{
	bi::Scene * biScene = dynamic_cast< bi::Scene * >( scene->GetPhysicsScene() );
	if( ! biScene )
	{
		return 0;
	}
	return biScene->GetWorld();
}

const btDiscreteDynamicsWorld *  bi::GetWorld( const dxi::scene::Scene * scene )
{
	const bi::Scene * biScene = dynamic_cast< const bi::Scene * >( scene->GetPhysicsScene() );
	if( ! biScene )
	{
		return 0;
	}
	return biScene->GetWorld();
}
