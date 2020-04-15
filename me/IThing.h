// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <string>
#include <memory>

namespace me
{
	/// <summary>
	/// An abstract base type that can be upcasted into more concrete types.
	/// </summary>
	class IThing
	{
	protected:
		/// <summary>
		/// Adds an interface.
		/// </summary>
		virtual void AddInterface( std::string name, me::IThing* ptr ) = 0;

	public:
		/// <summary>
		/// Returns the type of the component.
		/// </summary>
		virtual std::string GetTypeName() const = 0;

		/// <summary>
		/// Returns a small text, typically used in debugging to isolate the specific instance of the component.
		/// </summary>
		virtual std::string GetWhat() const = 0;

		/// <summary>
		/// Returns an interface.
		/// </summary>
		virtual IThing* QueryInterface( std::string name ) = 0;

		/// <summary>
		/// Returns an interface as a specific interface type.
		/// </summary>
		template< typename T >
		T* QueryInterfaceT( std::string name )
		{
			return dynamic_cast<T*>(QueryInterface( name ));
		}
	};
}