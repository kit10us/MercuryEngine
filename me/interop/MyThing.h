// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/interop/IValue.h>
#include <memory>

namespace me
{
	namespace interop
	{
		template< typename Thing >
		using Getter = std::function< std::string( Thing thing ) >;

		template< typename Thing >
		using Setter = std::function< void( Thing thing, std::string ) >;

		template< typename Thing >
		class MyThing : public IValue
		{
			Thing m_thing;
			Getter< Thing > m_getThing;
			Setter< Thing > m_setThing;

		public:
			MyThing( Thing thing, 
				std::function< std::string( Thing thing ) > getThing, 
				std::function< void( Thing thing, std::string ) > setThing = nullptr );

			bool IsWriteable() const override;

			void Set( std::string value );

			std::string Get() const;

			std::string ToString() const;
		};

		#include <me/interop/MyThing.inl>
	}
}