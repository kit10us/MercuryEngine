// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/scene2d/Element.h>
#include <me/IGame.h>

namespace me
{
	namespace scene2d
	{
		/// <summary>
		/// A Layer is the simplist grouping of elements. No effort is made beyond the grouping itself (no sorting, no overlapping, no aranging).
		/// </summary>
		class Layer : public scene2d::Element
		{
		public:
			typedef std::shared_ptr< scene2d::Layer > ptr;

			Layer( IGame * game, unify::V2< float > position = {0, 0}, unify::Size< float > size = {100,100}, Anchor anchor = Anchor::StretchFull );

			void AddElement( IElement::ptr element );

			/// <summary>
			/// Add element as a named element, so it can be searched for later.
			/// </summary>
			void AddElement( IElement::ptr element, std::string name );

			IElement::ptr FindElement( std::string name );

			void UpdateLayout( IRenderer * renderer, const RenderInfo & renderInfo, unify::Size< float > area );
			void Update( IRenderer * renderer, const RenderInfo & renderInfo ) override;
			void Render( IRenderer * renderer, const RenderInfo & renderInfo ) override;
			void OnSuspend() override;
			void OnResume() override;

		private:
			std::list< IElement::ptr > m_elements;
			std::map< std::string, IElement::ptr > m_elementMap;
		};
	}
}