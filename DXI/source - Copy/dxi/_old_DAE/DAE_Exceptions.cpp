#include <dxi/dae/DAE_Exceptions.h>

using namespace dxi;
using namespace dae;


Exception_MissingCOLLADA::Exception_MissingCOLLADA( const std::string & filename )
: unify::Exception( "COLLADA DAE file \"" + filename + "\" must have the root element \"COLLADA\"!" )
{
}


Exception_NotSupported::Exception_NotSupported( const std::string & feature )
: unify::Exception( "DAE feature \"" + feature + "\" not supported!" )
{
}

Exception_MissingChildElement::Exception_MissingChildElement( const std::string & parent, const std::string & child )
: unify::Exception( "DAE element " + parent + " is mising '" + child + "' child element!" )
{
}
