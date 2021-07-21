// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/canvas/Element.h>
#include <me/render/Effect.h>
#include <me/game/IGame.h>

namespace me::canvas
{
	/// <summary>
	/// A canvas text element.
	/// </summary>
	class TextElement : public Element
	{
		std::string m_text;
		render::Effect::ptr m_effect;
		render::IConstantBuffer::ptr m_vertexCB;
		render::IConstantBuffer::ptr m_pixelCB;
		render::IVertexBuffer::ptr m_vertexBuffer;
		bool m_changed;
		unify::V2< float > m_scale;
		bool m_shrinkToFit;

	public:
		TextElement( me::game::IGame * gameInstance, render::Effect::ptr effect, std::string text = std::string(), Anchor anchor = Anchor::Center, unify::V2< float > scale = { 1, 1 }, unify::V2< float > offset = {0, 0} );

		
		/// <summary>
		/// Set the text.
		/// </summary>
		void SetText( std::string text );

		/// <summary>
		/// Set the scale.
		/// </summary>
		void SetScale( unify::V2< float > scale );
			
		/// <summary>
		/// Returns the scale.
		/// </summary>
		unify::V2< float > GetScale() const;

	public: // me::canvas::Element...
		unify::Size< float > GetContentSize() const override;
		void UpdateLayout( UpdateParams params, unify::Rect< float > parentArea ) override;
		void Update( const UpdateParams & params ) override;
		void Render( const render::Params & params ) override;
		void OnSuspend() override;
		void OnResume() override;
	};
}