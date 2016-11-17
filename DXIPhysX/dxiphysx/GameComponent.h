// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <me/IGameComponent.h>
#include <dxiphysx/Util.h>
#include <me/IOS.h>
#include <me/IGame.h>

namespace dxiphysx
{
	class GameComponent : public me::IGameComponent
	{
	public:
		GameComponent();
		~GameComponent();

		std::string GetName() const override;

		void OnAttach( me::IGame * game ) override;
		void OnDetach( me::IGame * game ) override;
		void OnUpdate( me::IGame * game, const me::IRenderer * renderer, const me::RenderInfo & renderInfo ) override;
		void OnRender( me::IGame * game, const me::IRenderer * renderer, const me::RenderInfo & renderInfo ) override;

		physx::PxPhysics * GetPhysics();
		physx::PxDefaultCpuDispatcher * GetCpuDispatcher();

	private:
		std::shared_ptr< physx::PxErrorCallback > m_errorCallback;
		physx::PxDefaultAllocator m_defaultAllocatorCallback;
		std::shared_ptr< physx::PxFoundation > m_foundation;
		physx::PxProfileZoneManager * m_profileZoneManager;
		std::shared_ptr< physx::PxPhysics > m_physics;
		physx::PxDefaultCpuDispatcher * m_dispatcher;
	};
}