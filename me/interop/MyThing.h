// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/interop/IValue.h>
#include <memory>

namespace me::interop
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

	public: // me::interop::IValue
		bool IsReadable() const override;

		bool IsWriteable() const override;

		void Set( std::string value ) override;

		std::string Get() const override;

		std::string ToString() const override;
	};

	#include <me/interop/MyThing.inl>
}