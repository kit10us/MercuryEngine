// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/canvas/Element.h>
#include <me/game/IGame.h>

namespace me
{
	namespace canvas
	{
		/// <summary>
		/// A Layer is the simplist grouping of elements. No effort is made beyond the grouping itself (no sorting, no overlapping, no aranging).
		/// </summary>
		class Layer : public canvas::Element
		{
		public:
			typedef std::shared_ptr< canvas::Layer > ptr;

			Layer( game::IGame * gameInstance, unify::V2< float > position = {0, 0}, unify::Size< float > size = {100,100}, Anchor anchor = Anchor::StretchFull );
			
			/// <summary>
			/// Add an unnamed element.
			/// </summary>			
			void AddElement( IElement::ptr element );

			/// <summary>
			/// Add element as a named element, so it can be searched for later.
			/// </summary>
			void AddElement( IElement::ptr element, std::string name );

			/// <summary>
			/// Remove an element.
			/// </summary>
			void RemoveElement( IElement::ptr element );

			/// <summary>
			/// Remove an element by name.
			/// </summary>
			void RemoveElement( std::string name );

			/// <summary>
			/// Find an element by name.
			/// </summary>
			IElement::ptr FindElement( std::string name );

			/// <summary>
			/// Get the name of an element, if it is named. Returns an empty string if not found.
			/// </summary>
			std::string ElementName( IElement::const_ptr element ) const;

			/// <summary>
			/// Bring an element to the front.
			/// </summary>
			void BringToFront( IElement::ptr element );

			/// <summary>
			/// Send an element to the back.
			/// </summary>
			void SendToBack( IElement::ptr element );
			

			void UpdateLayout( UpdateParams params, unify::Rect< float > area );
			void Update( const UpdateParams & params ) override;
			void Render( const render::Params & params ) override;
			void OnSuspend() override;
			void OnResume() override;

		private:
			std::list< IElement::ptr > m_elements;
			std::map< std::string, IElement::ptr, unify::CaseInsensitiveLessThanTest > m_elementMap;
		};
	}
}