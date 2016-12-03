// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/ObjectComponent.h>
#include <me/scene/GeometryComponent.h>
#include <me/Effect.h>
#include <me/PrimitiveList.h>

namespace me
{
    namespace scene
    {
	    class BBoxRendererComponent : public ObjectComponent
	    {
		protected:
			BBoxRendererComponent( BBoxRendererComponent & component );

	    public:
			BBoxRendererComponent( IOS * os, Effect::ptr effect, unify::Color color = unify::Color::ColorBlue( 155 ) );
			virtual ~BBoxRendererComponent();

			std::string GetName() const override;

			bool Renderable() const { return true; }

			/// <summary>
			/// Get padding: the amount added to each vertex to increase the size of the visual BBox over the acutal BBox (so we can actually see it).
			/// </summary>
			float GetPadding() const;

			/// <summary>
			/// Set padding: the amount added to each vertex to increase the size of the visual BBox over the acutal BBox (so we can actually see it).
			/// </summary>
			void SetPadding( float padding );

			void OnAttach( Object * object ) override;
			void OnDetach( Object * object ) override;
			void OnUpdate( Object * object, IRenderer * renderer, const RenderInfo & renderInfo ) override;
			void OnRender( Object * object, IRenderer * renderer, const RenderInfo & renderInfo, GeometryCache & cache, const unify::FrameLite * frame ) override;

			IObjectComponent * Duplicate();
   
	    protected:
			Object * m_object;
			std::list< GeometryComponent * > m_geomertries;
			Geometry::ptr m_geometry;
			Effect::ptr m_effect;
			float m_padding;
			unify::Color m_color;
	    };
    }
}