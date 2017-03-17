// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IComponent.h>
#include <me/UpdateParams.h>
#include <me/Lookup.h>
#include <unify/FrameLite.h>
#include <string>
#include <memory>

namespace me
{
	class GeometryCache;

	namespace object
	{
		class Object;

		class IObjectComponent : public IComponent
		{
		public:
			typedef std::shared_ptr< IObjectComponent > ptr;
			typedef std::list< IObjectComponent * > cache;

			virtual ~IObjectComponent() {}			 

			// TODO: Should remove or replace...
			virtual bool Updateable() const = 0;
			virtual bool Renderable() const = 0;	 

			/// <summary>
			/// Called once, when we are first attached to an object.
			/// </summary>
			virtual void OnAttach( Object * object ) = 0;

			/// <summary>
			/// Called once, when we are detached from an object.
			/// </summary>
			virtual void OnDetach( Object * objecct ) = 0;

			/// <summary>
			/// Called once, regardless of enabled or not, before all other events.
			/// </summary>
			virtual void OnInit() = 0;

			/// <summary>
			/// called once, when enabled, only immediatly before the first OnUpdate.
			/// </summary>
			virtual void OnStart() = 0;

			/// <summary>
			/// Called every game update cycle.
			/// </summary>
			virtual void OnUpdate( UpdateParams params ) = 0;

			/// <summary>
			/// Called during rendering.
			/// </summary>
			virtual void CollectGeometry( GeometryCache & cache, const unify::FrameLite * frame ) = 0;

			/// <summary>
			/// Called when we get a suspend request. One more Render is called to allow updating based on becoming suspend (suspended graphics).
			/// </summary>
			virtual void OnSuspend() = 0;

			/// <summary>
			/// Called when resuming from suspending.
			/// </summary>
			virtual void OnResume() = 0;

			/// <summary>
			/// Duplicate component.
			/// </summary>
			virtual IObjectComponent::ptr Duplicate() = 0;
		};
	}
}