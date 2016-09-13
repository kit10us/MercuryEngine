// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Effect.h>
#include <rm/ResourceManagerSimple.h>
#include <qxml/Document.h>

namespace dxi
{
	class EffectXMLFactory : public rm::IXMLFactory< Effect >
	{
	public:
		virtual Effect * Produce( const qxml::Element & element ) override;
	};
}