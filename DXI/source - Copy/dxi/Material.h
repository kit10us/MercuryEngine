// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/Texture.h>
#include <dxi/Usage.h>
#include <unify/ColorUnit.h>
#include <unify/MinMax.h>

namespace dxi
{
	static const unify::ColorUnit g_DefaultDiffuse;
	static const unify::ColorUnit g_DefaultAmbient;
	static const unify::ColorUnit g_DefaultSpecular;
	static const unify::ColorUnit g_DefaultEmissive;
	static const float g_DefaultPower;


	class Material
	{
	public:
		typedef std::shared_ptr< Material > shared_ptr;
		typedef std::weak_ptr< Material > weak_ptr;

		Material();
		~Material();

		Material & operator = ( const Material & material );
		bool operator == ( const Material & material ) const;
		bool operator != ( const Material & material ) const;

		void Use();
		void Unuse();

		void IgnoreMaterial( bool tf = true );
		bool IgnoreMaterial();

		// Get...
		unify::ColorUnit GetDiffuse() const;
		unify::ColorUnit GetAmbient() const;
		unify::ColorUnit GetEmissive() const;
		unify::ColorUnit GetSpecular() const;
		float GetSpecularPower() const;

		// Set...
		void SetDiffuse( const unify::ColorUnit & diffuse );
		void SetAmbient( const unify::ColorUnit & ambient );
		void SetEmissive( const unify::ColorUnit & emissive );
		void SetSpecular( const unify::ColorUnit & specular );
		void SetSpecularPower( float power );

	protected:
		bool	m_ignoreMaterial;
		D3DMATERIAL9	m_material;
	};
}