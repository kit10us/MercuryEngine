// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>
#include <unify/Exception.h>
#include <memory>

namespace unify
{
	template< typename T >
	class Optional
	{
	public:
		Optional();
		Optional( const T & thing );
		~Optional();

		const T & operator() const;
	private:
		std::shared_ptr< T > m_thing;
	};
	#include <unify/Optional.inl>
}
