// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/loader/LoaderUtils.h>
#include <dxi/geo/Geometry.h>

namespace dxi
{
    namespace loader
    {
	    // Create geometry from xml document's root node.
		void ShapeLoader( const unify::Path & filePath, PrimitiveList & primitiveList, Managers managers );

	    // Create geometry from xml node.
	    void ShapeLoader( const qxml::Element * shapeNode, PrimitiveList & primitiveList, Managers managers );
    }
}