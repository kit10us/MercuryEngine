// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/IRenderer.h>
#include <me/RenderInfo.h>
#include <memory>

namespace me
{
	namespace scene2d
	{
		class IElement
		{
		public:
			typedef std::shared_ptr< IElement > ptr;

			virtual bool IsEnabled() const = 0;
			virtual void SetEnabled( bool enabled ) = 0;

			virtual unify::V2< float > GetOffset() const = 0;
			virtual unify::Size< float > GetSize( unify::Size< float > area ) const = 0;

			/// <summary>
			/// Called immediately after Update, to allow automatic layout processing. Area is the
			/// actual area space we area inside of.
			/// </summary>
			virtual void UpdateLayout( UpdateParams params, unify::Size< float > area ) = 0;

			virtual void Update( UpdateParams params ) = 0;
			virtual void Render( RenderParams params ) = 0;
			virtual void OnSuspend() = 0;
			virtual void OnResume() = 0;
		};
	}
}