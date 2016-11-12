// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

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

			virtual void Update( const RenderInfo & renderInfo ) = 0;
			virtual void Render( const RenderInfo & renderInfo ) = 0;
			virtual void OnSuspend() = 0;
			virtual void OnResume() = 0;
		};
	}
}