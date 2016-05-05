// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Effect.h>
#include <dxi/ResourceManager.h>

namespace dxi
{
	class EffectFactory : public ResourceManager< Effect >::IFactory
	{
	public:
		Effect * Produce( const qjson::Object & object );
	};
}