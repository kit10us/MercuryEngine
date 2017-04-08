// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/Effect.h>
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
			virtual me::render::Effect::ptr GetEffect( const dae::Shading & shading ) const = 0;
		};
	}
}