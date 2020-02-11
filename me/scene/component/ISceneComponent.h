// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/IComponent.h>
#include <me/UpdateParams.h>
#include <me/scene/RenderGirl.h>
#include <string>
#include <memory>

namespace me
{
	namespace scene
	{
		class IScene;

		namespace component
		{
			class ISceneComponent : public IComponent
			{
			public:
				typedef std::shared_ptr< ISceneComponent > ptr;

				virtual ~ISceneComponent() {}

				/// <summary>
				/// Called once, when we are first attached to the scene.
				/// </summary>
				virtual void OnAttach( IScene * scene ) = 0;

				/// <summary>
				/// Called once, when we are detached from the scene.
				/// </summary>
				virtual void OnDetach( IScene * scene ) = 0;			 

				virtual void OnBeforeStart() = 0;

				virtual void OnAfterStart() = 0;

				/// <summary>
				/// Called immediately before OnUpdate.
				/// This is where we apply pre-update affects, those that are automatic, that OnUpdate will likely respond to.
				/// </summary>
				virtual void OnEarlyUpdate( const UpdateParams & params ) = 0;

				/// <summary>
				/// Called every game update cycle.
				/// This is where users enteract with objecsts and components.
				/// </summary>
				virtual void OnUpdate( const UpdateParams & params ) = 0;

				/// <summary>
				/// Called immediately after OnUpdate.
				/// This is where we apply post-update affects, those that are automatic, however, that respond to OnUpdate (user update) changes.
				/// </summary>
				virtual void OnLateUpdate( const UpdateParams & params ) = 0;

				virtual void CollectCameras( RenderGirl & renderGirl ) = 0;
				//void GrowableObjectStack::CollectRendering( render::Params params, const FinalCamera & camera, GeometryCacheSummation & summation )
				virtual void OnRender( RenderGirl & rendergirl ) = 0;

				/// <summary>
				/// Called when we get a suspend request. One more Render is called to allow updating based on becoming suspend (suspended graphics).
				/// </summary>
				virtual void OnSuspend() = 0;

				/// <summary>
				/// Called when resuming from suspending.
				/// </summary>
				virtual void OnResume() = 0;

				/// <summary>
				/// Called when our scene ends.
				/// </summary>
				virtual void OnEnd() = 0;
			};
		}
	}
}