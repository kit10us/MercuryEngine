// Copyright (c) 2002 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <unify/Angle.h>
#include <unify/V2.h>
#include <unify/V3.h>
#include <dxi/animation/Instance.h>
#include <me/RenderInfo.h>
#include <me/Effect.h>
#include <me/IGame.h>

namespace dxi
{
	class Sprite
	{
	public:
		// TODO: Game here is a little heavy handed, right??? Why do we need this? Use an IRenderer, or rely on the one from RenderInfo?
		Sprite( me::IGame * game );
		Sprite( me::IGame * game, me::Effect::ptr effect, const unify::V2< float > & center, animation::Instance animationInstance, float depth = 0 );
		Sprite( me::IGame * game, me::Effect::ptr effect, const unify::V2< float > & center, const unify::V2< float > & scale, animation::Instance animationInstance, float depth = 0 );
		Sprite( me::IGame * game, me::Effect::ptr effect, const unify::V2< float > & center, const unify::Size< float > & size, animation::Instance animationInstance, float depth = 0 );
		~Sprite() throw ();

		me::Effect::ptr GetEffect() const;

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

		unify::Size< float > GetPixelSize() const;

		void Update( unify::Seconds elapsed );
		void Render( const me::RenderInfo & renderInfo );

	private:
		void SyncSizeToPixels();

		/// <summary>
		/// Initialization code. As there are so many constructors, this is to be called by all of them.
		/// </summary>
		void Init();

		me::IGame * m_game;

		me::Effect::ptr m_effect;
		animation::Instance m_animationInstance;
		unify::V2< float > m_center;
		unify::V3< unify::Angle > m_rotations;
		unify::V2< float > m_scale;
		unify::Size< float > m_size;
		float m_depth;
		bool m_sizeToPixels;
	};
}