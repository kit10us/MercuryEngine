#include <qxml/QXML.h>
#include <dxi/dae/DAE_Library.h>
#include <dxi/dae/DAE_Node.h>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_nodes (0 or more)
		/// </summary>
		class LibraryNodes : public Library< Node >
		{
		public:
			LibraryNodes( IDocument & document, const qxml::Element * node );
		};
	}
}
