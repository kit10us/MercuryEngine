// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

///<summary>
/// A geometry which facilitates rendering of several other geometry in different places.
///</summary>

#pragma once

#include <unify/TexArea.h>
#include <unify/Color.h>
#include <dxi/Geometry.h>
#include <dxi/PlacementEntry.h>
#include <dxi/Texture.h>
#include <vector>

namespace dxi
{
	class GeometryPlacement : public Geometry
	{
	public:
		GeometryPlacement( Geometry::ptr geometry );
		~GeometryPlacement();

		// ::Resource...
		void Create();
		void Destroy();
		void Validate();
		void Invalidate();

		// Geometry HAS TO Render
		virtual void Render( const RenderInfo & renderInfo, GeometryInstanceData * instanceData );

		// Geometry CAN Update, but doesn't have to
		virtual void Update( const RenderInfo & renderInfo, GeometryInstanceData * instanceData );

		void AddPlace( const unify::V3< float > & position );

		void AddPlacesFromTexture( dxi::Texture::ptr texture, const unify::TexArea & area, float depth, unify::Color color );

	protected:
		virtual const unify::BBox< float > & ComputeBounds();

		std::vector< PlacementEntry > m_objects;
		Geometry::ptr m_geometry;
	};
}