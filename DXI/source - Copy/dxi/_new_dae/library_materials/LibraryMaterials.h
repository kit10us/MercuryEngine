#include <qxml/QXML.h>
#include <dxi/dae/DAE_Library.h>
#include <dxi/dae/library_materials/DAE_Material.h>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_materials (0 or more)
		/// </summary>
		class LibraryMaterials : public Library< Material >
		{
		public:
			LibraryMaterials( IDocument & document, const qxml::Element * node );
		};
	}
}
