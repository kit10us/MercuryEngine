#include <dxi/dae/library_nodes/DAE_LibraryNodes.h>

using namespace dxi;
using namespace dae;

LibraryNodes::LibraryNodes( IDocument & document, const qxml::Element * node )
: Library< Node >( document, node, "node" ) 
{
}