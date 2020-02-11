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

namespace me
{
	namespace interop
	{
		template< typename T >
		class ReferenceCast : public IValue
		{
			bool m_isWriteable;
			bool m_isReadable;
			T & m_reference;
		public:
			ReferenceCast( T & reference, bool isWriteable = true, bool isReadable = true )
				: m_reference{ reference }
				, m_isWriteable{ isWriteable }
				, m_isReadable{ isReadable }
			{
			}

			~ReferenceCast()
			{
			}

			bool IsWriteable() const
			{
				return m_isWriteable;
			}

			bool IsReadable() const
			{
				return m_isReadable;
			}

			void Set( std::string value )
			{
				m_reference = unify::Cast< T >( value );
			}

			std::string Get() const override
			{
				return unify::Cast< std::string, T >( m_reference );
			}

			std::string ToString() const override
			{
				return Get();
			}
		};
	}
}