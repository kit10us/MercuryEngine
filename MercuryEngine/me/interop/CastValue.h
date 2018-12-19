// Copyright (c) 2002 - 2018, Evil Quail LLC
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
		class CastValue : public IValue
		{
			bool m_isWriteable;
			bool m_isReadable;
			T m_value;
		public:
			CastValue( T value, bool isWriteable = true, bool isReadable = true )
				: m_value{ value }
				, m_isWriteable{ isWriteable }
				, m_isReadable{ isReadable }
			{
			}
		
			~CastValue() 
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
				m_value = unify::Cast< T >( value );
			}
		
			std::string Get() const override
			{
				return unify::Cast< std::string >( m_value );
			}

			std::string ToString() const override
			{
				return Get();
			}
		};
	}
}