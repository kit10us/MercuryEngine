#include <dxi/dae/DAE_Input_Shared.h>
#include <unify/String.h>

using namespace dxi;
using namespace dae;

Input_Shared::Input_Shared( const qxml::Element * node )
: m_offset( node->GetIntegerAttribute( "offset" ) )
, m_semantic( node->GetStringAttribute( "semantic" ) )
, m_source( node->GetStringAttribute( "source" ) )
{
	if ( node->HasAttributes( "set" ) )
	{
		m_set = node->GetIntegerAttribute( "set" );
	}

	if ( unify::StringIs( m_semantic, "VERTEX" ) )
	{
		m_fvfType = FVF::XYZ;
		m_stride = 3;
	}
	else if ( unify::StringIs( m_semantic, "NORMAL" ) )
	{
		m_fvfType = FVF::Normal;
		m_stride = 3;
	}
	else if ( unify::StringIs( m_semantic, "COLOR" ) )
	{
		m_fvfType = FVF::Diffuse;
		m_stride = 1;
	}
	else if ( unify::StringIs( m_semantic, "TEXCOORD" ) )
	{
		m_fvfType = FVF::Tex1;
		m_stride = 2;
	}
}

const int Input_Shared::GetOffset() const
{
	return m_offset;
}

const std::string & Input_Shared::GetSemantic() const
{
	return m_semantic;
}

const std::string & Input_Shared::GetSource() const
{
	return m_source;
}

const boost::optional< int > & Input_Shared::GetSet() const
{
	return m_set;
}

const FVF::TYPE Input_Shared::GetFVFType() const
{
	return m_fvfType;
}

size_t Input_Shared::GetStride() const
{
	return m_stride;
}
