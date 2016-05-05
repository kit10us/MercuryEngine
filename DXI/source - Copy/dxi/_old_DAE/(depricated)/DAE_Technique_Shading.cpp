#include <dxi/dae/DAE_Technique_Shading.h>
#include <unify/String.h>

using namespace dxi;
using namespace dae;

Technique_Shading::Technique_Shading( const qxml::Element * node )
{
	for ( const qxml::Element * childNode = node->GetFirstChild(); childNode; childNode = childNode->GetNext() )
	{
		if ( childNode->IsTagName( "emission" ) )
		{
			m_emission = unify::ColorUnit( childNode->FindFirstElement( "color" )->GetText() ); 
		}
		else if ( childNode->IsTagName( "ambient" ) )
		{
			m_ambient = unify::ColorUnit( childNode->FindFirstElement( "color" )->GetText() ); 
		}
		else if ( childNode->IsTagName( "diffuse" ) )
		{
			m_diffuse = unify::ColorUnit( childNode->FindFirstElement( "color" )->GetText() ); 
		}
		else if ( childNode->IsTagName( "specular" ) )
		{
			m_specular = unify::ColorUnit( childNode->FindFirstElement( "color" )->GetText() ); 
		}
		else if ( childNode->IsTagName( "shininess" ) )
		{
			m_shininess = unify::Cast< float >( childNode->FindFirstElement( "float" )->GetText() );
		}
		else if ( childNode->IsTagName( "reflective" ) )
		{
			m_reflective = unify::ColorUnit( childNode->FindFirstElement( "color" )->GetText() ); 
		}
		else if ( childNode->IsTagName( "reflectivity" ) )
		{
			m_reflectivity = unify::Cast< float >( childNode->FindFirstElement( "float" )->GetText() );
		}
		else if ( childNode->IsTagName( "transparent" ) )
		{
			m_transparent = unify::ColorUnit( childNode->FindFirstElement( "color" )->GetText() ); 
		}
		else if ( childNode->IsTagName( "transparency" ) )
		{
			m_transparency = unify::Cast< float >( childNode->FindFirstElement( "float" )->GetText() );
		}
		else if ( childNode->IsTagName( "index_of_refraction" ) )
		{
			m_index_of_refraction = unify::Cast< float >( childNode->FindFirstElement( "float" )->GetText() );
		}
	}
}

Technique_Shading::Type Technique_Shading::GetType() const
{
	return m_type;
}

const boost::optional< unify::ColorUnit > & Technique_Shading::GetEmission() const
{
	return m_emission;
}

const boost::optional< unify::ColorUnit > & Technique_Shading::GetAmbient() const
{
	return m_ambient;
}

const boost::optional< unify::ColorUnit > & Technique_Shading::GetDiffuse() const
{
	return m_diffuse;
}

const boost::optional< unify::ColorUnit > & Technique_Shading::GetSpecular() const
{
	return m_specular;
}

const boost::optional< float > & Technique_Shading::GetShininess() const
{
	return m_shininess;
}

const boost::optional< unify::ColorUnit > & Technique_Shading::GetReflective() const
{
	return m_reflective;
}

const boost::optional< float > & Technique_Shading::GetReflectivity() const
{
	return m_reflectivity;
}

const boost::optional< unify::ColorUnit > & Technique_Shading::GetTransparent() const
{
	return m_transparent;
}

const boost::optional< float > & Technique_Shading::GetTransparency() const
{
	return m_transparency;
}

const boost::optional< float > & Technique_Shading::GetIndexOfRefraction() const
{
	return m_index_of_refraction;
}
