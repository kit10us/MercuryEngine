#include <dxi/dae/library_images/DAE_LibraryImages.h>

using namespace dxi;
using namespace dae;

LibraryImages::LibraryImages( IDocument & document, const qxml::Element * node )
: Library< Image >( document, node, "image" ) 
{
}