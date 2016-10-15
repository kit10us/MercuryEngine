// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/scene/Component.h>
#include <dxi/scene/GeometryComponent.h>

namespace dxi
{
    namespace scene
    {
	    class BBoxRendererComponent : public Component
	    {
	    public:
			BBoxRendererComponent( Effect::ptr effect );
			virtual ~BBoxRendererComponent();

			/// <summary>
			/// Get padding: the amount added to each vertex to increase the size of the visual BBox over the acutal BBox (so we can actually see it).
			/// </summary>
			float GetPadding() const;

			/// <summary>
			/// Set padding: the amount added to each vertex to increase the size of the visual BBox over the acutal BBox (so we can actually see it).
			/// </summary>
			void SetPadding( float padding );

			void OnInit( Object * object ) override;
			void OnStart( Object * object ) override;
			void Update( const RenderInfo & renderInfo ) override;
			void Render( const RenderInfo & renderInfo ) override;
			void OnSuspend() override;
			void OnResune() override;
   
	    protected:
			std::list< GeometryComponent * > m_geomertries;
			PrimitiveList m_pl;
			Effect::ptr m_effect;
			float m_padding;
			unify::Color m_color;
	    };
    }
}