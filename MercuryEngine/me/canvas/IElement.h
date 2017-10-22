// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/render/IRenderer.h>
#include <me/render/RenderInfo.h>
#include <memory>

namespace me
{
	namespace canvas
	{
		class IElement
		{
		public:
			typedef std::shared_ptr< IElement > ptr;

			virtual bool IsEnabled() const = 0;
			virtual void SetEnabled( bool enabled ) = 0;

			virtual void SetOffset( unify::V2< float > offset ) = 0;
			virtual unify::V2< float > GetOffset() const = 0;

			/// <summary>
			/// Returns the size of the content of the element (with no regard to parent).
			/// </summary>
			virtual unify::Size< float > GetContentSize() const = 0;

			/// <summary>
			/// Called immediately after Update, to allow automatic layout processing. Area is the
			/// actual area space we area inside of.
			/// </summary>
			virtual void UpdateLayout( UpdateParams params, unify::Rect< float > parentArea ) = 0;

			virtual void Update( UpdateParams params ) = 0;
			virtual void Render( render::Params params ) = 0;
			virtual void OnSuspend() = 0;
			virtual void OnResume() = 0;
		};
	}
}