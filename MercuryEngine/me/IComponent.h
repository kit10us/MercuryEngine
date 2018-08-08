// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/UpdateParams.h>
#include <me/Lookup.h>
#include <me/IThing.h>
#include <unify/FrameLite.h>
#include <string>
#include <memory>

namespace me
{
	class IComponent : public me::IThing
	{
	public:
		typedef std::shared_ptr< IComponent > ptr;
		typedef std::list< IComponent * > cache;

		virtual ~IComponent() {}

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