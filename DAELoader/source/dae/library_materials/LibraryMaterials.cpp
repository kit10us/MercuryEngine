// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <dae/library_materials/LibraryMaterials.h>

using namespace dae;

LibraryMaterials::LibraryMaterials( IDocument & document, const qxml::Element * node )
: Library< Material >( document, node, "material" ) 
{
}