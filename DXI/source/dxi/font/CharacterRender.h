// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/font/Character.h>
#include <dxi/RenderInfo.h>
#include <unify/Unify.h>

namespace dxi
{
	namespace font
	{
        	// Keeps a Character, as well as specific data on rendering that character, such as offsets from the previous character rendered.
		class CharacterRender
		{
		public:
			CharacterRender();
			CharacterRender( const Character & character, const unify::V2< float > & offset, float scale );
			~CharacterRender() throw ();

			const unify::Size< float > & GetSize() const;

			void Update( unify::Seconds elapsed );
			void Render( RenderInfo renderInfo, const unify::Matrix & origin, bool is3D );
		private:
			Character m_character;
			unify::V2< float > m_offset;
			float m_scale;
			unify::Size< float > m_size;
		};
	} // namespace font
} // namespace dxi
