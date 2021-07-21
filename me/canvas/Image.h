// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/canvas/Element.h>
#include <me/render/Effect.h>
#include <me/game/IGame.h>

namespace me::canvas
{
	class Image : public Element
	{
		render::Effect::ptr m_effect;
		render::IConstantBuffer::ptr m_vertexCB;
		render::IConstantBuffer::ptr m_pixelCB;
		render::IVertexBuffer::ptr m_vertexBuffer;
		bool m_changed;
		unify::V2< float > m_scale;
		bool m_shrinkToFit;

	public:
		Image(me::game::IGame* gameInstance, render::Effect::ptr effect, Anchor anchor = Anchor::Center, unify::V2< float > scale = { 1, 1 }, unify::V2< float > offset = { 0, 0 });

	public:
		/// <summary>
		/// Set the scale of the image.
		/// </summary>
		void SetScale(unify::V2< float > scale);

		/// <summary>
		/// Returns the scale of the image.
		/// </summary>
		unify::V2< float > GetScale() const;

	public: // IElement...
		unify::Size< float > GetContentSize() const override;
		void UpdateLayout(UpdateParams params, unify::Rect< float > parentArea) override;
		void Update(const UpdateParams& params) override;
		void Render(const render::Params& params) override;
		void OnSuspend() override;
		void OnResume() override;

	};
}