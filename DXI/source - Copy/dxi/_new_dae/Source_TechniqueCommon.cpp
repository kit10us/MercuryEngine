#include <dxi/dae/DAE_Source_TechniqueCommon.h>

using namespace dxi;
using namespace dae;

Source_TechniqueCommon::Source_TechniqueCommon( const qxml::Element * node )
{
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "accessor" ) )
		{
			m_accessor.reset( new Accessor( childNode ) );
		}
	}
}

const boost::shared_ptr< Accessor > & Source_TechniqueCommon::GetAccessor() const
{
	return m_accessor;
}
