// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/RenderInfo.h>
#include <me/RenderSet.h>
#include <unify/FrameLite.h>
#include <string>
#include <memory>

namespace me
{
	namespace scene
	{
		class Object;

		class IObjectComponent
		{
		public:
			typedef std::shared_ptr< IObjectComponent > ptr;

			virtual ~IObjectComponent() {}

			virtual std::string GetName() const = 0;

			virtual bool IsEnabled() const = 0;
			virtual void SetEnabled( bool enabled ) = 0;

			virtual bool Renderable() const = 0;

			/// <summary>
			/// Called once, when we are first attached to an object.
			/// </summary>
			virtual void OnAttach( Object * object ) = 0;

			/// <summary>
			/// Called once, when we are detached from an object.
			/// </summary>
			virtual void OnDetach( Object * object ) = 0;

			/// <summary>
			/// Called once, regardless of enabled or not, before all other events.
			/// </summary>
			virtual void OnInit( Object * object ) = 0;

			/// <summary>
			/// called once, when enabled, only immediatly before the first OnUpdate.
			/// </summary>
			virtual void OnStart( Object * object ) = 0;

			/// <summary>
			/// Called every game update cycle.
			/// </summary>
			virtual void OnUpdate( Object * object, IRenderer * renderer, const RenderInfo & renderInfo ) = 0;

			/// <summary>
			/// Called during rendering.
			/// </summary>
			virtual void OnRender( Object * object, IRenderer * renderer, const RenderInfo & renderInfo, GeometryCache & cache, const unify::FrameLite * frame ) = 0;

			/// <summary>
			/// Called when we get a suspend request. One more Render is called to allow updating based on becoming suspend (suspended graphics).
			/// </summary>
			virtual void OnSuspend( Object * object ) = 0;

			/// <summary>
			/// Called when resuming from suspending.
			/// </summary>
			virtual void OnResume( Object * object ) = 0;

			/// <summary>
			/// Duplicate component.
			/// </summary>
			virtual IObjectComponent * Duplicate() = 0;
		};
	}
}