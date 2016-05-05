#include <dae/Input_Unshared.h>
#include <unify/String.h>

using namespace dxi;
using namespace dae;

Input_Unshared::Input_Unshared( IDocument & document, const qxml::Element * node )
: DocumentNode( document, node )
, m_semantic( node->GetStringAttribute( "semantic" ) )
, m_source( node->GetStringAttribute( "source" ) )
{
}

const std::string & Input_Unshared::GetSemantic() const
{
	return m_semantic;
}

const std::string & Input_Unshared::GetSource() const
{
	return m_source;
}
