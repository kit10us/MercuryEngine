// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/EffectManager.h>
#include <dxi/EffectFactory.h>

using namespace dxi;

EffectManager::EffectManager()
: ResourceManager( ResourceDesc( "", "xml;fx" ), std::shared_ptr< ResourceManager< Effect >::IFactory >( new EffectFactory ) )
{
}
