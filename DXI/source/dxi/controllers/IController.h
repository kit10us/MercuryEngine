// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved
#pragma once

#include <dxi/core/IInput.h>
#include <unify/unify.h>
#include <dxi/RenderInfo.h>
#include <memory>

namespace dxi
{
	namespace controllers
	{
		class IController
		{
		public:
			typedef std::shared_ptr< IController > shared_ptr;

			virtual ~IController() {}
			virtual void Update( const RenderInfo & renderInfo, core::IInput & input ) = 0;
		};
	}
}