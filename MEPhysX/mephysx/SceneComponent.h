// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <MEPhysX.h>
#include <mephysx/GameComponent.h>
#include <me/IOS.h>
#include <me/scene/SceneComponent.h>
#include <PxPhysicsAPI.h>
#include <mephysx/Releaser.h>

namespace mephysx
{
	class SceneComponent : public me::scene::SceneComponent
	{
	public:
		MEPHYSX_API SceneComponent( me::IOS * os, GameComponent * gameComponent );
		~SceneComponent();

		physx::PxScene * GetScene();
		physx::PxMaterial * GetMaterial();
		physx::PxControllerManager * GetControllerManager();

	public: // ISceneComponent...
		void OnAttach( me::scene::Scene * scene ) override;
		void OnDetach( me::scene::Scene * scene ) override;
		void OnInit( me::scene::Scene * scene ) override;
		void OnStart( me::scene::Scene * scene ) override;
		void OnUpdate( me::scene::Scene * scene, me::UpdateParams params ) override;
		void OnRender( me::scene::Scene * scene, me::scene::RenderGirl & renderGirl ) override;
		void OnSuspend() override;
		void OnResume() override;

	public: // IComponent...
		std::string GetWhat() const override;
		
	private:
		GameComponent * m_gameComponent;
		std::shared_ptr< physx::PxScene > m_scene;
		std::shared_ptr< physx::PxMaterial > m_material;
		std::shared_ptr< physx::PxControllerManager > m_cctManager;
	};
}