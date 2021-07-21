// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/canvas/SizeTo.h>
#include <me/canvas/Element.h>
#include <me/render/Effect.h>

namespace me::canvas
{
	/// <summary>
	/// Create a simple menu system.
	/// </summary>
	class Grid : public Element
	{
		bool m_changed;
		unify::RowColumn< size_t > m_rc;
		unify::Size< float > m_size;
		std::vector< size_t > m_weightRows;
		std::vector< size_t > m_weightColumns;
		std::vector< std::vector< IElement::ptr > > m_items;
	public:
		Grid( game::IGame * gameInstance, unify::RowColumn< size_t > rc, unify::Size< float > size, Anchor anchor = Anchor::Center, unify::V2< float > offset = { 0, 0 } );

		/// <summary>
		/// Add an item into the grid.
		/// </summary>
		void AddItem( IElement::ptr item );

	public: // IElement...
		unify::Size< float > GetContentSize() const override;
		void UpdateLayout(UpdateParams params, unify::Rect< float > parentArea) override;
		void Update(const UpdateParams& params) override;
		void Render(const render::Params& params) override;
		void OnSuspend() override;
		void OnResume() override;

	};
}