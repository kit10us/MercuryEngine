#include <dxi/dae/library_visual_scenes/DAE_LibraryVisualScenes.h>

using namespace dxi;
using namespace dae;

LibraryVisualScenes::LibraryVisualScenes( IDocument & document, const qxml::Element * node )
: Library< VisualScene >( document, node, "visual_scene" )
{
}
