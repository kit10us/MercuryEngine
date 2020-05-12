// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/IThing.h>
#include <unify/Lookup.h>
#include <unify/Cast.h>
#include <string>
#include <memory>
#include <list>
#include <functional>

namespace me::interop
{
	class IValue
	{
	public:
		typedef std::shared_ptr< IValue > ptr;
		typedef std::shared_ptr< const IValue > const_ptr;

		virtual ~IValue() {}

		/// <summary>
		/// Returns true if the value is readable.
		/// </summary>
		virtual bool IsReadable() const = 0;

		/// <summary>
		/// Returns true if the value is modifiable.
		/// </summary>
		virtual bool IsWriteable() const = 0;

		/// <summary>
		/// Set the value.
		/// </summary>
		virtual void Set( std::string value ) = 0;

		/// <summary>
		/// Get the value.
		/// </summary>
		virtual std::string Get() const = 0;

		/// <summary>
		/// Return the value as a string.
		/// </summary>
		virtual std::string ToString() const = 0;
	};
}