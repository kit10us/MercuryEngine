// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Angle.h>
#include <unify/V2.h>
#include <unify/V3.h>
#include <dxi/animation/Instance.h>
#include <dxi/RenderInfo.h>
#include <dxi/Blend.h>
#include <dxi/Effect.h>

namespace dxi
{
	class Sprite
	{
	public:
		Sprite();
		Sprite( Effect::ptr effect, const unify::V2< float > & center, animation::Instance animationInstance, float depth = 0 );
		Sprite( Effect::ptr effect, const unify::V2< float > & center, const unify::V2< float > & scale, animation::Instance animationInstance, float depth = 0 );
		Sprite( Effect::ptr effect, const unify::V2< float > & center, const unify::Size< float > & size, animation::Instance animationInstance, float depth = 0 );
		~Sprite() throw ();

		Effect::ptr GetEffect() const;

		void SizeToPixels( bool sizeToPixels );
		bool GetSizeToPixels() const;
		
		const unify::V2< float > & GetCenter() const;
		void MoveTo( const unify::V2< float > & center );
		void MoveBy( const unify::V2< float > & by );
		
		const unify::V3< unify::Angle > & GetRotations() const;
		void RotateTo( const unify::V3< unify::Angle > & rotations );

		const unify::V2< float > & GetScale() const;
		void ScaleTo( const unify::V2< float > & scale );

		float GetDepth() const;
		void SetDepth( float depth );

		const Blend & GetBlend() const;
		void SetBlend( const Blend & blend );

		unify::Size< float > GetPixelSize() const;

		void Update( unify::Seconds elapsed );
		void Render( const RenderInfo & renderInfo );

	private:
		void SyncSizeToPixels();

		/// <summary>
		/// Initialization code. As there are so many constructors, this is to be called by all of them.
		/// </summary>
		void Init();

		Effect::ptr m_effect;
		animation::Instance m_animationInstance;
		unify::V2< float > m_center;
		unify::V3< unify::Angle > m_rotations;
		unify::V2< float > m_scale;
		unify::Size< float > m_size;
		float m_depth;
		bool m_sizeToPixels;
		Blend m_blend;
	};
}