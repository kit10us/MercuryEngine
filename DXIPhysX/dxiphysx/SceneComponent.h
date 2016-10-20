// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <DXIPhysX.h>
#include <dxiphysx/GameComponent.h>
#include <dxi/core/IOS.h>
#include <dxi/scene/ISceneComponent.h>
#include <PxPhysicsAPI.h>
#include <dxiphysx/Releaser.h>

namespace dxiphysx
{
	class SceneComponent : public dxi::scene::ISceneComponent
	{
	public:
		DXIPHYSX_API SceneComponent( dxi::core::IOS * os, GameComponent * gameComponent );
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

		physx::PxScene * GetScene();
		physx::PxMaterial * GetMaterial();

	private:
		dxi::core::IOS * m_os;
		bool m_enabled;

		GameComponent * m_gameComponent;
		std::shared_ptr< physx::PxScene > m_scene;
		std::shared_ptr< physx::PxMaterial > m_material;
	};
}