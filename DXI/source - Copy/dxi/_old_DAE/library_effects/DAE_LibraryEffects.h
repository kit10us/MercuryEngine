#include <qxml/QXML.h>
#include <dxi/dae/DAE_Library.h>
#include <dxi/dae/library_effects/DAE_Effect.h>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_effects (0 or more)
		/// </summary>
		class LibraryEffects : public Library< Effect >
		{
		public:
			LibraryEffects( IDocument & document, const qxml::Element * node );
		};
	}
}
