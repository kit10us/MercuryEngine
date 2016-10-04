// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Effect.h>
#include <dae/library_effects/Shading.h>
#include <unify/unify.h>

namespace dae
{
	namespace util
	{
		class IEffectSolver
		{
		public:
			virtual ~IEffectSolver() {}
			virtual dxi::Effect::ptr GetEffect( const dae::Shading & shading ) const = 0;				
		};
	}
}