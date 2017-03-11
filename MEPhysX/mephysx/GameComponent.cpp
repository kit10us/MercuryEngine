// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#include <mephysx/GameComponent.h>

using namespace mephysx;
using namespace physx;

class ErrorCallback : public PxErrorCallback
{
public:
	ErrorCallback( me::IOS * os )
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
	me::IOS * m_os;
};

GameComponent::GameComponent()
	: me::GameComponent( "PhysX" )
{
}

GameComponent::~GameComponent()
{
}

physx::PxPhysics * GameComponent::GetPhysics()
{
	return m_physics.get();
}

physx::PxDefaultCpuDispatcher * GameComponent::GetCpuDispatcher()
{
	return m_dispatcher;
}

void GameComponent::OnAttach( me::IGame * game )
{
	me::GameComponent::OnAttach( game );

	using namespace me;

	m_errorCallback.reset( new ErrorCallback( game->GetOS() ) );
	m_foundation.reset( PxCreateFoundation( PX_PHYSICS_VERSION, m_defaultAllocatorCallback, *m_errorCallback ), Releaser< PxFoundation > );
	if ( !m_foundation )
	{
		//fatalError( "PxCreateFoundation failed!" );
		game->ReportError( ErrorLevel::Failure,  "PhysX", "Failed to create Foundation!" );
		return;
	}

	bool recordMemoryAllocations = true;
	m_profileZoneManager = &PxProfileZoneManager::createProfileZoneManager( m_foundation.get() );
	if ( !m_profileZoneManager )
	{
		game->ReportError( ErrorLevel::Failure, "PhysX", "PxProfileZoneManager::createProfileZoneManager failed!" );
		return;
	}

	m_physics.reset( PxCreatePhysics( PX_PHYSICS_VERSION, *m_foundation, PxTolerancesScale(), recordMemoryAllocations ), Releaser< PxPhysics > );
	if ( !m_physics )
	{
		game->ReportError( ErrorLevel::Failure, "PhysX", "PxCreatePhysics failed!" );
		return;
	}

	m_dispatcher = PxDefaultCpuDispatcherCreate( 2 );
}

void GameComponent::OnDetach( me::IGame * game )
{
	me::GameComponent::OnDetach( game );
}

std::string GameComponent::GetWhat() const
{
	return std::string();
}
