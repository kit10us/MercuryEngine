// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Unify.h>

namespace unify
{
	template< typename T >
	T Lerp( T a, T b, float delta );

	#include <unify/Math.inl>
}
