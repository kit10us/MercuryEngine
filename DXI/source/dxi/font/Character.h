// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/animation/Instance.h>
#include <dxi/Effect.h>
#include <dxi/Geometry.h>
#include <unify/Size.h>

namespace dxi
{
	namespace font
	{
                // Contains attributes on a single character, where it came from, and how to render it.
		class Character
		{
		public:
			struct Source
			{
				enum TYPE
				{
					Invalid,
					Sprite,
					Geometry
				};
			};

			Character();
			Character( const Character & character );
			Character( const animation::Instance & animationInstance, std::shared_ptr< Effect > effect );
			Character( Geometry::shared_ptr geometry, const unify::Size< float > & sizeInPixels, float geometryPostScale2D, const unify::V3< float > & geometryPostOffset2D, float geometryPostScale3D, const unify::V3< float > & geometryPostOffset3D );
			~Character() throw ();

			bool empty() const;
			const unify::Size< float > & GetSize() const;
			void Update( unify::Seconds elapsed );
			void Render( RenderInfo & renderInfo, const unify::Matrix & origin, const unify::V2< float > & offset, float scale, bool is3D );

		protected:
			Source::TYPE m_source;
			std::shared_ptr< Effect > m_effect;
			animation::Instance m_animationInstance;
			std::shared_ptr< Geometry > m_geometry;
			unify::Size< float > m_size;

			float m_geometryPostScale2D; // Used to compensate for geometry sizes.
			unify::V3< float > m_geometryPostOffset2D;
			float m_geometryPostScale3D; // Used to compensate for geometry sizes.
			unify::V3< float > m_geometryPostOffset3D;
		};
	}
}
