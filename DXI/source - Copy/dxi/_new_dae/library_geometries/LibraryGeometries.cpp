#include <dxi/dae/library_geometries/DAE_LibraryGeometries.h>

using namespace dxi;
using namespace dae;

LibraryGeometries::LibraryGeometries( IDocument & document, const qxml::Element * node )
: Library< Geometry >( document, node, "geometry" )
{
}