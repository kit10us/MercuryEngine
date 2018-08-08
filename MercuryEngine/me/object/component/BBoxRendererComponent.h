// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/object/component/ObjectComponent.h>
#include <me/object/component/GeometryComponent.h>
#include <me/render/Effect.h>
#include <me/render/PrimitiveList.h>
#include <unify/Color.h>

namespace me
{
    namespace object
    {
		namespace component
		{
			class BBoxRendererComponent : public ObjectComponent
			{
			protected:
				BBoxRendererComponent( BBoxRendererComponent & component );

			public:
				BBoxRendererComponent( os::IOS * os, render::Effect::ptr effect, unify::Color color = unify::Color::ColorBlue( 155 ) );
				virtual ~BBoxRendererComponent();

				/// <summary>
				/// Get padding: the amount added to each vertex to increase the size of the visual BBox over the acutal BBox (so we can actually see it).
				/// </summary>
				float GetPadding() const;

				/// <summary>
				/// Set padding: the amount added to each vertex to increase the size of the visual BBox over the acutal BBox (so we can actually see it).
				/// </summary>
				void SetPadding( float padding );

			public: // IObjectComponent...
				void OnAttach( Object * object ) override;
				void OnDetach( Object * object ) override;
				void OnUpdate( const UpdateParams & params ) override;
				void CollectGeometry( render::GeometryCache & solids, render::GeometryCache & trans, const unify::FrameLite * frame ) override;

				IObjectComponent::ptr Duplicate();

			public: // IComponent...

			public: // IThing...

			protected:
				Object * m_object;
				os::IOS * m_os;
				unify::BBox< float > m_bbox;
				std::list< GeometryComponent * > m_geomertries;
				render::Geometry::ptr m_geometry;
				render::Effect::ptr m_effect;
				float m_padding;
				unify::Color m_color;
				int m_componentsChecked;
			};
		}
    }
}