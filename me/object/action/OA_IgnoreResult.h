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
			class IgnoreResult : public IObjectAction
			{
			public:
				IgnoreResult( IObjectAction::ptr action );

				bool Perform( Object* object, unify::TimeDelta delta ) override;

			private:
				IObjectAction::ptr m_action;
			};
		}
	}
}