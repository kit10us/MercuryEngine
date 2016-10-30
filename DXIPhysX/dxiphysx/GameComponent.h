// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/core/IGameComponent.h>
#include <dxiphysx/Util.h>
#include <dxi/core/IOS.h>
#include <dxi/core/IGame.h>

namespace dxiphysx
{
	class GameComponent : public dxi::core::IGameComponent
	{
	public:
		GameComponent();
		~GameComponent();

		std::string GetName() const override;

		void OnAttach( dxi::core::IGame * game ) override;
		void OnDetach( dxi::core::IGame * game ) override;
		void OnUpdate( dxi::core::IGame * game, const dxi::RenderInfo & renderInfo ) override;
		void OnRender( dxi::core::IGame * game, const dxi::RenderInfo & renderInfo ) override;

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