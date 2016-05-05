#include <dxi/dae/DAE_Profile_Technique.h>

using namespace dxi;
using namespace dae;

Profile_Technique::Profile_Technique( const qxml::Element * node )
: m_id( node->GetStringAttributeElse( "id", std::string() ) )
, m_sid( node->GetStringAttribute( "sid" ) )
{
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "blinn" ) )
		{
			// TODO:
			//technique._shadingType = Blinn;
		}
		else if ( childNode->IsTagName( "constant" ) )
		{
			// TODO:
		}
		else if ( childNode->IsTagName( "lambert" ) )
		{
			m_shading.reset( new Shading( childNode ) );
		}
		else if ( childNode->IsTagName( "phong" ) )
		{
			m_shading.reset( new Shading( childNode ) );
		}
	}
}

const std::string & Profile_Technique::GetID() const
{
	return m_id;
}

const std::string & Profile_Technique::GetSID() const
{
	return m_sid;
}

const Shading & Profile_Technique::GetShading() const
{
	return *m_shading.get();
}
