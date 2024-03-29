// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/render/GeometryCache.h>
#include <me/IComponent.h>
#include <me/UpdateParams.h>
#include <unify/FrameLite.h>
#include <unify/BBox.h>
#include <string>
#include <memory>

namespace me
{
	namespace object
	{
		class Object;

		namespace component
		{
			class IObjectComponent : public IComponent
			{
			public:
				typedef std::shared_ptr< IObjectComponent > ptr;
				typedef std::weak_ptr< IObjectComponent > weak_ptr;
				typedef std::list< IObjectComponent * > cache;

				virtual ~IObjectComponent() {}

				virtual void SetAlias( std::string alias ) = 0;
				virtual std::string GetAlias() const = 0;

				virtual Object* GetObject() = 0;
				virtual const Object* GetObject() const = 0;

				/// <summary>
				/// Adds to a BBox and a matrix (origin) the shape of the object component.
				/// </summary>
				virtual void GetBBox( unify::BBox< float > & bbox, const unify::Matrix & matrix ) const = 0;

				/// <summary>
				/// Does the object require updates?
				/// </summary>
				virtual bool Updateable() const = 0;

				/// <summary>
				/// Does the object require rendering?
				/// </summary>
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
				/// called once, when enabled, only immediatly before the first OnUpdate.
				/// </summary>
				virtual void OnStart() = 0;

				/// <summary>
				/// Called every game update cycle.
				/// </summary>
				virtual void OnUpdate( const UpdateParams & params ) = 0;

				/// <summary>
				/// Called during rendering.
				/// </summary>
				virtual void CollectGeometry( render::GeometryCache & solids, render::GeometryCache & trans, const unify::FrameLite * frame ) = 0;

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
}