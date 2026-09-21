// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/scene/component/ISceneComponent.h>
#include <me/IComponent.h>
#include <unify/Result.h>

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

				/// <summary>
				/// Attach to the SceneManager.
				/// </summary>
				virtual unify::Result<> OnAttach( SceneManager * sceneManager ) = 0;
				
				/// <summary>
				/// Detach from the SceneManager.
				/// </summary>
				virtual unify::Result<> OnDetach( SceneManager * sceneManager ) = 0;
				
				/// <summary>
				/// Triggered before a Scene begins.
				/// </summary>
				virtual unify::Result<> OnSceneStart( IScene * scene ) = 0;

				/// <summary>
				/// Triggered adfter a Scene ends.
				/// </summary>
				/// <param name="from"></param>
				/// <returns></returns>
				virtual unify::Result<> OnSceneEnd( IScene * from ) = 0;

				/// <summary>
				/// If currently attached, returns the scene manager.
				/// </summary>
				virtual SceneManager * GetSceneManager() = 0;
			};
		}
	}
}