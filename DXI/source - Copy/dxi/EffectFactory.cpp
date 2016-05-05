// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/EffectFactory.h>

using namespace dxi;

Effect * EffectFactory::Produce( const qjson::Object & object )
{
	return new Effect();
}
