// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene/SceneComponent.h>
#include <me/scene/GeometryComponent.h>
#include <me/scene/Scene.h>
#include <me/Effect.h>

namespace me
{
    namespace scene
    {
		/// <summary>
		/// Automatically adds a BBoxRendererComponent to all new objects, except those flagged as GUI.
		/// </summary>
	    class AutoBBoxSceneComponent : public SceneComponent
	    {
	    public:
			AutoBBoxSceneComponent( IOS * os, Effect::ptr effect );
			virtual ~AutoBBoxSceneComponent();

			/// <summary>
			/// Get padding: the amount added to each vertex to increase the size of the visual BBox over the acutal BBox (so we can actually see it).
			/// </summary>
			float GetPadding() const;

			/// <summary>
			/// Set padding: the amount added to each vertex to increase the size of the visual BBox over the acutal BBox (so we can actually see it).
			/// </summary>
			void SetPadding( float padding );

			void OnUpdate( Scene * scene, const IRenderer * renderer, const RenderInfo & renderInfo ) override;
   
	    protected:
			Effect::ptr m_effect;
			float m_padding;
			unify::Color m_color;
	    };
    }
}