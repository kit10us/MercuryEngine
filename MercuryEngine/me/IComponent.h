// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/IThing.h>
#include <me/interop/Interop.h>
#include <string>
#include <memory>
#include <list>

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
		/// Interop features allow communications between various extensions and scripts.
		/// This function returns the interop interface.
		/// </summamry>
		virtual interop::Interop * GetLookup() = 0;

		/// <summary>
		/// Interop features allow communications between various extensions and scripts.
		/// This function returns a constant version of the interop interface.
		/// </summamry>
		virtual const interop::Interop * GetLookup() const = 0;
	};
}