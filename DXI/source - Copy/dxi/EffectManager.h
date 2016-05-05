// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Effect.h>
#include <dxi/ResourceManager.h>
#include <memory>

namespace dxi
{
	class EffectManager : public ResourceManager< Effect >
	{
	public:
		typedef std::shared_ptr< EffectManager > shared_ptr;
		typedef std::weak_ptr< EffectManager > weak_ptr;

		EffectManager();
	};
}