// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <memory>

namespace me
{
	namespace scene
	{
		class SceneManager;
		class IScene;

		class ISceneManagerComponent
		{
		public:
			typedef std::shared_ptr< ISceneManagerComponent > ptr;

			virtual ~ISceneManagerComponent() {}

			virtual void OnAttach( SceneManager * sceneManager ) = 0;
			virtual void OnDetach( SceneManager * sceneManager ) = 0;
			virtual void OnSceneAdded( IScene * scene ) = 0;
			virtual void OnSceneChange( IScene * from, IScene * to ) = 0;
		};
	}
}