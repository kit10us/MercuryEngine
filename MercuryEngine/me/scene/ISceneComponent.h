// Copyright (c) 2002 - 2013, Quentin S. Smith
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

			virtual void OnEnterScene( me::scene::IScene * previous ) = 0;
			
			virtual void OnLeaveScene( me::scene::IScene * next ) = 0;

			virtual void OnBeforeStart() = 0;

			virtual void OnAfterStart() = 0;

			/// <summary>
			/// Called every game update cycle.
			/// </summary>
			virtual void OnUpdate( UpdateParams params ) = 0;


			virtual void CollectCameras( RenderGirl & renderGirl ) = 0;
			//void GrowableObjectStack::CollectRendering( RenderParams params, const FinalCamera & camera, GeometryCacheSummation & summation )
			virtual void OnRender( RenderGirl & rendergirl ) = 0;

			/// <summary>
			/// Called when we get a suspend request. One more Render is called to allow updating based on becoming suspend (suspended graphics).
			/// </summary>
			virtual void OnSuspend() = 0;

			/// <summary>
			/// Called when resuming from suspending.
			/// </summary>
			virtual void OnResume() = 0;
		};
	}
}