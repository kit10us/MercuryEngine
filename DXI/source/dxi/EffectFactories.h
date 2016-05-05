// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Effect.h>
#include <rm/ResourceManagerSimple.h>
#include <qxml/Document.h>

namespace dxi
{
	Effect * ProduceEffect( const qxml::Element * childNode );

	class EffectSourceFactory : public rm::ISourceFactory< Effect >
	{
	public:
		virtual Effect * Produce( unify::Path path ) override;
	};
}