// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <DXIPhysX.h>
#include <dxiphysx/GameComponent.h>
#include <me/IOS.h>
#include <me/scene/ISceneComponent.h>
#include <PxPhysicsAPI.h>
#include <dxiphysx/Releaser.h>

namespace dxiphysx
{
	class SceneComponent : public me::scene::ISceneComponent
	{
	public:
		DXIPHYSX_API SceneComponent( me::IOS * os, GameComponent * gameComponent );
		~SceneComponent();

		const char * GetName() const override;
		bool IsEnabled() const override;
		void SetEnabled( bool enabled ) override;

		void OnAttach( me::scene::Scene * scene ) override;
		void OnDetach( me::scene::Scene * scene ) override;
		void OnInit( me::scene::Scene * scene ) override;
		void OnStart( me::scene::Scene * scene ) override;
		void OnUpdate( me::scene::Scene * scene, me::UpdateParams params ) override;
		void OnRender( me::scene::Scene * scene, me::RenderParams params ) override;
		void OnSuspend() override;
		void OnResume() override;

		physx::PxScene * GetScene();
		physx::PxMaterial * GetMaterial();
		physx::PxControllerManager * GetControllerManager();

	private:
		me::IOS * m_os;
		bool m_enabled;

		GameComponent * m_gameComponent;
		std::shared_ptr< physx::PxScene > m_scene;
		std::shared_ptr< physx::PxMaterial > m_material;
		std::shared_ptr< physx::PxControllerManager > m_cctManager;
	};
}