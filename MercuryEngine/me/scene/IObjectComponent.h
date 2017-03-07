// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/UpdateParams.h>
#include <me/GeometryCache.h>
#include <me/Lookup.h>
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
			typedef std::list< IObjectComponent * > cache;

			virtual ~IObjectComponent() {}

			/// <summary>
			/// Returns the type of the component.
			/// </summary>
			virtual std::string GetType() const = 0;

			/// <summary>
			/// Returns a small text, typically used in debugging to isolate the specific instance of the component.
			/// </summary>
			virtual std::string GetWhat() const = 0;

			/// <summary>
			/// Returns the number of values.
			/// </summary>
			virtual int GetValueCount() const = 0;

			/// <summary>
			/// Check if a values exists.
			/// </summary>
			virtual bool ValueExists( std::string ) const = 0;

			/// <summary>
			/// Get the name of a value by index.
			/// </summary>
			virtual std::string GetValueName( int index ) const = 0;

			/// <summary>
			/// Returns the index of a value by name.
			/// </summary>
			virtual int FindValueIndex( std::string name ) const = 0;

			/// <summary>
			/// Get a value by index.
			/// </summary>
			virtual std::string GetValue( int index ) const = 0;

			/// <summary>
			/// Get a value by name.
			/// </summary>
			virtual std::string GetValue( std::string name ) const = 0;

			/// <summary>
			/// Set a value by index.
			/// </summary>
			virtual bool SetValue( int index, std::string value ) = 0;

			/// <summary>
			/// Set a value.
			/// </summary>
			virtual bool SetValue( std::string name, std::string value ) = 0; 
			
			virtual bool IsEnabled() const = 0;
			virtual void SetEnabled( bool enabled ) = 0;

			virtual bool Updateable() const = 0;
			virtual bool Renderable() const = 0;

			/// <summary>
			/// Called once, when we are first attached to an object.
			/// </summary>
			virtual void OnAttach( Object * object ) = 0;

			/// <summary>
			/// Called once, when we are detached from an object.
			/// </summary>
			virtual void OnDetach() = 0;

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
			virtual IObjectComponent * Duplicate() = 0;
		};
	}
}