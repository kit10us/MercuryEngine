// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/RenderInfo.h>

#include <string>
#include <memory>

namespace me
{
	class Scene;
	class Object;

	class ISceneComponent
	{
	public:
		typedef std::shared_ptr< ISceneComponent > ptr;

		virtual ~ISceneComponent() {}

		virtual const char * GetName() const = 0;

		virtual bool IsEnabled() const = 0;
		virtual void SetEnabled( bool enabled ) = 0;

		/// <summary>
		/// Called once, when we are first attached to the scene.
		/// </summary>
		virtual void OnAttach( Scene * scene ) = 0;

		/// <summary>
		/// Called once, when we are detached from the scene.
		/// </summary>
		virtual void OnDetach( Scene * scene ) = 0;			 

		/// <summary>
		/// Called once, regardless of enabled or not, before all other events.
		/// </summary>
		virtual void OnInit( Scene * scene ) = 0;

		/// <summary>
		/// called once, when enabled, only immediatly before the first OnUpdate.
		/// </summary>
		virtual void OnStart( Scene * scene ) = 0;

		/// <summary>
		/// Called every game update cycle.
		/// </summary>
		virtual void OnUpdate( Scene * scene, const RenderInfo & renderInfo ) = 0;

		/// <summary>
		/// Called during rendering.
		/// </summary>
		virtual void OnRender( Scene * scene, const RenderInfo & renderInfo ) = 0;

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