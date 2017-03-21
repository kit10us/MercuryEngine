// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/ISceneManagerComponent.h>
#include <me/scene/SceneManager.h>

namespace me
{
	namespace scene
	{
		class SceneManagerComponent : public ISceneManagerComponent
		{
		public:
			void OnAttach( SceneManager * sceneManager ) override;
			void OnDetach( SceneManager * sceneManager ) override;
			void OnSceneStart( IScene * scene ) override;
			void OnSceneEnd( IScene * from ) override;
		};
	}
}