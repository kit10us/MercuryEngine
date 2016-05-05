// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/Managers.h>
#include <qxml/Document.h>

namespace dxi
{
    namespace loader
    {
	    // Modify existing instance.
		void EffectLoader( dxi::Effect & effect, const qxml::Element * node, Managers managers );

		// Automatically add to manager.
		Effect::shared_ptr EffectLoader( const std::string & name, const qxml::Element * node, Managers managers );

		// Load from a source file, whose root is expected to be the source node.
		Effect::shared_ptr EffectLoader( const std::string & name, const unify::Path & source, Managers managers ); 
    }
}