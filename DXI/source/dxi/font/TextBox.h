// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <dxi/font/CharacterRender.h>
#include <dxi/font/FontString.h>
#include <dxi/font/IText.h>
#include <dxi/font/Font.h>
#include <unify/NinePoint.h>

namespace dxi
{
	namespace font
	{
		typedef std::vector< CharacterRender > CharacterRenderList;

		/// <summary>
		/// An instance of text, taking a font to use when rendering.
		/// </summary>
		class TextBox : public IText
		{
		public:
			TextBox();
			TextBox( std::shared_ptr< Font > font, const unify::V2< float > & position, const FontString & fontString, float scale = float( 1.0f ) );
			virtual ~TextBox();

			void SyncCharacterRenderList();
			void Update( unify::Seconds elapsed );
			void Render( RenderInfo & renderInfo, bool is3D );

		protected:
			std::shared_ptr< Font > m_font;
			unify::V2< float > m_position;
            unify::Rect< float > m_rectangle; // Used as position.
            unify::NinePoint::TYPE m_orientation;
			FontString m_string;
			float m_scale;
			CharacterRenderList m_characterRenderList;
            unify::Size< float > m_totalSize; // Accumulation of sizes from all characters accumulated. Used for NinePoint rendering.
		};
	} // namespace font
} // namespace dxi