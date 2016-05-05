#include <unify/Exception.h>

namespace dxi
{
	namespace exception
	{
		class NotImplemented : public unify::Exception
		{
		public:
			NotImplemented( const std::string & what = std::string() )
				: Exception( std::string( "Not implemented: " ) + what )
			{
			}
		};
	}
}