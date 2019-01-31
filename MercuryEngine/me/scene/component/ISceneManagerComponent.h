// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/scene/component/ISceneComponent.h>
#include <me/IComponent.h>

namespace me
{
	namespace scene
	{
		class SceneManager;
		class IScene;

		namespace component
		{
			/// <summary>
			/// A component for the scene manager.
			/// </summary>
			class ISceneManagerComponent : public IComponent
			{
			public:
				typedef std::shared_ptr< ISceneManagerComponent > ptr;

				virtual ~ISceneManagerComponent() {}

				/// <summary>
				/// Create a scene component. This allows end games and extensions from having to link against a specific
				/// extension.
				/// </summary>
				virtual ISceneComponent::ptr CreateSceneComponent( std::string type ) = 0;

				virtual void OnAttach( SceneManager * sceneManager ) = 0;
				virtual void OnDetach( SceneManager * sceneManager ) = 0;
				virtual void OnSceneStart( IScene * scene ) = 0;
				virtual void OnSceneEnd( IScene * from ) = 0;

				virtual SceneManager * GetSceneManager() = 0;
			};
		}
	}
}