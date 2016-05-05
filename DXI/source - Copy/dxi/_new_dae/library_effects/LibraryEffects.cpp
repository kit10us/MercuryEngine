#include <dxi/dae/library_effects/DAE_LibraryEffects.h>

using namespace dxi;
using namespace dae;

LibraryEffects::LibraryEffects( IDocument & document, const qxml::Element * node )
: Library< Effect >( document, node, "effect" ) 
{
}