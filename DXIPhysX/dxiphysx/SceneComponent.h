// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/core/IOS.h>
#include <dxi/scene/ISceneComponent.h>
#include <PxPhysicsAPI.h>
#include <dxiphysx/Releaser.h>

namespace dxiphysx
{
	class SceneComponent : public dxi::scene::ISceneComponent
	{
	public:
		SceneComponent( dxi::core::IOS * os );
		~SceneComponent();

		const char * GetName() const override;
		bool IsEnabled() const override;
		void SetEnabled( bool enabled ) override;

		void OnAttach( dxi::scene::Scene * scene ) override;
		void OnDetach( dxi::scene::Scene * scene ) override;
		void OnInit( dxi::scene::Scene * scene ) override;
		void OnStart( dxi::scene::Scene * scene ) override;
		void OnUpdate( dxi::scene::Scene * scene, const dxi::RenderInfo & renderInfo ) override;
		void OnRender( dxi::scene::Scene * scene, const dxi::RenderInfo & renderInfo ) override;
		void OnSuspend() override;
		void OnResume() override;

	private:
		dxi::core::IOS * m_os;
		bool m_enabled;

		std::shared_ptr< physx::PxErrorCallback > m_errorCallback;
		physx::PxDefaultAllocator m_defaultAllocatorCallback;
		std::shared_ptr< physx::PxFoundation > m_foundation;
		physx::PxProfileZoneManager * m_profileZoneManager;
		std::shared_ptr< physx::PxPhysics > m_physics;		
		physx::PxDefaultCpuDispatcher * m_displatcher;
		std::shared_ptr< physx::PxScene > m_scene;
		std::shared_ptr< physx::PxMaterial > m_material;
	};
}