// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#include <mephy/phy3d/SceneComponent3D.h>

using namespace mephy;
using namespace phy3d;

char* SceneComponent::Name()
{
	return "MEPhysicsSceneComponent3D";
}

SceneComponent::SceneComponent( me::os::IOS * os )
	: me::scene::SceneComponent( os, Name() )
{
}

SceneComponent::~SceneComponent()
{																	 
}

void SceneComponent::AddCollider( collider::ColliderBase::ptr collider )
{
	m_colliders.push_back( collider );
}

void SceneComponent::CleanTheDead()
{
	for( auto itr = m_colliders.begin(); itr != m_colliders.end(); )
	{
		if( itr->expired() || itr->lock().get()->GetObject() == nullptr )
		{
			itr = m_colliders.erase( itr );
		}
		else
		{
			itr++;
		}
	}
}

void SceneComponent::OnEarlyUpdate( const me::UpdateParams & params )
{
	CleanTheDead();
	for( auto component : m_colliders )
	{
		auto * object = component.lock()->GetObject();
		assert( object );

		auto * collider = unify::polymorphic_downcast< collider::ColliderBase * >( component.lock().get() );
		collider->CaptureEarly();
	}
}

void SceneComponent::OnLateUpdate( const me::UpdateParams & params )
{
	CleanTheDead();
	// Very basic.
	for( auto colliderComponentA : m_colliders )
	{
		auto * colliderA = unify::polymorphic_downcast< collider::ColliderBase * >( colliderComponentA.lock().get() );
		assert( colliderA );

		for( auto colliderComponentB : m_colliders )
		{
			auto * colliderB = unify::polymorphic_downcast< collider::ColliderBase * >( colliderComponentB.lock().get() );
			assert( colliderB );
			colliderA->TestCollision( colliderB, params );
		}
	}
}
