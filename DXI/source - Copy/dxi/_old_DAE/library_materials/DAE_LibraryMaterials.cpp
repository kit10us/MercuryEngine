// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/dae/library_materials/DAE_LibraryMaterials.h>

using namespace dxi;
using namespace dae;

LibraryMaterials::LibraryMaterials( IDocument & document, const qxml::Element * node )
: Library< Material >( document, node, "material" ) 
{
}