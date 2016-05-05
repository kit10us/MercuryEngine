// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/dae/DAE_NewParam.h>
#include <unify/String.h>

using namespace dxi;
using namespace dae;

NewParam::NewParam(  IDocument & document, const qxml::Element * node )
: DocumentNode( document, node )
, m_sid( node->GetStringAttribute( "sid" ) )
{
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "sampler2D" ) )
		{
			m_type = Sampler2DType;
			m_sampler2D.source = childNode->GetElement( "source" )->GetText();
			if ( childNode->HasElements( "minfilter" ) )
			{
				m_sampler2D.minfilter = childNode->GetElement( "minfilter" )->GetText();
			}
			if ( childNode->HasElements( "magfilter" ) )
			{
				m_sampler2D.magfilter = childNode->GetElement( "magfilter" )->GetText();
			}
		}
		else if ( childNode->IsTagName( "surface" ) )
		{
			m_type = SurfaceType;
			m_surface.type = childNode->GetStringAttributeElse( "type", std::string() );
			m_surface.init_from = childNode->GetElement( "init_from" )->GetText();
			if ( childNode->HasElements( "format" ) )
			{
				m_surface.format = childNode->GetElement( "format" )->GetText();
			}
		}
		/*
		if ( childNode->IsTagName( "translate" ) )
		{
			std::vector< float > components;
			components = unify::SplitOnWhitespace< float >( childNode->GetText() );
			unify::V3< float > translation( components[ 0 ], components[ 1 ], components[ 2 ] );
			m_matrix.Translate( translation ); 
		}
		*/
	}
}

const std::string & NewParam::GetSID() const
{
	return m_sid;
}

dxi::dae::NewParam::Type NewParam::GetType() const
{
	return m_type;
}

const dxi::dae::NewParam::Surface & NewParam::GetSurface() const
{
	return m_surface;
}

const dxi::dae::NewParam::Sampler2D & NewParam::GetSampler2D() const
{
	return m_sampler2D;
}
