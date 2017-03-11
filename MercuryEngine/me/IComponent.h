// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/UpdateParams.h>
#include <me/Lookup.h>
#include <unify/FrameLite.h>
#include <string>
#include <memory>

namespace me
{
	class IComponent
	{
	public:
		typedef std::shared_ptr< IComponent > ptr;
		typedef std::list< IComponent * > cache;

		virtual ~IComponent() {}


		///////////////////////////////////
		/// Identity functions...
		///////////////////////////////////

		/// <summary>
		/// Returns the type of the component.
		/// </summary>
		virtual std::string GetTypeName() const = 0;

		/// <summary>
		/// Returns a small text, typically used in debugging to isolate the specific instance of the component.
		/// </summary>
		virtual std::string GetWhat() const = 0;


		///////////////////////////////////
		/// State functions...
		///////////////////////////////////

		/// <summary>
		/// Returns true if the component is enabled, else false.
		/// </summary>
		virtual bool IsEnabled() const = 0;

		/// <summary>
		/// Enables or disables the component.
		/// </summary>
		virtual void SetEnabled( bool enabled ) = 0;


		///////////////////////////////////
		/// Value interop functions...
		///////////////////////////////////

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
	};
}