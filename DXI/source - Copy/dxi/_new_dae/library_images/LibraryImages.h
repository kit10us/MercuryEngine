#include <qxml/QXML.h>
#include <dxi/dae/DAE_Library.h>
#include <dxi/dae/library_images/DAE_Image.h>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_images (0 or more)
		/// </summary>
		class LibraryImages : public Library< Image >
		{
		public:
			LibraryImages( IDocument & document, const qxml::Element * node );
		};
	}
}
