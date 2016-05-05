// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <qxml/QXML.h>
#include <unify/ColorUnit.h>
#include <boost/optional.hpp>

namespace dxi
{
	namespace dae
	{
		/// <summary>
		/// DAE node: COLLADA->library_effects->effect->profile_COMMON.technique.{blinn, constant lambert, phong} (1)
		/// </summary>
		class Technique_Shading
		{
		public:
			enum Type {
				Blinn, Constant, Lambert, Phong
			};

			Technique_Shading( const qxml::Element * node );
			Type GetType() const;

			/// <summary>
			/// blinn, constant, lambert, phong
			/// </summary>
			const boost::optional< unify::ColorUnit > & GetEmission() const;

			/// <summary>
			/// blinn, lambert, phong
			/// </summary>
			const boost::optional< unify::ColorUnit > & GetAmbient() const;
	
			/// <summary>
			/// blinn, lambert, phong
			/// </summary>
			const boost::optional< unify::ColorUnit > & GetDiffuse() const;

			/// <summary>
			/// blinn, phong
			/// </summary>
			const boost::optional< unify::ColorUnit > & GetSpecular() const;
			
			/// <summary>
			/// blinn, phong
			/// </summary>
			const boost::optional< float > & GetShininess() const;
			
			/// <summary>
			/// blinn, constant, lambert, phong
			/// </summary>
			const boost::optional< unify::ColorUnit > & GetReflective() const;
			
			/// <summary>
			/// blinn, constant, lambert, phong
			/// </summary>
			const boost::optional< float > & GetReflectivity() const;
			
			/// <summary>
			/// blinn, constant, lambert, phong
			/// </summary>
			const boost::optional< unify::ColorUnit > & GetTransparent() const;
			
			/// <summary>
			/// blinn, constant, lambert, phong
			/// </summary>
			const boost::optional< float > & GetTransparency() const;
			
			/// <summary>
			/// blinn, constant, lambert, phong
			/// </summary>
			const boost::optional< float > & GetIndexOfRefraction() const;

		private:
			Type m_type;
			boost::optional< unify::ColorUnit > m_emission;
			boost::optional< unify::ColorUnit > m_ambient;
			boost::optional< unify::ColorUnit > m_diffuse;
			boost::optional< unify::ColorUnit > m_specular;
			boost::optional< float > m_shininess;
			boost::optional< unify::ColorUnit > m_reflective;
			boost::optional< float > m_reflectivity;
			boost::optional< unify::ColorUnit > m_transparent;
			boost::optional< float > m_transparency;
			boost::optional< float > m_index_of_refraction;
		};
	}
}