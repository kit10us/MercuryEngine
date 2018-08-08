// Copyright (c) 2002 - 2013, Evil Quail LLC
// All Rights Reserved
#pragma once

#include <me/GameComponent.h>
#include <mephysx/Util.h>
#include <me/IOS.h>
#include <me/IGame.h>

namespace mephysx
{
	class GameComponent : public me::GameComponent
	{
	public:
		GameComponent();
		~GameComponent();

		physx::PxPhysics * GetPhysics();
		physx::PxDefaultCpuDispatcher * GetCpuDispatcher();

	public: // IGameComponent...
		void OnAttach( me::IGame * game ) override;
		void OnDetach( me::IGame * game ) override;

	public: // IComponent...
		std::string GetWhat() const;

	private:
		std::shared_ptr< physx::PxErrorCallback > m_errorCallback;
		physx::PxDefaultAllocator m_defaultAllocatorCallback;
		std::shared_ptr< physx::PxFoundation > m_foundation;
#ifdef _DEBUG
		physx::PxProfileZoneManager * m_profileZoneManager;
#endif
		std::shared_ptr< physx::PxPhysics > m_physics;
		physx::PxDefaultCpuDispatcher * m_dispatcher;
	};
}