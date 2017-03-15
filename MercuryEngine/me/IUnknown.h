// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <string>
#include <memory>

namespace me
{
	class IUnknown
	{
	public:
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
		/// Interface functions...
		///////////////////////////////////

		virtual IUnknown* QueryInterface( std::string name ) = 0;

		template< typename T >
		T* QueryInterfaceT( std::string name )
		{
			return dynamic_cast<T*>(QueryInterface( name ));
		}
	};
}