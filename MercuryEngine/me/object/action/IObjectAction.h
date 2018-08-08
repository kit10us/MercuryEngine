// Copyright (c) 2002 - 2018, Evil Quail LLC
// All Rights Reserved

#pragma once

#include <me/object/Object.h>
#include <memory>

namespace me
{
	namespace object
	{
		namespace action
		{
			/// <summary>
			/// Actions that are performed on an object.
			/// </summary>
			class IObjectAction
			{
			public:
				typedef std::shared_ptr< IObjectAction > ptr;

				virtual ~IObjectAction() {}

				virtual bool Perform(Object* object, float delta ) = 0;
			};
		}
	}
}