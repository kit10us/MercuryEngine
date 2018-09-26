// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <canvas/Element.h>
#include <me/game/IGame.h>

namespace canvas
{
	/// <summary>
	/// A Layer is the simplist grouping of elements. No effort is made beyond the grouping itself (no sorting, no overlapping, no aranging).
	/// </summary>
	class Layer : public canvas::Element
	{
	public:
		typedef std::shared_ptr< canvas::Layer > ptr;

		Layer( me::game::IGame * gameInstance, unify::V2< float > position = {0, 0}, unify::Size< float > size = {100,100}, Anchor anchor = Anchor::StretchFull );

		void AddElement( IElement::ptr element );

		/// <summary>
		/// Add element as a named element, so it can be searched for later.
		/// </summary>
		void AddElement( IElement::ptr element, std::string name );

		IElement::ptr FindElement( std::string name );

		void UpdateLayout( me::UpdateParams params, unify::Rect< float > area );
		void Update( const me::UpdateParams & params ) override;
		void Render( const me::render::Params & params ) override;
		void OnSuspend() override;
		void OnResume() override;

	private:
		std::list< IElement::ptr > m_elements;
		std::map< std::string, IElement::ptr, unify::CaseInsensitiveLessThanTest > m_elementMap;
	};
}