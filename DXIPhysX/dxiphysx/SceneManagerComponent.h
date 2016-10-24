// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/core/IOS.h>
#include <dxi/scene/ISceneManagerComponent.h>
#include <PxPhysicsAPI.h>

namespace dxiphysx
{
	class SceneManagerComponent : public dxi::scene::ISceneManagerComponent
	{
	public:
		SceneManagerComponent( dxi::core::IOS * os );
		~SceneManagerComponent();

		const char * GetName() const override;

		void OnNewScene( dxi::scene::Scene * scene ) override;

	private:
		dxi::core::IOS * m_os;
	};
}