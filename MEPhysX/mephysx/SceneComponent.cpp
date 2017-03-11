// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/SceneComponent.h>
#include <me/scene/Scene.h>

#include <mephysx/objectcomponent/RigidBody.h>
#include <mephysx/objectcomponent/BoxCollider.h>
#include <mephysx/objectcomponent/SphereCollider.h>
#include <mephysx/Util.h>

using namespace mephysx;
using namespace me;
using namespace physx;
		 
MEPHYSX_API SceneComponent::SceneComponent( me::IOS * os, mephysx::GameComponent * gameComponent )
	: scene::SceneComponent( os, "PhysXScene" )
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

	// Create character controller (CCT) manager...
	m_cctManager.reset( PxCreateControllerManager( *m_scene.get() ), Releaser< PxControllerManager > );
}

SceneComponent::~SceneComponent()
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

physx::PxControllerManager * SceneComponent::GetControllerManager()
{
	return m_cctManager.get();
}

void SceneComponent::OnAttach( me::scene::Scene * scene )
{
}

void SceneComponent::OnDetach( me::scene::Scene * scene )
{
}

void SceneComponent::OnInit( me::scene::Scene * scene )
{
}

void SceneComponent::OnStart( me::scene::Scene * scene )
{
	using namespace scene;

	auto physics = m_gameComponent->GetPhysics();

	{
		Object * plane = scene->FindObject( "Plane" );
		
		PxRigidStatic* body = PxCreatePlane( *physics, util::Convert< PxPlane >( plane->GetFrame().GetMatrix() ), *m_material );
		body->userData = plane;
		m_scene->addActor( *body );

		//mephysx::ObjectComponent * oc = new mephysx::ObjectComponent( m_os, "PhysX", body );
		//plane->AddComponent( scene::IObjectComponent::ptr( oc ) );
	}

	auto player = scene->FindObject( "player" );

	int x(0);x;
}

void SceneComponent::OnUpdate( me::scene::Scene * scene, UpdateParams params )
{			
	using namespace scene;

	float elapsedTime = params.GetDelta();

	/*
	// NOTE: This might speed things up, but causes physx warnings (simulate calls overlapping)
	const float maxElapsed = 0.05f;
	while( elapsedTime > maxElapsed )
	{
		m_scene->simulate( maxElapsed );
		elapsedTime -= maxElapsed;
	}
	*/

	m_scene->simulate( elapsedTime );	  

	m_scene->fetchResults( true );

	// Sync physics to geometry...
	PxU32 nbActiveTransforms;
	const PxActiveTransform* activeTransforms = m_scene->getActiveTransforms( nbActiveTransforms );	
	for ( PxU32 i = 0; i < nbActiveTransforms; ++i )
	{
		Object * object = static_cast< Object * >(activeTransforms[i].userData);
		object->GetFrame().SetPosition( util::Convert< unify::V3< float > >( activeTransforms[i].actor2World.p ) );
		object->GetFrame().SetRotation( util::Convert< unify::Quaternion >( activeTransforms[i].actor2World.q ) );
	}


	/*
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
	*/
}

void SceneComponent::OnRender( me::scene::Scene * scene, RenderParams params )
{
}

void SceneComponent::OnSuspend()
{
}

void SceneComponent::OnResume()
{
}

std::string SceneComponent::GetWhat() const
{
	return std::string();
}


/*
std::string SceneComponent::GetType() const
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
*/
