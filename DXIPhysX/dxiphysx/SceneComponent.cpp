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

class ErrorCallback : public PxErrorCallback
{
public:
	ErrorCallback( core::IOS * os )
		: m_os( os )
	{
	}
	
	~ErrorCallback()
	{
	}

	void reportError( PxErrorCode::Enum code, const char* message, const char* file, int line ) override
	{
		m_os->DebugWriteLine( std::string( "PhysX error: " ) + message );
	}

private:
	core::IOS * m_os;
};


SceneComponent::SceneComponent( core::IOS * os )
	: m_os( os )
	, m_enabled( true )
{
	m_errorCallback.reset( new ErrorCallback( os ) );
	m_foundation.reset( PxCreateFoundation( PX_PHYSICS_VERSION, m_defaultAllocatorCallback, *m_errorCallback ), Releaser< PxFoundation > );
	if ( ! m_foundation )
	{
		//fatalError( "PxCreateFoundation failed!" );
		OutputDebugStringA( "Failed to create Foundation!" );
		assert( 0 );
	}

	bool recordMemoryAllocations = true;
	m_profileZoneManager = &PxProfileZoneManager::createProfileZoneManager( m_foundation.get() );
	if ( !m_profileZoneManager )
	{
		OutputDebugStringA( "PxProfileZoneManager::createProfileZoneManager failed!" );
		assert( 0 );
	}

	m_physics.reset( PxCreatePhysics( PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale(), recordMemoryAllocations ), Releaser< PxPhysics > );
	if ( !m_physics )
	{
		OutputDebugStringA( "PxCreatePhysics failed!" );
		assert( 0 );
	}

	m_displatcher = PxDefaultCpuDispatcherCreate( 2 );

	PxSceneDesc sceneDesc( m_physics->getTolerancesScale() );
	sceneDesc.gravity = PxVec3( 0.0f, -9.81f, 0.0f );
	sceneDesc.cpuDispatcher = m_displatcher;
	sceneDesc.filterShader = PxDefaultSimulationFilterShader;
	sceneDesc.flags |= PxSceneFlag::eENABLE_ACTIVETRANSFORMS;

	m_scene.reset( m_physics->createScene( sceneDesc ), Releaser< PxScene > );
	if( ! m_scene )
	{
		OutputDebugStringA( "Failed to create PhysX scene!" );
		assert( 0 );
	}

	m_material.reset( m_physics->createMaterial( 0.5f, 0.5f, 0.6f ), Releaser< PxMaterial > );
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
	scene::IObjectComponent::ptr boxCollider( new objectcomponent::BoxCollider( m_os, m_physics.get(), unify::V3< float >( 1.0f, 1.0f, 1.0f ) ) );
	scene::IObjectComponent::ptr sphereCollider( new objectcomponent::SphereCollider( m_os, m_physics.get(), 1.0f ) );

	for( auto object : scene->GetRoot()->AllChildren( true ) )
	{
		if ( object->HasTag( "cube" ) )
		{
			// Create component.
			objectcomponent::RigidBody * rbc = new objectcomponent::RigidBody( m_os, m_physics.get() );

			// Add to object, which will trigger it's attach and actually create the RigidBody.
			object->AddComponent( scene::IObjectComponent::ptr( rbc ) );
			
			// Attach shape to body.
			object->AddComponent( boxCollider );

			// Update body.
			PxRigidBodyExt::updateMassAndInertia( *rbc->GetRigidBody(), 10.0f );

			// Add body to scene.
			m_scene->addActor( *rbc->GetRigidBody() );
		}
		else if ( object->HasTag( "sphere" ) )
		{
			objectcomponent::RigidBody * rbc = new objectcomponent::RigidBody( m_os, m_physics.get() );
			object->AddComponent( scene::IObjectComponent::ptr( rbc ) );
			object->AddComponent( sphereCollider );
			PxRigidBodyExt::updateMassAndInertia( *rbc->GetRigidBody(), 10.0f );
			m_scene->addActor( *rbc->GetRigidBody() );
		}
	}

	//cube->release();

	{
		scene::Object::ptr plane = scene->FindObject( "Plane" );
		PxRigidStatic* body = PxCreatePlane( *m_physics, util::Convert< PxPlane >( plane->GetFrame().GetMatrix() ), *m_material );
		body->userData = plane.get();
		m_scene->addActor( *body );

		//dxiphysx::ObjectComponent * oc = new dxiphysx::ObjectComponent( m_os, "PhysX", body );
		//plane->AddComponent( scene::IObjectComponent::ptr( oc ) );
	}
}

void SceneComponent::OnUpdate( dxi::scene::Scene * scene, const RenderInfo & renderInfo )
{											
	static unify::MinMax< float > deltaRange;
	deltaRange += renderInfo.GetDelta();

	float elapsedTime = 0.035f; // renderInfo.GetDelta();
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
	if ( rationMoving < 0.30f )
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
