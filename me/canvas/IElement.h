// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/render/IRenderer.h>
#include <me/render/RenderInfo.h>
#include <me/UpdateParams.h>
#include <me/render/RenderParams.h>
#include <memory>

namespace me
{
	namespace canvas
	{
		/// <summary>
		/// The interface for all other canvas elements.
		/// </summary>
		class IElement
		{
		public:
			typedef std::shared_ptr< IElement > ptr;
			typedef std::shared_ptr< const IElement > const_ptr;

			/// <summary>
			/// Set the name of this element.
			/// </summary>
			virtual void SetName( std::string name ) = 0;

			/// <summary>
			/// Get the name of this element.
			/// </summary>
			virtual std::string GetName() const = 0;

			/// <summary>
			/// Check if the element is enabled.
			/// </summary>
			virtual bool IsEnabled() const = 0;

			/// <summary>
			/// Make the element enabled or disabled.
			/// </summary>
			virtual void SetEnabled( bool enabled ) = 0;

			/// <summary>
			/// Set the offset of this element. This is in respect to the parent's offset.
			/// </summary>
			virtual void SetOffset( unify::V2< float > offset ) = 0;

			/// <summary>
			/// Get the offset of this element. This is in respect to the parent's offset.
			/// </summary>
			virtual unify::V2< float > GetOffset() const = 0;

			/// <summary>
			/// Returns the size of the content of the element.
			/// </summary>
			virtual unify::Size< float > GetContentSize() const = 0;

			/// <summary>
			/// Called immediately after Update, to allow automatic layout processing. Area is the
			/// actual area space we area inside of.
			/// </summary>
			virtual void UpdateLayout( UpdateParams params, unify::Rect< float > parentArea ) = 0;

			/// <summary>
			/// Updates the element. What this actually does is element type specific.
			/// </summary>
			virtual void Update( const UpdateParams & params ) = 0;

			/// <summary>
			/// Renders the element.
			/// </summary>
			virtual void Render( const render::Params & params ) = 0;

			/// <summary>
			/// Performed on suspending the element.
			/// </summary>
			virtual void OnSuspend() = 0;

			/// <summary>
			/// Performed on the element resuming after having been suspended.
			/// </summary>
			virtual void OnResume() = 0;
		};
	}
}