// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/animation/Instance.h>
#include <me/Effect.h>
#include <me/Geometry.h>
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
			Character( const animation::Instance & animationInstance, me::Effect::ptr effect );
			Character( me::Geometry::ptr geometry, const unify::Size< float > & sizeInPixels, float geometryPostScale2D, const unify::V3< float > & geometryPostOffset2D, float geometryPostScale3D, const unify::V3< float > & geometryPostOffset3D );
			~Character() throw ();

			Source::TYPE GetSource() const;

			me::Effect::ptr GetEffect();
			const me::Effect::ptr GetEffect() const;

			const animation::Instance & GetAnimationInstance() const;

			me::Geometry::ptr GetGeometry();
			const me::Geometry::ptr GetGeometry() const;

			unify::Size< float > GetSize() const;

			bool empty() const;
			void Update( unify::Seconds elapsed );
			void Render( const me::RenderInfo & renderInfo, const unify::Matrix & origin, const unify::V2< float > & offset, float scale, bool is3D );

		protected:
			Source::TYPE m_source;
			me::Effect::ptr m_effect;
			animation::Instance m_animationInstance;
			me::Geometry::ptr m_geometry;
			unify::Size< float > m_size;

			float m_geometryPostScale2D; // Used to compensate for geometry sizes.
			unify::V3< float > m_geometryPostOffset2D;
			float m_geometryPostScale3D; // Used to compensate for geometry sizes.
			unify::V3< float > m_geometryPostOffset3D;
		};
	}
}
