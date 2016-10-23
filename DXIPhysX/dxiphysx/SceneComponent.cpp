// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxiphysx/SceneComponent.h>
#include <dxi/scene/Scene.h>

#include <dxiphysx/objectcomponent/RigidBody.h>
#include <dxiphysx/objectcomponent/BoxCollider.h>
#include <dxiphysx/objectcomponent/SphereCollider.h>
#include <dxiphysx/Util.h>

using namespace dxiphysx;
using namespace dxi;
using namespace physx;
		 
DXIPHYSX_API SceneComponent::SceneComponent( core::IOS * os, GameComponent * gameComponent )
	: m_os( os )
	, m_enabled( true )
	, m_gameComponent( gameComponent )
{
	PxSceneDesc sceneDesc( m_gameComponent->GetPhysics()->getTolerancesScale() );
	sceneDesc.gravity = PxVec3( 0.0f, -9.81f, 0.0f );
	sceneDesc.cpuDispatcher = m_gameComponent->GetCpuDispatcher();
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVETRANSFORMS;

	m_scene.reset( m_gameComponent->GetPhysics()->createScene( sceneDesc ), Releaser< PxScene > );
	if( ! m_scene )
	{
		OutputDebugStringA( "Failed to create PhysX scene!" );
		assert( 0 );
	}

	m_material.reset( m_gameComponent->GetPhysics()->createMaterial( 0.5f, 0.5f, 0.6f ), Releaser< PxMaterial > );
}

SceneComponent::~SceneComponent()
{
}

const char * SceneComponent::GetName() const
{
	return "PhysXScene";
}

bool SceneComponent::IsEnabled() const
{
	return m_enabled;
}

void SceneComponent::SetEnabled( bool enabled )
{
	m_enabled = enabled;
}

void SceneComponent::OnAttach( dxi::scene::Scene * scene )
{
}

void SceneComponent::OnDetach( dxi::scene::Scene * scene )
{
}

void SceneComponent::OnInit( dxi::scene::Scene * scene )
{
}

void SceneComponent::OnStart( dxi::scene::Scene * scene )
{
	auto physics = m_gameComponent->GetPhysics();

	{
		scene::Object::ptr plane = scene->FindObject( "Plane" );
		
		PxRigidStatic* body = PxCreatePlane( *physics, util::Convert< PxPlane >( plane->GetFrame().GetMatrix() ), *m_material );
		body->userData = plane.get();
		m_scene->addActor( *body );

		//dxiphysx::ObjectComponent * oc = new dxiphysx::ObjectComponent( m_os, "PhysX", body );
		//plane->AddComponent( scene::IObjectComponent::ptr( oc ) );
	}
}

void SceneComponent::OnUpdate( dxi::scene::Scene * scene, const RenderInfo & renderInfo )
{											
	float elapsedTime = renderInfo.GetDelta();

	const float maxElapsed = 0.05f;
	while( elapsedTime > maxElapsed )
	{
		m_scene->simulate( maxElapsed );
		elapsedTime -= maxElapsed;
	}
	m_scene->simulate( elapsedTime );	  

	m_scene->fetchResults( true );

	PxU32 nbActiveTransforms;
	const PxActiveTransform* activeTransforms = m_scene->getActiveTransforms( nbActiveTransforms );

	for ( PxU32 i = 0; i < nbActiveTransforms; ++i )
	{
		scene::Object * object = static_cast<scene::Object*>(activeTransforms[i].userData);
		physx::PxMat44 mat( activeTransforms[i].actor2World );
		object->GetFrame().SetMatrix( *(unify::Matrix*)&mat );
	}

	PxU32 actorCount = m_scene->getNbActors( PxActorTypeFlag::eRIGID_DYNAMIC );

	float rationMoving = (float)nbActiveTransforms / (float)actorCount;
	if ( rationMoving < 0.70f )
	{
		const PxU32 maxActors = 2000;
		PxRigidDynamic * actors[maxActors];
		m_scene->getActors( PxActorTypeFlag::eRIGID_DYNAMIC, (PxActor**)&actors, maxActors, 0 );
		for ( PxU32 i = 0; i < actorCount; ++i )
		{
			actors[i]->setLinearVelocity( 
				PxVec3( 
					0,
					i * 0.1f, 
					0 ) );
		}
	}
}

void SceneComponent::OnRender( dxi::scene::Scene * scene, const RenderInfo & renderInfo )
{
}

void SceneComponent::OnSuspend()
{
}

void SceneComponent::OnResume()
{
}

physx::PxScene * SceneComponent::GetScene()
{
	return m_scene.get();
}

physx::PxMaterial * SceneComponent::GetMaterial()
{
	return m_material.get();
}
