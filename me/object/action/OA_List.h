// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/object/action/IObjectAction.h>
#include <list>

namespace me
{
	namespace object
	{
		namespace action
		{
			class List : public IObjectAction
			{
			public:
				List( std::initializer_list< IObjectAction::ptr > actions );

				bool Perform( Object* object, unify::TimeDelta delta ) override;

			private:
				std::vector< IObjectAction::ptr > m_actions;
			};
		}
	}
}