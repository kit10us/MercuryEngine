// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IOS.h>
#include <me/scene/ISceneManagerComponent.h>
#include <PxPhysicsAPI.h>

namespace dxiphysx
{
	class SceneManagerComponent : public me::scene::ISceneManagerComponent
	{
	public:
		SceneManagerComponent( me::IOS * os );
		~SceneManagerComponent();

		const char * GetName() const override;

		void OnNewScene( me::scene::Scene * scene ) override;

	private:
		me::IOS * m_os;
	};
}